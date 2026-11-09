#!/usr/bin/env python3
"""Check a turok2.o2r against the ROM it claims to come from.

    python tools/assets/archive_check.py turok2.o2r baserom.us.z64 [--folder DIR]

Opens the archive with the stdlib zipfile (independent of the builder's writer) and
checks, against romfs walked afresh from the ROM:

  * the libultraship 'version' entry (byte order, game version = ROM CRC1) and its
    T2 extension (format version, SHA1 of the ROM);
  * t2/units: one record per load unit, equal to RomFS.manifest() (key, ROM size, host
    size, type, flags), sorted, no overlaps; t2/types covers every type id;
  * every t2/u/XXXXXXXX entry: the 64-byte libultraship header (Little, Blob 'OBLB' v0,
    id = key, 'T2LU' block = key / ROM size / host size / type / flags / format), the
    u32 size, and the bytes: ROM[key:key+size] for stored units, the RNC stream's
    unpacked bytes (length = header size, CRC16 = header CRC) for RNC units;
  * t2/swap (format 2): equal to the plans recomputed from swap_spec.json on the checked unit
    bytes (swapcheck.unit_plan, as mkarchive.py builds them), and every plan, applied twice
    (swapcheck.apply_plan), gives the unit's bytes back;
  * no missing or extra entries, zip CRCs good, fixed timestamps.
With --folder, also checks that the folder tree has the same files with the same bytes.
Exit status 0 = all good.
"""
import argparse
import os
import struct
import sys
import time
import zipfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import mkarchive  # noqa: E402
import rnc  # noqa: E402
import romfs  # noqa: E402
import romid  # noqa: E402
import swapcheck  # noqa: E402


def check(archive, rom_path, folder=None, log=print, jobs=1):
    t0 = time.time()
    errors = []

    def err(msg):
        if len(errors) < 50:
            log("ERROR " + msg)
        errors.append(msg)

    with open(rom_path, "rb") as f:
        rom, sha1, known = romid.identify(f.read())
    if rom is None or not known:
        raise SystemExit("not the supported ROM")
    fs = romfs.RomFS(rom)
    rows = fs.manifest()
    z = zipfile.ZipFile(archive)
    infos = z.infolist()
    names = [i.filename for i in infos]
    if len(set(names)) != len(names):
        err("duplicate entry names")
    for i in infos:
        if i.date_time != mkarchive.ZIP_DATE:
            err("%s: timestamp %r" % (i.filename, i.date_time))
            break
    bad = z.testzip()
    if bad:
        err("zip CRC error in %s" % bad)

    # version
    v = z.read("version")
    order, crc1 = struct.unpack_from("<BI", v, 0)
    if order != 0 or crc1 != struct.unpack_from(">I", rom, 0x10)[0]:
        err("version: byte order %d / game version 0x%X" % (order, crc1))
    if v[5:9] != mkarchive.T2_VERSION_MAGIC or v[13:33].hex() != sha1:
        err("version: T2 block / SHA1 mismatch")
    fmt = struct.unpack_from("<HH", v, 9)

    # units table
    types = [line.split("\t")[1] for line in z.read("t2/types").decode().splitlines()]
    tab = z.read("t2/units")
    magic, major, minor, count, ntypes = struct.unpack_from("<4sHHII", tab, 0)
    if magic != mkarchive.T2_UNITS_MAGIC or (major, minor) != fmt or count != len(rows) or ntypes != len(types):
        err("t2/units header: %r %d.%d count %d types %d" % (magic, major, minor, count, ntypes))
    recs = [struct.unpack_from("<IIIHH", tab, 16 + 16 * i) for i in range(count)]
    if len(tab) != 16 + 16 * count:
        err("t2/units: size %d" % len(tab))
    for r, (k, s, h, t, fl) in zip(rows, recs):
        if (k, s, h, fl) != (r["key"], r["size"], r["host_size"], r["flags"]) or types[t] != r["type"]:
            err("t2/units: record %08X differs from romfs" % k)
    if [x[0] for x in recs] != sorted(x[0] for x in recs):
        err("t2/units not sorted")

    # unit entries
    expected = {mkarchive.unit_name(r["key"]) for r in rows}
    meta = {"version", "t2/info.json", "t2/units", "t2/units.tsv", "t2/types", "t2/swap"}
    if fmt[0] != mkarchive.FORMAT_MAJOR:
        err("format %d.%d, this checker expects %d.x (rebuild the archive)" % (fmt[0], fmt[1], mkarchive.FORMAT_MAJOR))
    extra = set(names) - expected - meta
    missing = (expected | meta) - set(names)
    if extra:
        err("extra entries: %s" % sorted(extra)[:5])
    if missing:
        err("missing entries: %s" % sorted(missing)[:5])
    n_rnc = n_raw = host = 0
    unit_bytes = {}
    for r in rows:
        name = mkarchive.unit_name(r["key"])
        if name not in names:
            continue
        e = z.read(name)
        hdr = e[:mkarchive.LUS_HEADER_SIZE]
        order, custom, _, typ, ver, ident = struct.unpack_from("<BBHIIQ", hdr, 0)
        t2 = struct.unpack_from("<4sIIIHHHH", hdr, 0x14)
        if (order, custom, typ, ver, ident) != (0, 0, mkarchive.LUS_TYPE_BLOB, 0, r["key"]):
            err("%s: libultraship header %r" % (name, (order, custom, typ, ver, ident)))
        if t2[0] != mkarchive.T2_UNIT_MAGIC or t2[1:4] != (r["key"], r["size"], r["host_size"]) \
                or types[t2[4]] != r["type"] or t2[5] != r["flags"] or t2[6:8] != fmt:
            err("%s: T2 block %r" % (name, t2))
        size = struct.unpack_from("<I", e, mkarchive.LUS_HEADER_SIZE)[0]
        data = e[mkarchive.LUS_HEADER_SIZE + 4:]
        if size != len(data) or size != r["host_size"]:
            err("%s: size %d / %d bytes / manifest %d" % (name, size, len(data), r["host_size"]))
        k = r["key"]
        if r["flags"] & romfs.UF_RNC_UNPACKED:
            hd = rnc.header(rom, k)
            if 18 + hd["packed"] > r["size"]:
                err("%s: RNC stream longer than its slot" % name)
            if len(data) != hd["unpacked"] or rnc.crc16(data) != hd["crc_unpacked"] \
                    or data != rnc.decompress(rom, k):
                err("%s: unpacked bytes differ from the RNC stream" % name)
            n_rnc += 1
        else:
            if rnc.is_rnc(rom, k):
                err("%s: RNC stream stored packed" % name)
            if data != rom[k:k + r["size"]]:
                err("%s: bytes differ from ROM 0x%X+0x%X" % (name, k, r["size"]))
            n_raw += 1
        host += len(data)
        unit_bytes[k] = data
        if folder:
            p = os.path.join(folder, *name.split("/"))
            if not os.path.exists(p) or open(p, "rb").read() != e:
                err("%s: folder copy differs" % name)
    if folder:
        for m in meta:
            p = os.path.join(folder, *m.split("/"))
            if not os.path.exists(p) or open(p, "rb").read() != z.read(m):
                err("%s: folder copy differs" % m)
    # byte-swap plans
    n_plans = 0
    if "t2/swap" in names and len(unit_bytes) == len(rows):
        plans = mkarchive.swap_plans(rows, unit_bytes, jobs)
        if z.read("t2/swap") != mkarchive.swap_table(rows, plans):
            err("t2/swap differs from the plans of swap_spec.json")
        for r in rows:
            runs, planes, _ = plans[r["key"]]
            if not runs and not planes:
                continue
            n_plans += 1
            d = unit_bytes[r["key"]]
            if swapcheck.apply_plan(swapcheck.apply_plan(d, runs, planes), runs, planes) != d:
                err("t2/swap: plan of %08X is not an involution" % r["key"])
    dt = time.time() - t0
    log("%s: %d entries, %d units (%d RNC unpacked, %d stored, %d with a swap plan), %d unit bytes, archive %d bytes, "
        "format %d.%d, rom sha1 %s: %s (%.1fs)" % (
            archive, len(names), len(rows), n_rnc, n_raw, n_plans, host, os.path.getsize(archive), fmt[0], fmt[1],
            sha1, "OK" if not errors else "%d ERRORS" % len(errors), dt))
    return errors


def main(argv=None):
    ap = argparse.ArgumentParser(description="Check turok2.o2r against the ROM.")
    ap.add_argument("archive")
    ap.add_argument("rom")
    ap.add_argument("--folder", help="also compare a --folder tree written by mkarchive.py")
    ap.add_argument("-j", "--jobs", type=int, default=min(4, os.cpu_count() or 1),
                    help="processes for recomputing the swap plans (default min(4, cpus))")
    a = ap.parse_args(argv)
    return 1 if check(a.archive, a.rom, a.folder, jobs=a.jobs) else 0


if __name__ == "__main__":
    sys.exit(main())
