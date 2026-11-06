#!/usr/bin/env python3
"""Turok 2 ROM filesystem: the asset region, its block sets and their types.

Everything the engine loads goes through CCache__Request(key = ROM address, size,
..., decompressCallback, description). The ROM addresses come from nested
"ISets" (CISet__GetBlock / GetROMAddressAndSize, src/us/code/func_002017D4.c ..
func_0020188C.c):

    ISet  = u32 count; u32 offset[count + 1]; data...
            entry i = bytes [offset[i], offset[i+1]) relative to the set start
            (offset[0] = 8 + 4*count rounded up to 8; offset[count] = set size)
    USet  = u32 elemSize; u32 count; count * elemSize bytes   (CUSet__*)
    RNC   = 'RNC' + method ... (ProPack, see propack.py); a block whose callback
            is ProPack__Decompress / Model_Decompress* / Texture_Decompress* etc.
            is stored compressed and expanded after the DMA.

Top-level regions (ROM offsets; all four are found in the C, see docs/assets/ROMFS.md):
    0x01E0000-0x020F194  audio control block   VM_Alloc(0x2F194, 0, 0x1E0000), CAudioWorld__Initialize
    0x020F194-0x0BE1814  audio wave table      gAudioRomBase = 0x20F194 (ADPCM samples)
    0x0BE1814-0x0FCBFB4  sound data file       VM_Alloc(0x3EA7A0, 0, 0xBE1814), CAudioWorld__LoadSfxDataFile
    0x0FCBFB4-0x1FF7AEC  cartridge index       CEngineApp m_CartridgeIndex = 0xFCBFB4, CScene__Construct

Python API (import romfs):
    fs = romfs.RomFS("baserom.us.z64")            # or RomFS(bytes)
    fs.regions                                    # list of Region(name, start, end, desc)
    n = fs.node("cart/5/12/8")                    # Node by path (lazy); "cart/levels/12/..." not needed
    n.type, n.kind, n.rom, n.size, n.compressed, n.unpacked_size, n.count
    fs.children(n)                                # child Nodes (descends into RNC blobs too)
    fs.read(n)                                    # stored bytes (what the DMA reads)
    fs.blob(n)                                    # bytes after ProPack (= read(n) when not RNC)
    fs.walk("cart", decompress=False)             # generator of Nodes, depth first
    fs.object(i), fs.texture_set(i), fs.level(i), fs.graphicset(i), fs.cinema(i)  -> Node
    fs.count("objects") etc.  (objects, texture_sets, levels, graphicsets, cinemas)
    fs.find_rom(addr)                             # deepest ROM-level node containing addr

Paths: "<region>/<i>/<j>/...". Inside a compressed block the path simply goes on
(the children are entries of the unpacked blob); such nodes have rom=None and
`container` = the ROM node of the compressed block, `offset` = offset in its blob.

CLI:
    python tools/assets/romfs.py baserom.us.z64 --out DIR [--depth N] [--decompress]
        [--extract] [--coverage]
    writes DIR/romfs_tree.tsv (+ .json), DIR/romfs_types.tsv; --extract writes
    DIR/blobs/<path>.bin (stored) and .unp.bin (decompressed) for ROM leaves.
"""
import fnmatch
import json
import os
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import propack  # noqa: E402

ASSET_START = 0x1E0000
ASSET_END = 0x1FF7AEC
AUDIO_CTL = 0x1E0000
AUDIO_CTL_SIZE = 0x2F194
AUDIO_CTL_VRAM = 0x480000      # its absolute pointers are relocated to this virtual address
AUDIO_TBL = 0x20F194           # gAudioRomBase
SOUND_DATA = 0xBE1814
SOUND_DATA_SIZE = 0x3EA7A0
CARTRIDGE = 0xFCBFB4


class Region:
    def __init__(self, name, start, end, desc):
        self.name, self.start, self.end, self.desc = name, start, end, desc

    def __repr__(self):
        return "Region(%s 0x%X-0x%X)" % (self.name, self.start, self.end)


REGIONS = [
    Region("audioctl", AUDIO_CTL, AUDIO_CTL + AUDIO_CTL_SIZE,
           "audio control block: ALBankFile(s) and sound/instrument structures, pointers relocated to 0x480000"),
    Region("audiotbl", AUDIO_TBL, SOUND_DATA, "audio wave table (ADPCM sample data, ALWaveTable.base relative to 0x20F194)"),
    Region("sound", SOUND_DATA, SOUND_DATA + SOUND_DATA_SIZE, "sound data file (ISet): sfx tables"),
    Region("cart", CARTRIDGE, ASSET_END, "cartridge index (ISet tree of every game asset)"),
]

# ---------------------------------------------------------------- types
# Established from the C (descriptions = CCache__Request strings; see ROMFS.md).
CART_NAMES = {
    0: "objects index", 1: "intelligence", 2: "object types", 3: "texture sets index",
    4: "particle effects", 5: "levels index", 6: "load/save persistent bit sections",
    7: "game persistent bit sections", 8: "warp dests", 9: "binaries index",
    10: "object types 2", 11: "particle swooshes", 12: "dynamic lights", 13: "curves",
    14: "rumbles", 15: "rumble types", 16: "graphicset index", 17: "graphicset type",
    18: "Cinemas index", 19: "link group types", 20: "end marker",
}
LEVEL_NAMES = {0: "level info", 1: "collision", 2: "level paths", 3: "animated instances",
               4: "grid bounds", 5: "grid sections index", 6: "sky layers", 7: "links",
               8: "level name"}
OBJECT_NAMES = {0: "anim object info", 1: "geometry", 2: "animations index", 3: "static events"}

# (pattern, type, how to parse: "iset", "uset", "rnc", "raw", "auto")
SCHEMA = [
    ("cart", "cartridge index", "iset"),
    ("cart/0", "objects index", "iset"),
    ("cart/0/*", "object index", "iset"),
    ("cart/0/*/0", "anim object info", "auto"),
    ("cart/0/*/1", "geometry", "auto"),
    ("cart/0/*/2", "animations index", "iset"),
    ("cart/0/*/2/*", "animation", "auto"),
    ("cart/0/*/3", "static events", "auto"),
    ("cart/3", "texture sets index", "iset"),
    ("cart/3/*", "texture", "auto"),
    ("cart/5", "levels index", "iset"),
    ("cart/5/*", "level index", "iset"),
    ("cart/5/*/5", "grid sections index", "iset"),
    ("cart/5/*/5/*", "grid section", "auto"),
    ("cart/5/*/8", "level name", "raw"),
    ("cart/9", "binaries index", "iset"),
    ("cart/9/*", "binary block", "auto"),
    ("cart/16", "graphicset index", "iset"),
    ("cart/16/*", "graphicset", "auto"),
    ("cart/18", "Cinemas index", "iset"),
    ("cart/18/0", "cinema table", "raw"),
    ("cart/18/*", "Cinema", "auto"),
    ("cart/20", "end marker", "raw"),
    ("sound", "sound data file", "iset"),
    ("audioctl", "audio control block", "raw"),
    ("audiotbl", "audio wave table", "raw"),
]
for _k, _v in CART_NAMES.items():
    if not any(p == "cart/%d" % _k for p, _, _ in SCHEMA):
        SCHEMA.append(("cart/%d" % _k, _v, "auto"))
for _k, _v in LEVEL_NAMES.items():
    if not any(p == "cart/5/*/%d" % _k for p, _, _ in SCHEMA):
        SCHEMA.append(("cart/5/*/%d" % _k, _v, "auto"))


def _schema(path):
    for pat, typ, how in SCHEMA:
        if fnmatch.fnmatchcase(path, pat) and pat.count("/") == path.count("/"):
            return typ, how
    return None, "auto"


# ---------------------------------------------------------------- parsing helpers
def u32(b, o):
    return struct.unpack_from(">I", b, o)[0]


def parse_iset(b, o, size, strict=False):
    """Offsets of an ISet at b[o:o+size] or None. strict=False is the schema case."""
    if size < 8 or o + 8 > len(b):
        return None
    n = u32(b, o)
    if n > 0x10000 or 8 + 4 * n > size:
        return None
    offs = struct.unpack_from(">%dI" % (n + 1), b, o + 4)
    h = 8 + 4 * n
    if offs[0] < h or offs[0] - h >= 16:
        return None
    for i in range(n):
        if offs[i + 1] < offs[i]:
            return None
    if offs[n] > size or size - offs[n] >= 16:
        return None
    if strict and n == 0 and size != 8:
        return None
    return offs


def is_uset(b, o, size):
    if size < 8:
        return False
    es, n = struct.unpack_from(">II", b, o)
    return 0 < es < 0x10000 and n < 0x100000 and 0 <= size - (es * n + 8) < 8 and es * n > 0


class Node:
    __slots__ = ("path", "type", "kind", "rom", "container", "offset", "size", "compressed",
                 "method", "unpacked_size", "count", "_offs", "_buf", "_base", "heuristic")

    def __repr__(self):
        loc = ("rom 0x%X" % self.rom) if self.rom is not None else ("in 0x%X+0x%X" % (self.container.rom, self.offset))
        extra = " RNC%d->0x%X" % (self.method, self.unpacked_size) if self.compressed else ""
        cnt = " n=%d" % self.count if self.count is not None else ""
        return "<%s %s %s size 0x%X %s%s%s>" % (self.path, self.type, loc, self.size, self.kind, cnt, extra)

    @property
    def name(self):
        return self.path.rsplit("/", 1)[-1]


class RomFS:
    def __init__(self, rom):
        if isinstance(rom, (bytes, bytearray)):
            self.rom = bytes(rom)
        else:
            with open(rom, "rb") as f:
                self.rom = f.read()
        if len(self.rom) < ASSET_END or u32(self.rom, CARTRIDGE) != 21:
            raise ValueError("not a Turok 2 (US) ROM: cartridge index not found at 0x%X" % CARTRIDGE)
        self.regions = REGIONS
        self._cache = {}
        self._blobs = {}
        self._kids = {}

    def _forget(self, n):
        for c in self._kids.pop(n.path, ()):
            self._forget(c)
        self._cache.pop(n.path, None)

    # -------- node construction
    def _make(self, path, buf, base, off, size, rom, container):
        """buf[base+off : base+off+size] is the node's stored bytes."""
        n = Node()
        n.path, n.rom, n.container, n.offset, n.size = path, rom, container, off, size
        n._buf, n._base = buf, base + off
        n.compressed, n.method, n.unpacked_size, n.count, n._offs = False, None, None, None, None
        n.heuristic = False
        typ, how = _schema(path)
        if typ is None:
            parent = path.rsplit("/", 1)[0]
            ptyp = self._cache[parent].type if parent in self._cache else "?"
            typ = "%s[%s]" % (ptyp, n.name)
        n.type = typ
        p = n._base
        if how == "raw" or size == 0:
            n.kind = "raw"
        elif propack.is_rnc(buf, p) and size >= propack.HEADER_LEN:
            h = propack.header(buf, p)
            n.kind, n.compressed, n.method, n.unpacked_size = "rnc", True, h["method"], h["unpacked"]
        else:
            offs = parse_iset(buf, p, size, strict=(how != "iset"))
            if offs is not None and (how == "iset" or len(offs) > 1 or size == 8):
                n.kind, n._offs, n.count = "iset", offs, len(offs) - 1
                n.heuristic = how != "iset"
                if n.count == 0:
                    n.kind = "empty"
            elif is_uset(buf, p, size):
                n.kind = "uset"
                n.count = u32(buf, p + 4)
            elif size == 8 and buf[p:p + 8] == b"\0" * 8:
                n.kind = "empty"
            else:
                n.kind = "raw"
        self._cache[path] = n
        return n

    def _region_node(self, r):
        if r.name in self._cache:
            return self._cache[r.name]
        return self._make(r.name, self.rom, r.start, 0, r.end - r.start, r.start, None)

    def node(self, path):
        path = path.strip("/")
        if path in self._cache:
            return self._cache[path]
        parts = path.split("/")
        reg = [r for r in self.regions if r.name == parts[0]]
        if not reg:
            raise KeyError(path)
        n = self._region_node(reg[0])
        for p in parts[1:]:
            kids = self.children(n)
            i = int(p)
            if i >= len(kids):
                raise KeyError(path)
            n = kids[i]
        return n

    def children(self, n):
        if n.kind == "rnc":
            blob = self.blob(n)
            offs = parse_iset(blob, 0, len(blob))
            if offs is None:
                return []
            buf, base, container = blob, 0, n
        elif n.kind == "iset":
            offs, buf, base = n._offs, n._buf, n._base
            container = n.container
        else:
            return []
        out = []
        for i in range(len(offs) - 1):
            p = "%s/%d" % (n.path, i)
            if p in self._cache:
                out.append(self._cache[p])
                continue
            rel = (n._base - base) if buf is not self.rom else 0
            if buf is self.rom:
                rom = base + offs[i]
                out.append(self._make(p, self.rom, base, offs[i], offs[i + 1] - offs[i], rom, None))
            else:
                start = (n._base if n.kind == "iset" else 0) + offs[i]
                c = container
                out.append(self._make(p, buf, 0, start, offs[i + 1] - offs[i], None, c))
                out[-1].offset = start
            del rel
        self._kids[n.path] = out
        return out

    # -------- data
    def read(self, n):
        return bytes(n._buf[n._base:n._base + n.size])

    def blob(self, n, verify=True):
        if not n.compressed:
            return self.read(n)
        key = n.path
        b = self._blobs.get(key)
        if b is None:
            b = propack.decompress(n._buf, n._base, verify)
            self._blobs[key] = b
        return b

    def walk(self, path="cart", decompress=False, maxdepth=None, _d=0):
        n = self.node(path) if isinstance(path, str) else path
        yield n
        if maxdepth is not None and _d >= maxdepth:
            return
        if n.kind == "rnc" and not decompress:
            return
        for c in self.children(n):
            for x in self.walk(c, decompress, maxdepth, _d + 1):
                yield x
        if n.kind == "rnc" and decompress:
            # drop the blob and its parsed children: walking everything must not keep 60 MB around
            self._blobs.pop(n.path, None)
            for c in list(self._kids.pop(n.path, ())):
                self._forget(c)

    def find_rom(self, addr):
        reg = [r for r in self.regions if r.start <= addr < r.end]
        if not reg:
            return None
        n = self._region_node(reg[0])
        while n.kind == "iset":
            nxt = [c for c in self.children(n) if c.rom <= addr < c.rom + c.size]
            if not nxt:
                break
            n = nxt[0]
        return n

    # -------- convenience
    _COLL = {"objects": "cart/0", "texture_sets": "cart/3", "levels": "cart/5",
             "graphicsets": "cart/16", "cinemas": "cart/18", "sound": "sound"}

    def count(self, what):
        return self.node(self._COLL[what]).count

    def object(self, i):
        return self.node("cart/0/%d" % i)

    def texture_set(self, i):
        return self.node("cart/3/%d" % i)

    def level(self, i):
        return self.node("cart/5/%d" % i)

    def graphicset(self, i):
        return self.node("cart/16/%d" % i)

    def cinema(self, i):
        """Cinema i (Cinemas index entry i+1, as Cinema_Load does; entry 0 is the table)."""
        return self.node("cart/18/%d" % (i + 1))

    def level_name(self, i):
        return self.read(self.node("cart/5/%d/8" % i)).split(b"\0")[0].decode("latin1")

    def audio_ptr(self, vaddr):
        """ROM offset of a pointer inside the audio control block (relocated to 0x480000)."""
        return vaddr - AUDIO_CTL_VRAM + AUDIO_CTL


# ---------------------------------------------------------------- CLI
def _leaves(fs):
    """ROM-level leaves of every region (nodes whose bytes are not split further)."""
    for r in fs.regions:
        for n in fs.walk(r.name, decompress=False):
            if n.kind != "iset":
                yield n


def coverage(fs):
    by_type = {}
    hdr = 0
    for r in fs.regions:
        for n in fs.walk(r.name):
            if n.kind == "iset":
                hdr += n._offs[0]
                pad = n.size - n._offs[-1]
                hdr += pad
                kids_end = n._offs[-1]
                del kids_end
            else:
                t = n.type
                if "[" in t:
                    t = t.split("[")[0] + "[*]"
                e = by_type.setdefault(t, [0, 0, 0, 0])
                e[0] += 1
                e[1] += n.size
                e[2] += 1 if n.compressed else 0
                e[3] += n.unpacked_size if n.compressed else n.size
    return by_type, hdr


def main(argv=None):
    import argparse
    ap = argparse.ArgumentParser(description="Turok 2 ROM filesystem walker")
    ap.add_argument("rom")
    ap.add_argument("--out", required=True)
    ap.add_argument("--depth", type=int, default=None)
    ap.add_argument("--decompress", action="store_true", help="also walk inside RNC blobs (slow)")
    ap.add_argument("--extract", action="store_true", help="write stored + decompressed ROM leaves")
    ap.add_argument("--coverage", action="store_true")
    a = ap.parse_args(argv)
    fs = RomFS(a.rom)
    os.makedirs(a.out, exist_ok=True)
    rows = []
    with open(os.path.join(a.out, "romfs_tree.tsv"), "w") as f:
        f.write("path\ttype\tkind\trom\tsize\tcompressed\tmethod\tunpacked\tcount\theuristic\n")
        for r in fs.regions:
            for n in fs.walk(r.name, a.decompress, a.depth):
                row = [n.path, n.type, n.kind,
                       ("0x%X" % n.rom) if n.rom is not None else "in:0x%X+0x%X" % (n.container.rom, n.offset),
                       "0x%X" % n.size, "1" if n.compressed else "0", str(n.method or ""),
                       ("0x%X" % n.unpacked_size) if n.compressed else "",
                       "" if n.count is None else str(n.count), "1" if n.heuristic else ""]
                f.write("\t".join(row) + "\n")
                rows.append(row)
                if a.extract and n.kind != "iset" and n.rom is not None:
                    p = os.path.join(a.out, "blobs", n.path + ".bin")
                    os.makedirs(os.path.dirname(p), exist_ok=True)
                    open(p, "wb").write(fs.read(n))
                    if n.compressed:
                        open(p[:-4] + ".unp.bin", "wb").write(fs.blob(n))
    with open(os.path.join(a.out, "romfs_tree.json"), "w") as f:
        json.dump([dict(zip(["path", "type", "kind", "rom", "size", "compressed", "method",
                             "unpacked", "count", "heuristic"], r)) for r in rows], f, indent=0)
    print("%d nodes -> %s" % (len(rows), a.out))
    if a.coverage:
        by_type, hdr = coverage(fs)
        total = ASSET_END - ASSET_START
        acc = hdr + sum(v[1] for v in by_type.values())
        with open(os.path.join(a.out, "romfs_types.tsv"), "w") as f:
            f.write("type\tcount\tbytes\tcompressed\tunpacked_bytes\n")
            for t, (c, b, k, u) in sorted(by_type.items(), key=lambda kv: -kv[1][1]):
                f.write("%s\t%d\t%d\t%d\t%d\n" % (t, c, b, k, u))
                print("%-40s %6d %10d bytes %6d RNC %10d unpacked" % (t, c, b, k, u))
        print("ISet headers+padding: %d bytes" % hdr)
        print("accounted 0x%X of 0x%X bytes (0x%X-0x%X): %.4f%%" % (acc, total, ASSET_START, ASSET_END,
                                                                  100.0 * acc / total))


if __name__ == "__main__":
    main()
