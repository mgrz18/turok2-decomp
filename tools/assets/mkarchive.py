#!/usr/bin/env python3
"""Build the port's game archive (turok2.o2r) from the user's own Turok 2 ROM.

    python tools/assets/mkarchive.py baserom.us.z64 -o turok2.o2r [--folder DIR] [-j N]

The archive is an O2R (a zip) that libultraship's ArchiveManager mounts. It holds
one entry per engine load unit (romfs.RomFS.load_units(): the (ROM key, size)
pairs CCache__Request / CLoader / VM_Alloc read), named after the ROM address the
engine already uses as its cache key:

    version          libultraship version file (u8 byte order, u32 game version)
                     + T2 extension: 'T2AV', u16 major, u16 minor, ROM SHA1 (20 bytes)
    t2/info.json     ROM name / SHA1 / tool version / counts (human readable)
    t2/units         binary table of contents (little-endian; layout in ARCHIVE.md)
    t2/units.tsv     the same, readable (romfs.manifest_tsv)
    t2/types         type id -> type name / CCache descriptions (TSV)
    t2/swap          per-unit byte-swap plans (swap_spec.json run through swapcheck.py):
                     what a little-endian host reverses to get each unit in host order
    t2/u/XXXXXXXX    one load unit: 64-byte libultraship resource header (type Blob
                     'OBLB', v0) + u32 data size + the bytes. RNC streams are stored
                     unpacked; everything stays in ROM (big-endian) byte order: the
                     reader applies t2/swap once per load on a little-endian host.

Same ROM -> byte-identical archive (fixed entry order, timestamps and attributes;
deflate output depends only on the zlib version, recorded in t2/info.json; use
--store for an archive that does not depend on zlib at all).

Format, naming and the decisions behind them: docs/assets/ARCHIVE.md.
Check an archive against the ROM with tools/assets/archive_check.py.
"""
import argparse
import hashlib
import json
import multiprocessing
import os
import struct
import sys
import time
import zipfile
import zlib

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import rnc  # noqa: E402
import romfs  # noqa: E402
import romid  # noqa: E402
import swapcheck  # noqa: E402

TOOL = "mkarchive.py"
FORMAT_MAJOR = 2          # bump when an entry's layout or naming changes (the port re-extracts)
FORMAT_MINOR = 0          # 2.0: t2/swap (byte-swap plans) added; the port refuses 1.x archives

LUS_HEADER_SIZE = 0x40
LUS_ENDIAN_LITTLE = 0     # Ship::Endianness::Little
LUS_TYPE_BLOB = 0x4F424C42  # Ship::ResourceType::Blob ('OBLB')
LUS_BLOB_VERSION = 0      # ResourceFactoryBinaryBlobV0
T2_UNIT_MAGIC = b"T2LU"   # T2 block inside the header's reserved bytes (0x14..0x40)
T2_UNITS_MAGIC = b"T2UT"
T2_VERSION_MAGIC = b"T2AV"
T2_SWAP_MAGIC = b"T2SW"
SWAP_WIDTH_CODE = {2: 1, 4: 2, 8: 3}
SWAP_INDEX_SIZE = 20
UNIT_PREFIX = "t2/u/"
ZIP_DATE = (1980, 1, 1, 0, 0, 0)


def unit_name(key):
    return "%s%08X" % (UNIT_PREFIX, key)


def lus_header(row, type_id):
    """64-byte libultraship resource header for one unit (see ARCHIVE.md)."""
    h = bytearray(LUS_HEADER_SIZE)
    struct.pack_into("<BBH", h, 0x00, LUS_ENDIAN_LITTLE, 0, 0)          # byte order, isCustom, unused
    struct.pack_into("<II", h, 0x04, LUS_TYPE_BLOB, LUS_BLOB_VERSION)    # type, resource version
    struct.pack_into("<Q", h, 0x0C, row["key"])                          # id (= ROM key)
    h[0x14:0x18] = T2_UNIT_MAGIC                                         # LUS ignores 0x14..0x40
    struct.pack_into("<IIIHHHH", h, 0x18, row["key"], row["size"], row["host_size"],
                     type_id, row["flags"], FORMAT_MAJOR, FORMAT_MINOR)
    return bytes(h)


def unit_entry(row, type_id, data):
    if len(data) != row["host_size"]:
        raise ValueError("unit %08X: %d bytes, manifest says %d" % (row["key"], len(data), row["host_size"]))
    return lus_header(row, type_id) + struct.pack("<I", len(data)) + data


def units_table(rows, type_ids):
    """t2/units: 'T2UT' u16 major u16 minor u32 count u32 typeCount, then count x
    {u32 key, u32 romSize, u32 hostSize, u16 type, u16 flags}, sorted by key (little-endian)."""
    out = bytearray(T2_UNITS_MAGIC + struct.pack("<HHII", FORMAT_MAJOR, FORMAT_MINOR, len(rows), len(type_ids)))
    for r in rows:
        out += struct.pack("<IIIHH", r["key"], r["size"], r["host_size"], type_ids[r["type"]], r["flags"])
    return bytes(out)


def version_file(rom, sha1):
    """libultraship 'version' (u8 byte order + u32 game version = ROM header CRC1, which
    the port registers with ArchiveManager as the valid game version) + T2 extension."""
    crc1 = struct.unpack_from(">I", rom, 0x10)[0]
    return (struct.pack("<BI", LUS_ENDIAN_LITTLE, crc1) + T2_VERSION_MAGIC +
            struct.pack("<HH", FORMAT_MAJOR, FORMAT_MINOR) + bytes.fromhex(sha1))


def swap_plan_bytes(runs, planes):
    """One unit's plan (ARCHIVE.md "t2/swap"): runs {u32 offset, u32 count << 2 | width code},
    then tables {u32 offset, u32 records, u32 swaps, swaps x {u16 plane, u16 width}}."""
    out = bytearray()
    for o, w, n in runs:
        if n >= 1 << 30:
            raise ValueError("swap run too long")
        out += struct.pack("<II", o, (n << 2) | SWAP_WIDTH_CODE[w])
    for d, n, swaps in planes:
        out += struct.pack("<III", d, n, len(swaps))
        for p, w in swaps:
            out += struct.pack("<HH", p, w)
    return bytes(out)


def swap_table(rows, plans):
    """t2/swap: 'T2SW' u16 major u16 minor u32 count u32 0, then count x {u32 key, u32 hostSize,
    u32 offset (from the start of the entry), u32 runs, u32 tables}, sorted by key, then the plans.
    Units whose plan is empty (byte streams, texels, VADPCM) have no record."""
    recs = [(r, plans[r["key"]]) for r in rows if plans[r["key"]][0] or plans[r["key"]][1]]
    off = 16 + SWAP_INDEX_SIZE * len(recs)
    head = bytearray(T2_SWAP_MAGIC + struct.pack("<HHII", FORMAT_MAJOR, FORMAT_MINOR, len(recs), 0))
    body = bytearray()
    for r, (runs, planes, blob) in recs:
        head += struct.pack("<IIIII", r["key"], r["host_size"], off + len(body), len(runs), len(planes))
        body += blob
    return bytes(head + body)


# ---------------------------------------------------------------- unit bytes
_ROM = None
_DATA = None
_SPEC = None


def _init(rom):
    global _ROM
    _ROM = rom


def _unpack(job):
    key, method = job
    return rnc.decompress(_ROM, key, verify=True)


def unit_data(rom, rows, jobs=1):
    """{key: bytes} for every unit: RNC units unpacked (CRC16 checked), the rest as stored."""
    packed = [(r["key"], r["kind"]) for r in rows if r["flags"] & romfs.UF_RNC_UNPACKED]
    if jobs > 1:
        ctx = multiprocessing.get_context("fork")
        with ctx.Pool(jobs, _init, (rom,)) as pool:
            unp = pool.map(_unpack, packed, chunksize=64)
    else:
        _init(rom)
        unp = [_unpack(j) for j in packed]
    data = dict(zip((k for k, _ in packed), unp))
    for r in rows:
        if r["key"] not in data:
            data[r["key"]] = rom[r["key"]:r["key"] + r["size"]]
    return data


def _plan(job):
    key, typ = job
    runs, planes, errors = swapcheck.unit_plan(_SPEC, _DATA[key], key, typ)
    return key, runs, planes, swap_plan_bytes(runs, planes), errors


def swap_plans(rows, data, jobs=1):
    """{key: (runs, planes, plan bytes)} for every unit, from tools/assets/swap_spec.json."""
    global _DATA, _SPEC
    _DATA = data
    _SPEC = swapcheck.Spec(os.path.join(os.path.dirname(os.path.abspath(__file__)), "swap_spec.json"),
                           os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
    work = [(r["key"], r["type"]) for r in rows]
    if jobs > 1:
        ctx = multiprocessing.get_context("fork")
        with ctx.Pool(jobs) as pool:
            res = pool.map(_plan, work, chunksize=64)
    else:
        res = [_plan(j) for j in work]
    _DATA = None
    bad = [(k, e) for k, _, _, _, e in res if e]
    if bad:
        raise SystemExit("swap spec errors in %d units, first %08X: %s" % (len(bad), bad[0][0], bad[0][1][:3]))
    return {k: (runs, planes, blob) for k, runs, planes, blob, _ in res}


# ---------------------------------------------------------------- build
def check_rows(rows):
    """Decisions the archive relies on (ARCHIVE.md "Decisions"): unique keys, units do
    not overlap, no container carries RNC children, RNC units are whole streams."""
    prev_end = 0
    for r in rows:
        if r["key"] < prev_end:
            raise ValueError("unit %08X overlaps the previous unit" % r["key"])
        prev_end = r["key"] + r["size"]
        if r["flags"] & romfs.UF_RNC_CHILDREN:
            raise ValueError("unit %08X: container with RNC children (needs KEEP_PACKED or a table rewrite)"
                             % r["key"])


def build(rom_bytes, jobs=1, log=print):
    """-> (entries: list of (name, bytes) in archive order, info dict)."""
    t0 = time.time()
    rom, sha1, known = romid.identify(rom_bytes)
    if rom is None:
        raise SystemExit("not an N64 ROM image")
    if not known or not known.get("supported"):
        raise SystemExit("unsupported ROM (sha1 %s); the port needs %s (%s)" % (
            sha1, romid.SUPPORTED_SHA1, romid.KNOWN[romid.SUPPORTED_SHA1]["name"]))
    fs = romfs.RomFS(rom)
    rows = fs.manifest()
    check_rows(rows)
    types = sorted({r["type"] for r in rows})
    type_ids = {t: i for i, t in enumerate(types)}
    descs = {}
    for r in rows:
        descs.setdefault(r["type"], r["description"])
    log("%d load units, %d types (%.1fs)" % (len(rows), len(types), time.time() - t0))
    data = unit_data(rom, rows, jobs)
    log("unit bytes ready (%.1fs)" % (time.time() - t0))
    plans = swap_plans(rows, data, jobs)
    swap = swap_table(rows, plans)
    log("swap plans ready: %d bytes (%.1fs)" % (len(swap), time.time() - t0))

    info = {
        "rom": known["name"], "sha1": sha1, "tool": TOOL,
        "format": "%d.%d" % (FORMAT_MAJOR, FORMAT_MINOR),
        "units": len(rows), "types": len(types),
        "rom_bytes": sum(r["size"] for r in rows),
        "host_bytes": sum(r["host_size"] for r in rows),
        "rnc_unpacked": sum(1 for r in rows if r["flags"] & romfs.UF_RNC_UNPACKED),
        "zlib": zlib.ZLIB_VERSION,
        "swap_units": sum(1 for p in plans.values() if p[0] or p[1]),
        "swap_runs": sum(len(p[0]) for p in plans.values()),
        "swap_tables": sum(len(p[1]) for p in plans.values()),
        "swap_bytes": len(swap),
    }
    entries = [
        ("version", version_file(rom, sha1)),
        ("t2/info.json", (json.dumps(info, indent=1, sort_keys=True) + "\n").encode()),
        ("t2/units", units_table(rows, type_ids)),
        ("t2/units.tsv", romfs.manifest_tsv(rows).encode()),
        ("t2/types", "".join("%d\t%s\t%s\n" % (i, t, descs[t]) for i, t in enumerate(types)).encode()),
        ("t2/swap", swap),
    ]
    for r in rows:
        entries.append((unit_name(r["key"]), unit_entry(r, type_ids[r["type"]], data[r["key"]])))
    return entries, info


def write_zip(entries, path, store=False, level=6):
    tmp = path + ".tmp"
    comp = zipfile.ZIP_STORED if store else zipfile.ZIP_DEFLATED
    with zipfile.ZipFile(tmp, "w", comp, allowZip64=True) as z:
        for name, data in entries:
            zi = zipfile.ZipInfo(name, date_time=ZIP_DATE)
            zi.compress_type = comp
            zi.create_system = 3                  # unix; fixed so the host OS does not leak in
            zi.external_attr = 0o100644 << 16      # regular file, rw-r--r--
            if store:
                z.writestr(zi, data)
            else:
                z.writestr(zi, data, compress_type=comp, compresslevel=level)
    os.replace(tmp, path)


def write_folder(entries, root):
    for name, data in entries:
        p = os.path.join(root, *name.split("/"))
        os.makedirs(os.path.dirname(p), exist_ok=True)
        with open(p, "wb") as f:
            f.write(data)


def main(argv=None):
    ap = argparse.ArgumentParser(description="Build turok2.o2r from a Turok 2 (USA) ROM.")
    ap.add_argument("rom", help="the user's ROM (.z64 / .v64 / .n64)")
    ap.add_argument("-o", "--out", default="turok2.o2r", help="archive path (default turok2.o2r)")
    ap.add_argument("--folder", help="also write the same tree as a folder (libultraship FolderArchive)")
    ap.add_argument("--store", action="store_true", help="no compression (zlib-independent bytes)")
    ap.add_argument("--level", type=int, default=6, help="deflate level (default 6)")
    ap.add_argument("-j", "--jobs", type=int, default=min(8, os.cpu_count() or 1),
                    help="processes for RNC unpacking and swap plans (default min(8, cpus))")
    a = ap.parse_args(argv)
    t0 = time.time()
    with open(a.rom, "rb") as f:
        entries, info = build(f.read(), a.jobs)
    write_zip(entries, a.out, a.store, a.level)
    if a.folder:
        write_folder(entries, a.folder)
    size = os.path.getsize(a.out)
    with open(a.out, "rb") as f:
        digest = hashlib.sha1(f.read()).hexdigest()
    print("%s: %d entries (%d units), %d bytes of unit data, archive %d bytes, sha1 %s, %.1fs" % (
        a.out, len(entries), info["units"], info["host_bytes"], size, digest, time.time() - t0))
    return 0


if __name__ == "__main__":
    sys.exit(main())
