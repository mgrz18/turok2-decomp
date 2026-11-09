#!/usr/bin/env python3
"""Turok 2 (N64) level and world data: decode the cartridge's level set to JSON, draw maps.

    python tools/assets/levels.py baserom.us.z64 --out DIR [--level N ...] [--map N ...]

Writes DIR/levels/NNN_<name>.json per level (header, sky layers, regions = collision floor mesh,
region infos, grid bounds, grid sections = streamed static objects / level actors / light volumes,
animated instances, paths, links), DIR/levels/index.json, DIR/warp_dests.json and, with --map,
DIR/maps/NNN_<name>.png (top-down region map with the placed objects).

The C is the parser (src/us/code): CScene__Construct (func_00429FA0.c) opens the cartridge index,
CScene__LoadLevel (func_0042AC0C.c) the nine blocks of a level, CScene__DecompressCollision,
CScene__DecompressGridSection, CScene__DecompressAnimatedInstances, CScene__DecompressPaths,
CScene__DecompressLinks, CScene__FindWarpDests.  docs/assets/LEVELS.md describes the formats.

Locating goes through tools/assets/romfs.py (fs.level(i), fs.cinema(i)) and RNC unpacking through
rnc.py; the parsers only see bytes.
"""
import argparse
import json
import math
import os
import re
import struct
import sys
import zlib

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import rnc  # noqa: E402
import romfs  # noqa: E402

# ---------------------------------------------------------------------------------------------
# Sets: the engine's "indexed set" (CISet) is  u32 count, (count+1) u32 offsets relative to the
# set start, the last offset being the end; block i = [off[i], off[i+1]).  ROM-level locating is
# romfs.RomFS; set_offsets/blocks below only split sets inside bytes already in hand.
# ---------------------------------------------------------------------------------------------
CARTRIDGE_INDEX = romfs.CARTRIDGE
CART = romfs.CART_NAMES   # cartridge index entries (CScene__Construct)


def u32(d, o):
    return struct.unpack_from(">I", d, o)[0]


def set_offsets(d, base=0):
    """CISet__GetROMAddressAndSize over the whole set: [(start, size)] relative to d."""
    n = u32(d, base)
    offs = [u32(d, base + 4 + 4 * i) for i in range(n + 1)]
    return [(base + offs[i], offs[i + 1] - offs[i]) for i in range(n)]


def blocks(d, base=0):
    """CISet__GetBlockAndSize for every block of the set at d[base:]."""
    return [d[a:a + s] for a, s in set_offsets(d, base)]


def rom_set(rom, key):
    """[(rom_offset, size)] of the set whose ROM key is `key` (rom = ROM bytes)."""
    return set_offsets(rom, key)


def unpack(blob):
    """ProPackDecompress semantics: RNC chunks are unpacked, raw blocks pass through."""
    return rnc.maybe_decompress(blob)


def _fs(rom):
    return rom if isinstance(rom, romfs.RomFS) else romfs.RomFS(rom)


# ---------------------------------------------------------------------------------------------
# Common encodings
# ---------------------------------------------------------------------------------------------
def untranspose(b):
    """Inverse of the packing undone by CUSet__Transpose (func_0020192C.c): a table
    {s32 width, s32 count} whose count*width bytes are stored byte-plane by byte-plane
    (byte k of every record, then byte k+1 ...).  Returns (width, count, records_bytes)."""
    w, c = struct.unpack_from(">ii", b, 0)
    data = b[8:8 + w * c]
    out = bytearray(w * c)
    for x in range(w):
        out[x::w] = data[x * c:(x + 1) * c]
    return w, c, bytes(out)


def table(b):
    """A plain {s32 width, s32 count, records} table (no transposition)."""
    w, c = struct.unpack_from(">ii", b, 0)
    return w, c, b[8:8 + w * c]


def recs(w, c, data):
    return [data[i * w:(i + 1) * w] for i in range(c)]


def f(x):
    return round(x, 4)


def cstr(b):
    return b.split(b"\0", 1)[0].decode("latin-1")


# ---------------------------------------------------------------------------------------------
# Level blocks (CScene__LoadLevel: block index -> description string, callback)
# ---------------------------------------------------------------------------------------------
LEVEL_BLOCKS = {
    0: ("level info", "raw: CLevelHeader"),
    1: ("collision", "CScene__DecompressCollision"),
    2: ("level paths", "CScene__DecompressPaths"),
    3: ("animated instances", "CScene__DecompressAnimatedInstances"),
    4: ("grid bounds", "ProPack__Decompress"),
    5: ("grid sections index", "nested set, each cell via CScene__DecompressGridSection"),
    6: ("sky layers", "ProPack__Decompress: CEmitterDef table"),
    7: ("links", "CScene__DecompressLinks"),
    8: ("name", "raw ASCII, 16 bytes or less (not loaded by LoadLevel)"),
}

VERT_SCALE = 0.029296875  # UnpackScaledVec4 (func_0026BAE4.c)


def parse_header(b):
    """CLevelHeader (structs/world.h), block 0 (0x18 bytes on the cart)."""
    scale, = struct.unpack_from(">f", b, 0)
    return {
        "scale": f(scale),
        "unk_04": b[4:10].hex(),
        "light_color": list(b[0x0A:0x0D]),
        "ambient_color": list(b[0x0D:0x10]),
        "flags": b[0x10],
        "fog": bool((b[0x10] >> 2) & 1),
        "light_dir": list(struct.unpack_from(">3b", b, 0x11)),
        "tail": b[0x14:].hex(),
    }


def parse_sky_layers(b):
    """Block 6 "sky layers": {0x2C, n} + n CEmitterDef (structs/emitter.h), not transposed.
    Region infos enable layer k with bit k of CRegionInfo.m_EmitterMask (CEmitterPool__Advance)."""
    w, c, data = table(b)
    out = []
    for i, r in enumerate(recs(w, c, data)):
        tex, = struct.unpack_from(">h", r, 0)
        fl = struct.unpack_from(">7f", r, 8)
        out.append({
            "index": i, "texture": tex, "color": list(r[2:5]), "env_color": list(r[5:8]),
            "spin_rate": f(fl[0]), "texture_scale": f(fl[1]), "phase_rate": f(fl[2]),
            "base_height": f(fl[3]), "fade_depth": f(fl[4]), "camera_scale": f(fl[5]),
            "throttle_rate": f(fl[6]), "alpha": r[0x28], "flags": r[0x29],
        })
    return out


def parse_region_info(r):
    """CRegionInfo (structs/region.h), 0x64 bytes; unnamed bytes kept as hex."""
    return {
        "unk_00": r[0:0x20].hex(),
        "water_height": f(struct.unpack_from(">f", r, 0x20)[0]),
        "damage_interval": f(struct.unpack_from(">f", r, 0x28)[0]),
        "unk_2C": f(struct.unpack_from(">f", r, 0x2C)[0]),
        "pos": [f(v) for v in struct.unpack_from(">3f", r, 0x34)],
        "flags": struct.unpack_from(">I", r, 0x44)[0],
        "damage_amount": struct.unpack_from(">H", r, 0x50)[0],
        "flags52": struct.unpack_from(">H", r, 0x52)[0],
        "flags54": struct.unpack_from(">H", r, 0x54)[0],
        "emitter_mask": struct.unpack_from(">H", r, 0x56)[0],
        "map_colour": r[0x5C],
        "rest": (r[0x24:0x28] + r[0x30:0x34] + r[0x40:0x44] + r[0x48:0x50] + r[0x58:0x5C] + r[0x5D:]).hex(),
    }


def parse_collision(b):
    """Block 1 "collision" after RNC: a set of 4 transposed tables (CScene__DecompressCollision):
       0 CRegionInfo[n] (0x64)   1 vertices (0x10: s32 x, y, z, w; * 0.029296875)
       2 packed regions (0x14: u16 info, u16 flags, u16 v[3], u16 neighbour[3] (0xFFFF none), s32 vis)
       3 trailing table (0x14 records; copied as is)."""
    bl = blocks(b)
    w0, c0, d0 = untranspose(bl[0])
    w1, c1, d1 = untranspose(bl[1])
    w2, c2, d2 = untranspose(bl[2])
    w3, c3, d3 = untranspose(bl[3])
    verts = [[f(v * VERT_SCALE) for v in struct.unpack_from(">4i", r, 0)] for r in recs(w1, c1, d1)]
    regions = []
    for r in recs(w2, c2, d2):
        info, flags, a, b_, c, n0, n1, n2, vis = struct.unpack_from(">8HI", r, 0)
        regions.append({"info": info, "flags": flags, "v": [a, b_, c],
                        "neighbours": [None if n == 0xFFFF else n for n in (n0, n1, n2)],
                        "vis": vis})
    return {
        "region_infos": [parse_region_info(r) for r in recs(w0, c0, d0)],
        "vertices": verts,
        "regions": regions,
        "table3": {"width": w3, "records": [r.hex() for r in recs(w3, c3, d3)]},
    }


def parse_grid_bounds(b):
    """Block 4 "grid bounds" (CScene__LoadVisibleGridSections): set of 5 {?, n, f32[n]} lists
    (0 cell max x, 1 cell min x, 2 cell max z, 3 cell min z) and 4 = {0x14, nx*nz} cells
    {CRect minX, minZ, maxX, maxZ; s32 empty}.  Cell (x, z) = grid section x + z*nx."""
    bl = blocks(b)
    lists = []
    for k in range(4):
        _, n = struct.unpack_from(">ii", bl[k], 0)
        lists.append([f(v) for v in struct.unpack_from(">%df" % n, bl[k], 8)])
    w, c, data = table(bl[4])
    cells = []
    for r in recs(w, c, data):
        x0, z0, x1, z1 = struct.unpack_from(">4f", r, 0)
        cells.append({"rect": [f(x0), f(z0), f(x1), f(z1)], "empty": struct.unpack_from(">i", r, 16)[0]})
    return {"nx": len(lists[0]), "nz": len(lists[2]), "x_max": lists[0], "x_min": lists[1],
            "z_max": lists[2], "z_min": lists[3], "cells": cells}


def parse_grid_section(b, scale):
    """One grid section (CScene__DecompressGridSection), after RNC: a set of 4 transposed tables
    (or a lone 0 word for an empty cell):
       0 compact static objects 0x1C (CStaticObject__ConstructCompact)
       1 static objects 0x34        (CStaticObject__Construct)
       2 level actors 0x20          (CLevelActor__Construct)
       3 light volumes 0x48         (LightVolume_Construct)"""
    if len(b) < 8 or u32(b, 0) == 0:
        return None
    bl = blocks(b)
    out = {"compact_objects": [], "static_objects": [], "level_actors": [], "light_volumes": []}
    w, c, d = untranspose(bl[0])
    for r in recs(w, c, d):
        value, sc, oid, seg, flags, model, y = struct.unpack_from(">ifHHHHh", r, 0)
        ext = list(struct.unpack_from(">6b", r, 0x12))
        bits, x, z = r[0x18], r[0x19], r[0x1A]
        px = x * scale * (-1 if bits & 0x10 else 1)
        pz = z * scale * (-1 if bits & 0x20 else 1)
        out["compact_objects"].append({
            "object": oid, "intelligence": model, "region": None if seg == 0xFFFF else seg,
            "flags": flags, "vis": value, "scale": f(sc),
            "pos": [f(px), f(y * 5.12), f(pz)], "rot_y": (bits & 3) * 90,
            "extents": [e * 10.24 for e in ext], "raw_bits": bits})
    w, c, d = untranspose(bl[1])
    for r in recs(w, c, d):
        value, = struct.unpack_from(">i", r, 0)
        pos = struct.unpack_from(">3f", r, 4)
        sc = struct.unpack_from(">3f", r, 0x10)
        ext = struct.unpack_from(">6H", r, 0x1C)
        oid, seg, model = struct.unpack_from(">3H", r, 0x28)
        q = [v * 0.007874016 for v in struct.unpack_from(">4b", r, 0x2F)]
        out["static_objects"].append({
            "object": oid, "intelligence": model, "region": None if seg == 0xFFFF else seg,
            "flags": r[0x2E], "vis": value, "pos": [f(v) for v in pos], "scale": [f(v) for v in sc],
            "quat": [f(v) for v in q], "extents_half": list(ext)})
    w, c, d = untranspose(bl[2])
    for r in recs(w, c, d):
        value, x, y, z, rad, oid, seg, flags, killed, model = struct.unpack_from(">i4f5H", r, 0)
        ang, = struct.unpack_from(">b", r, 0x1E)
        out["level_actors"].append({
            "object": oid, "intelligence": None if model == 0xFFFF else model,
            "region": None if seg == 0xFFFF else seg, "flags": flags, "killed_index": killed,
            "vis": value, "pos": [f(x), f(y), f(z)], "radius": f(rad),
            "rot_y": f(ang * 0.02473695)})
    w, c, d = untranspose(bl[3])
    for r in recs(w, c, d):  # LightVolume_Construct (func_00211F64.c), 0x48-byte records
        v = struct.unpack_from(">11i", r, 0)
        u3c = struct.unpack_from(">3I", r, 0x3C)
        out["light_volumes"].append({
            "pos": [f(x * 2.384185791015625e-05) for x in v[0:3]],
            "rot": [f(x * 9.313225746154785e-09) for x in v[3:6]],  # radians (mode 1: an aim vector)
            "scale": [f(x * 2.384185791015625e-05) for x in v[6:9]],
            "width": f(2.0 * v[9] * 4.656612873077393e-10),
            "height": f(2.0 * v[10] * 4.656612873077393e-10),
            "mode": r[0x2F], "unk37": r[0x37], "unk3B": r[0x3B],
            "unk3C": [f(x * 2.3748725652694702e-06) for x in u3c],
            "raw_2C": r[0x2C:0x3C].hex()})
    return out


def parse_instances(b):
    """Block 3 "animated instances" after RNC: one transposed table of 0x28-byte records
    (CScene__DecompressAnimatedInstances -> CGameObjectInstance__ConstructFromRecord):
       0x00 s32 unused, 0x04 f32 pos[3], 0x10 f32 scale[3], 0x1C u16 object (model),
       0x1E u16 type, 0x20 u16 region (0xFFFF none), 0x22 u16 intelligence,
       0x24 s16 rot_y (* pi/32767), 0x26 u8 flags, 0x27 u8 sound (0xFF none).
    Record 0 and every record whose intelligence is of type 0xB are player starts."""
    w, c, d = untranspose(b)
    out = []
    for r in recs(w, c, d):
        unk0, x, y, z, sx, sy, sz, model, typ, reg, intel, rot = struct.unpack_from(">i6f4Hh", r, 0)
        out.append({"object": model, "type": typ, "region": None if reg == 0xFFFF else reg,
                    "intelligence": intel, "pos": [f(x), f(y), f(z)], "scale": [f(sx), f(sy), f(sz)],
                    "rot_y": f(rot * 9.587673e-5), "flags": r[0x26],
                    "sound": None if r[0x27] == 0xFF else r[0x27], "unk0": unk0})
    return out


def parse_paths(b):
    """Block 2 "level paths" after RNC (CScene__DecompressPaths): a set of groups, each group a
    set whose block 1 is {?, n} + n 0x1C items (0x0C s32 region index or -1, 0x10 f32[3], the first
    an angle wrapped by Angle_Wrap); other blocks kept raw."""
    if len(b) < 4 or u32(b, 0) == 0:
        return []
    groups = []
    for g in blocks(b):
        gb = blocks(g)
        items = []
        if len(gb) > 1:
            _, n = struct.unpack_from(">ii", gb[1], 0)
            for i in range(n):
                r = gb[1][8 + i * 0x1C: 8 + (i + 1) * 0x1C]
                p = struct.unpack_from(">3f", r, 0)
                ref, = struct.unpack_from(">i", r, 0xC)
                a = struct.unpack_from(">3f", r, 0x10)
                items.append({"pos": [f(v) for v in p], "region": None if ref == -1 else ref,
                              "data": [f(v) for v in a]})
        groups.append({"items": items, "other_blocks": [x.hex() for k, x in enumerate(gb) if k != 1]})
    return groups


def parse_event(r):
    """LevelEvent (structs/event.h), 0x14 bytes: what LevelEvent_Fire fires."""
    head, actor, sound, music, level, delay = struct.unpack_from(">IHhHHH", r, 0)
    return {"head": head, "actor": actor, "sound": sound, "music": music, "level": level,
            "delay_half": delay, "flags": r[0x0E], "key": r[0x0F], "message": r[0x10],
            "kind": r[0x11], "handler": r[0x12], "group": r[0x13]}


def parse_links(b):
    """Block 7 "links" after RNC (CScene__DecompressLinks): the level's event records, two lists
    (CScene.m_pRecords / m_pAltRecords: cinema cues pick one with their 'table' word), each as a
    pair of plain {width, count} tables: an s32 per record (sorted keys), then the LevelEvent
    records (0x14).  Byte 0x13 is the record's link group (bit tested in the persistent data)."""
    bl = blocks(b)
    out = {}
    for name, k in (("records", 0), ("alt_records", 2)):
        if len(bl) < k + 2:
            continue
        _, nk, dk = table(bl[k])
        keys = list(struct.unpack_from(">%di" % nk, dk, 0))
        w, c, d = table(bl[k + 1])
        out[name] = [dict(parse_event(r), id=keys[i] if i < len(keys) else None)
                     for i, r in enumerate(recs(w, c, d))]
    return out


def level_name(rom, rom_off, size):
    return cstr(rom[rom_off:rom_off + size])


def parse_level(fs, idx, with_sections=True):
    """Level idx (cart/5/<idx>, CScene__LoadLevel's nine blocks); fs = romfs.RomFS or ROM bytes."""
    fs = _fs(fs)
    rom = fs.rom
    node = fs.level(idx)
    lkey, lsize = node.rom, node.size
    kids = fs.children(node)
    sub = [(k.rom, k.size) for k in kids]
    raw = [fs.read(k) for k in kids]
    out = {"index": idx, "rom": hex(lkey), "size": lsize,
           "blocks": [{"n": k, "name": LEVEL_BLOCKS.get(k, ("?",))[0], "rom": hex(a), "size": s,
                       "compressed": rom[a:a + 3] == b"RNC"} for k, (a, s) in enumerate(sub)]}
    out["name"] = cstr(raw[8]) if len(raw) > 8 else ""
    hdr = parse_header(raw[0])
    out["header"] = hdr
    out["sky_layers"] = parse_sky_layers(unpack(raw[6]))
    out["collision"] = parse_collision(unpack(raw[1]))
    out["grid"] = parse_grid_bounds(unpack(raw[4]))
    out["instances"] = parse_instances(unpack(raw[3]))
    out["paths"] = parse_paths(unpack(raw[2]))
    out["links"] = parse_links(unpack(raw[7]))
    out["grid_sections"] = []
    for k, gn in enumerate(fs.children(kids[5])):
        a, s = gn.rom, gn.size
        ent = {"cell": k, "rom": hex(a), "size": s, "compressed": rom[a:a + 3] == b"RNC"}
        if with_sections and s:
            ent["data"] = parse_grid_section(fs.blob(gn), hdr["scale"])
        out["grid_sections"].append(ent)
    return out


def parse_warp_dests(fs):
    """Cartridge entry 8 "warp dests" (CScene__FindWarpDests): set of 2 tables:
    0 = {?, n, s32 key[n]} sorted warp ids (binary searched), 1 = {?, n, CWarpPoint[n]}
    (f32 pos[3], f32 rot_y, s16 level, s16 id)."""
    fs = _fs(fs)
    bl = blocks(fs.read(fs.node("cart/8")))
    _, n = struct.unpack_from(">ii", bl[0], 0)
    keys = struct.unpack_from(">%di" % n, bl[0], 8)
    out = []
    for i in range(n):
        x, y, z, ry, lvl, wid = struct.unpack_from(">4fhh", bl[1], 8 + i * 0x14)
        out.append({"warp_id": keys[i], "level": lvl, "id": wid, "pos": [f(x), f(y), f(z)], "rot_y": f(ry)})
    return out


# ---------------------------------------------------------------------------------------------
# Top-down map (verification): regions filled by height, edges darker, objects as dots
# ---------------------------------------------------------------------------------------------
def write_png(path, w, h, rgb):
    raw = b"".join(b"\0" + bytes(rgb[y * w * 3:(y + 1) * w * 3]) for y in range(h))

    def chunk(t, d):
        c = struct.pack(">I", len(d)) + t + d
        return c + struct.pack(">I", zlib.crc32(t + d) & 0xFFFFFFFF)
    with open(path, "wb") as fp:
        fp.write(b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 2, 0, 0, 0))
                 + chunk(b"IDAT", zlib.compress(raw, 9)) + chunk(b"IEND", b""))


def render_map(level, path, size=1024):
    col = level["collision"]
    vs = col["vertices"]
    if not vs:
        return False
    xs = [v[0] for v in vs]
    zs = [v[2] for v in vs]
    ys = [v[1] for v in vs]
    x0, x1, z0, z1 = min(xs), max(xs), min(zs), max(zs)
    y0, y1 = min(ys), max(ys)
    span = max(x1 - x0, z1 - z0, 1.0)
    k = (size - 16) / span
    w = int((x1 - x0) * k) + 16
    h = int((z1 - z0) * k) + 16
    img = bytearray(b"\x18\x18\x20" * (w * h))

    def P(x, z):
        return (x - x0) * k + 8, (z - z0) * k + 8

    def put(x, y, c):
        if 0 <= x < w and 0 <= y < h:
            img[(y * w + x) * 3:(y * w + x) * 3 + 3] = bytes(c)

    def line(a, b, c):
        (ax, ay), (bx, by) = a, b
        n = int(max(abs(bx - ax), abs(by - ay))) + 1
        for i in range(n + 1):
            t = i / n
            put(int(ax + (bx - ax) * t), int(ay + (by - ay) * t), c)

    infos = col["region_infos"]
    for r in col["regions"]:
        pts = [vs[i] for i in r["v"]]
        t = (sum(p[1] for p in pts) / 3 - y0) / max(y1 - y0, 1.0)
        water = r["info"] < len(infos) and infos[r["info"]]["flags"] & 1
        base = (40 + int(150 * t), 70 + int(160 * t), 50 + int(60 * t))
        if water:
            base = (40, 90 + int(100 * t), 200)
        if r["flags"] & 0x40:  # REGION_FLAG_CEILING
            base = (base[0] // 2 + 60, base[1] // 2, base[2] // 2)
        q = [P(p[0], p[2]) for p in pts]
        ymin = int(min(p[1] for p in q))
        ymax = int(max(p[1] for p in q)) + 1
        for yy in range(ymin, ymax + 1):
            xsx = []
            for i in range(3):
                (ax, ay), (bx, by) = q[i], q[(i + 1) % 3]
                if (ay <= yy + 0.5 < by) or (by <= yy + 0.5 < ay):
                    xsx.append(ax + (yy + 0.5 - ay) * (bx - ax) / (by - ay))
            if len(xsx) >= 2:
                for xx in range(int(min(xsx) + 0.5), int(max(xsx) + 0.5)):
                    put(xx, yy, base)
        for i in range(3):
            if r["neighbours"][i] is None:
                line(q[i], q[(i + 1) % 3], (230, 230, 230))

    def dot(x, z, c, rad=2):
        px, py = P(x, z)
        for dy in range(-rad, rad + 1):
            for dx in range(-rad, rad + 1):
                put(int(px) + dx, int(py) + dy, c)
    for g in level["grid_sections"]:
        d = g.get("data")
        if not d:
            continue
        for o in d["static_objects"] + d["compact_objects"]:
            dot(o["pos"][0], o["pos"][2], (255, 200, 40), 1)
        for o in d["level_actors"]:
            dot(o["pos"][0], o["pos"][2], (60, 200, 255), 1)
    for i, o in enumerate(level["instances"]):
        dot(o["pos"][0], o["pos"][2], (255, 60, 60) if i else (255, 255, 255), 2 if i else 4)
    write_png(path, w, h, img)
    return True


# ---------------------------------------------------------------------------------------------
def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("rom")
    ap.add_argument("--out", required=True)
    ap.add_argument("--level", type=int, action="append", help="only these levels (repeatable)")
    ap.add_argument("--map", type=int, action="append", help="draw maps of these levels ('-1' = all)")
    ap.add_argument("--no-sections", action="store_true", help="skip the grid section contents")
    a = ap.parse_args(argv)
    fs = romfs.RomFS(a.rom)
    nlev = fs.count("levels")
    os.makedirs(os.path.join(a.out, "levels"), exist_ok=True)
    which = a.level if a.level else list(range(nlev))
    maps = set(a.map or [])
    if -1 in maps:
        maps = set(which)
    index = []
    for i in which:
        lv = parse_level(fs, i, not a.no_sections)
        slug = re.sub(r"[^A-Za-z0-9]+", "_", lv["name"]).strip("_") or "unnamed"
        fn = "%03d_%s" % (i, slug)
        with open(os.path.join(a.out, "levels", fn + ".json"), "w") as fp:
            json.dump(lv, fp, indent=1)
        secs = [g["data"] for g in lv["grid_sections"] if g.get("data")]
        index.append({"index": i, "name": lv["name"], "rom": lv["rom"], "file": fn + ".json",
                      "regions": len(lv["collision"]["regions"]), "vertices": len(lv["collision"]["vertices"]),
                      "instances": len(lv["instances"]), "grid": [lv["grid"]["nx"], lv["grid"]["nz"]],
                      "static_objects": sum(len(s["static_objects"]) + len(s["compact_objects"]) for s in secs),
                      "level_actors": sum(len(s["level_actors"]) for s in secs),
                      "light_volumes": sum(len(s["light_volumes"]) for s in secs)})
        if i in maps:
            os.makedirs(os.path.join(a.out, "maps"), exist_ok=True)
            render_map(lv, os.path.join(a.out, "maps", fn + ".png"))
    with open(os.path.join(a.out, "levels", "index.json"), "w") as fp:
        json.dump(index, fp, indent=1)
    with open(os.path.join(a.out, "warp_dests.json"), "w") as fp:
        json.dump(parse_warp_dests(fs), fp, indent=1)
    print("%d levels -> %s" % (len(index), a.out))


if __name__ == "__main__":
    main()
