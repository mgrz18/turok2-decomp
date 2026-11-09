#!/usr/bin/env python3
"""Expected field values of a few typed load units, for port/tests/t2asset_typed_test.c.

    python port/tests/t2asset_typed.py baserom.us.z64 -o typed.txt

Reads the units straight from the ROM (RNC unpacked by tools/assets/rnc.py) and parses them with
the area tools' big-endian readers (tools/assets/levels.py parse_header, models.py
parse_geometry, textures.py parse_texture_bank). Every value is written next to the offset the
engine reads it from, as a line the C test checks against T2Asset_Read's host-order bytes:

    F KEY OFF TYPE VALUE        a field read with a native load (u32/s32/u16/s16/f32, VALUE hex bits)
    B KEY OFF HEX               bytes that must stay in ROM order (byte fields, texels, TLUTs,
                                the packed model command stream)
    V KEY TAB IDX x y z flag s t c0 c1 c2 c3
                                record IDX of the transposed Vtx table at TAB, after the swap and
                                CUSet__Transpose's untranspose on the host (decimal values)

Units: every level header (level info, 165), the first 40 model geometries with vertices, the first
40 texture banks (with their first palette when they have one).
"""
import argparse
import os
import struct
import sys

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.join(ROOT, "tools", "assets"))
import levels  # noqa: E402
import mkarchive  # noqa: E402
import models  # noqa: E402
import romfs  # noqa: E402
import romid  # noqa: E402
import textures  # noqa: E402


def fbits(x):
    return struct.unpack(">I", struct.pack(">f", x))[0]


def main(argv=None):
    ap = argparse.ArgumentParser()
    ap.add_argument("rom")
    ap.add_argument("-o", "--out", required=True)
    ap.add_argument("-n", type=int, default=40, help="geometries and texture banks to take (default 40)")
    a = ap.parse_args(argv)
    with open(a.rom, "rb") as f:
        rom, sha1, known = romid.identify(f.read())
    fs = romfs.RomFS(rom)
    rows = fs.manifest()
    pick = [r for r in rows if r["type"] in ("level info", "geometry", "texture")]
    data = mkarchive.unit_data(rom, pick, 1)
    out = []
    counts = {"level info": 0, "geometry": 0, "texture": 0}
    for r in pick:
        k, t, b = r["key"], r["type"], data[r["key"]]
        if t == "level info":
            h = levels.parse_header(b)
            out.append("F %08X 0 f32 %08X" % (k, fbits(h["scale"])))
            out.append("B %08X A %s" % (k, bytes(h["light_color"]).hex() + bytes(h["ambient_color"]).hex()))
            out.append("B %08X 10 %02x%s" % (k, h["flags"], struct.pack(">3b", *h["light_dir"]).hex()))
            counts[t] += 1
        elif t == "geometry" and counts[t] < a.n:
            g = models.parse_geometry(b)
            if not any(g["vertex_lists"]):
                continue
            out.append("F %08X 0 u32 %08X" % (k, g["blocks"]))
            for i in range(g["blocks"] + 1):
                out.append("F %08X %X u32 %08X" % (k, 4 + 4 * i, models.be32(b, 4 + 4 * i)))
            vt = models.iset_block(b, 0, 0)
            for i, vl in enumerate(g["vertex_lists"]):
                tab = models.iset_block(b, vt, i)
                out.append("F %08X %X s32 %08X" % (k, tab, 16 if vl else 0))
                out.append("F %08X %X s32 %08X" % (k, tab + 4, len(vl)))
                for j in sorted({0, len(vl) // 2, len(vl) - 1} if vl else ()):
                    x, y, z, fl, s, tt, c = vl[j]
                    out.append("V %08X %X %d %d %d %d %d %d %d %d %d %d %d" % ((k, tab, j, x, y, z, fl, s, tt) + c))
            lt = models.iset_block(b, 0, 2)
            for i, parts in enumerate(g["lods"]):
                pt = models.iset_block(b, lt, i)
                for j, part in enumerate(parts):
                    po = models.iset_block(b, pt, j)
                    mo = models.iset_block(b, po, 0)
                    m = part["material"]
                    out.append("F %08X %X s32 %08X" % (k, mo, m["texture"] & 0xFFFFFFFF))
                    out.append("F %08X %X u32 %08X" % (k, mo + 8, m["flags"]))
                    out.append("B %08X %X %s" % (k, mo + 12, bytes(m["prim"]).hex() + bytes(m["env"]).hex()))
                    out.append("F %08X %X u16 %04X" % (k, mo + 20, m["scale_s"]))
                    out.append("F %08X %X u16 %04X" % (k, mo + 22, m["scale_t"]))
                    co = models.iset_block(b, po, 1)
                    out.append("B %08X %X %s" % (k, co, b[co:co + 8].hex()))
            counts[t] += 1
        elif t == "texture" and counts[t] < a.n:
            tb = textures.parse_texture_bank(b)
            blocks = textures.iset_entries(b, 0)
            out.append("F %08X 0 u32 %08X" % (k, len(blocks)))
            i0 = blocks[0][0]
            out.append("B %08X %X %s" % (k, i0, tb["info"]))
            assert 1 << b[i0 + 2] == tb["width"] and 1 << b[i0 + 3] == tb["height"]
            fb = blocks[1][0]
            out.append("F %08X %X u32 %08X" % (k, fb, len(tb["frames"])))
            fs0 = textures.iset_entries(b, fb)[0][0]
            out.append("B %08X %X %s" % (k, fs0, tb["frames"][0][:16].hex()))
            pb = blocks[2][0]
            out.append("F %08X %X u32 %08X" % (k, pb, len(tb["palettes"])))
            if tb["palettes"]:
                ps0 = textures.iset_entries(b, pb)[0][0]
                out.append("B %08X %X %s" % (k, ps0, tb["palettes"][0][:16].hex()))
            counts[t] += 1
    with open(a.out, "w") as f:
        f.write("# %s; %s\n" % (sha1, ", ".join("%d %s" % (n, t) for t, n in counts.items())))
        f.write("\n".join(out) + "\n")
    print("%s: %d checks over %s" % (a.out, len(out), ", ".join("%d %s" % (n, t) for t, n in counts.items())))
    return 0


if __name__ == "__main__":
    sys.exit(main())
