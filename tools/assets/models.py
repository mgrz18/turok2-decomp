#!/usr/bin/env python3
"""Turok 2 object models ("stat"/"simp"/"anim geometry"): locate, decode, export.

usage: python tools/assets/models.py baserom.us.z64 --out DIR [--ids 0,1,5-9] [--obj] [--gltf]
                                                              [--render] [--json] [--limit N]

Everything here is read off the C (docs/assets/MODELS.md has the format and the evidence):
  * locating   : CEngineApp__Main puts the cartridge index at ROM 0xFCBFB4; CScene__Construct reads
                 its block 0 as the "objects index"; every object is a 4-block set (block 1 is the
                 RNC-packed geometry: CLevelActor__LoadGeometry, Render_QueueModelByKey).
  * geometry   : Model_DecompressCallback* -> Model_Decompress (src/us/code/func_00209AF4.c).
  * materials  : Material_Unpack (func_0026D070.c), Render_SetupMaterial (func_00207FEC.c).
The locator part (Rom, objects()) is separate from the parser (parse_geometry()) so it can be
switched to tools/assets/romfs.py (part a1) once that lands.
"""
import argparse
import json
import math
import os
import struct
import sys
import zlib

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
try:                                   # part a1's shared decompressor (tools/assets/propack.py)
    from propack import decompress as rnc_unpack  # type: ignore
except ImportError:                    # stand-in until it is merged
    from rnc_min import unpack as rnc_unpack
try:                                   # part a1's ROM filesystem (tools/assets/romfs.py)
    import romfs  # type: ignore
except ImportError:
    romfs = None

CARTRIDGE_INDEX = 0xFCBFB4             # CEngineApp__Main: e->m_CartridgeIndex = 0xFCBFB4
CART_OBJECTS = 0                       # CScene__Construct: "objects index"
CART_TEXTURE_SETS = 3                  # "texture sets index" (material texture ids index it)


def be32(b, o):
    return struct.unpack_from(">I", b, o)[0]


def sbe32(b, o):
    return struct.unpack_from(">i", b, o)[0]


# ---------------------------------------------------------------- CISet helpers (SetN64.c)
def iset_count(b, o):
    return be32(b, o)


def iset_block(b, o, i):
    """CISet__GetBlock: base + offsets[i] (offsets follow the count)."""
    return o + be32(b, o + 4 + 4 * i)


def iset_block_size(b, o, i):
    """CISet__GetBlockAndSize: (address, offsets[i+1] - offsets[i])."""
    a = be32(b, o + 4 + 4 * i)
    return o + a, be32(b, o + 8 + 4 * i) - a


# ---------------------------------------------------------------- locator
class Rom:
    def __init__(self, path):
        self.d = open(path, "rb").read()
        self.fs = romfs.RomFS(self.d) if romfs is not None else None
        c = CARTRIDGE_INDEX
        self.cart = c
        self.objects_key = iset_block(self.d, c, CART_OBJECTS)
        self.texsets_key = iset_block(self.d, c, CART_TEXTURE_SETS)
        self.nobjects = iset_count(self.d, self.objects_key)
        # cartridge block 2, "object types": {s32 2, s32 count, u16 id[count]}; object i has type id[i]
        # (CScene__FindObjectType maps a type id back to the object index).
        t = iset_block(self.d, c, 2)
        n = be32(self.d, t + 4)
        self.type_ids = list(struct.unpack_from(">%dH" % n, self.d, t + 8))

    def object_key(self, i):
        """CScene__GetObject / CISet__GetROMAddress(objects, key, i): the ROM address of object i."""
        return iset_block(self.d, self.objects_key, i)

    def object_blocks(self, i):
        """The object's set ("object index", 0x18 bytes: count 4 + 5 offsets): [(rom, size)] * 4."""
        k = self.object_key(i)
        n = iset_count(self.d, k)
        return [iset_block_size(self.d, k, j) for j in range(n)]

    def geometry_bytes(self, i):
        if self.fs is not None:        # locate through romfs: cart/0/<i>/1 is the geometry
            node = self.fs.children(self.fs.object(i))[1]
            return self.fs.blob(node), node.rom, node.size, bool(node.compressed)
        rom, size = self.object_blocks(i)[1]
        if self.d[rom:rom + 3] == b"RNC":
            return rnc_unpack(self.d, rom), rom, size, True
        return self.d[rom:rom + size], rom, size, False


# ---------------------------------------------------------------- parser
def transpose(b, o):
    """CUSet__Transpose: {s32 width, s32 height, bytes stored column-major} -> row-major rows."""
    w, h = be32(b, o), be32(b, o + 4)
    src = b[o + 8:o + 8 + w * h]
    out = bytearray(w * h)
    for x in range(w):                 # byte plane x holds byte x of every row
        out[x::w] = src[x * h:(x + 1) * h]
    return w, h, bytes(out)


def parse_material(b, o):
    tex, unk4, flags = struct.unpack_from(">iiI", b, o)
    prim = list(b[o + 12:o + 16])
    env = list(b[o + 16:o + 20])
    ss, st = struct.unpack_from(">HH", b, o + 20)
    # ss/st are the gSPTexture scales (0.16 fixed) Render_SetupMaterial hands to Texture_Load
    # (flag 0x10: taken from the texture instead).
    return {"texture": tex, "unk4": unk4, "flags": flags, "prim": prim, "env": env,
            "scale_s": ss, "scale_t": st}


def decode_commands(b, o):
    """The packed command stream of one part, as Model_Decompress expands it.
    Returns a list of ('vtx', first, count) / ('tri', a, b, c) / ('mtx', n) / ('st', v, s, t)."""
    cmds = []
    p = o
    c = b[p]
    while c != 0:
        if c & 0x80:                   # one or two triangles, 5-bit vertex cache indices
            w = (b[p] << 8) | b[p + 1]
            p += 2
            cmds.append(("tri", (w >> 10) & 31, (w >> 5) & 31, w & 31))
            if b[p] & 0x80:
                w = (b[p] << 8) | b[p + 1]
                p += 2
                cmds.append(("tri", (w >> 10) & 31, (w >> 5) & 31, w & 31))
        else:
            k = c & 0x60
            if k == 0x20:              # gSPVertex: count vertices into cache slots first..
                w = (b[p] << 8) | b[p + 1]
                p += 2
                cnt = w & 31 or 32
                cmds.append(("vtx", (w >> 5) & 31, cnt))
            elif k == 0x40:            # gSPMatrix(segment 1 + n*64, MODELVIEW|LOAD|NOPUSH)
                cmds.append(("mtx", b[p + 1]))
                p += 2
            elif k == 0x60:            # gSPModifyVertex(v, G_MWO_POINT_ST, s<<16|t)
                s, t = struct.unpack_from(">hh", b, p + 1)
                cmds.append(("st", c & 31, s, t))
                p += 5
            else:
                raise ValueError("bad model command %#x at %#x" % (c, p))
        c = b[p]
    return cmds, p + 1 - o


def parse_geometry(g):
    """A decompressed model resource -> dict (see docs/assets/MODELS.md)."""
    top = iset_count(g, 0)
    model = {"blocks": top, "lods": [], "extra": []}
    # block 0: one vertex array per level of detail (transposed Vtx[]).
    vt = iset_block(g, 0, 0)
    vlists = []
    for i in range(iset_count(g, vt)):
        w, h, rows = transpose(g, iset_block(g, vt, i))
        if w * h == 0:                 # placeholder objects carry an empty {0, 0} array
            vlists.append([])
            continue
        if w != 16:
            raise ValueError("vertex width %d" % w)
        vl = []
        for j in range(h):
            x, y, z, flag, s, t = struct.unpack_from(">hhhHhh", rows, j * 16)
            vl.append((x, y, z, flag, s, t, tuple(rows[j * 16 + 12:j * 16 + 16])))
        vlists.append(vl)
    model["vertex_lists"] = vlists
    a, sz = iset_block_size(g, 0, 1)
    model["block1"] = g[a:a + sz]
    lt = iset_block(g, 0, 2)
    for i in range(iset_count(g, lt)):
        pt = iset_block(g, lt, i)
        parts = []
        for j in range(iset_count(g, pt)):
            po = iset_block(g, pt, j)
            mat = parse_material(g, iset_block(g, po, 0))
            cmds, n = decode_commands(g, iset_block(g, po, 1))
            parts.append({"material": mat, "commands": cmds})
        model["lods"].append(parts)
    for k in range(3, top):
        a, sz = iset_block_size(g, 0, k)
        model["extra"].append(g[a:a + sz])
    if top >= 6:
        model["skeleton"] = parse_skeleton(g, iset_block(g, 0, 3))
    return model


def parse_skeleton(g, o):
    """Block 3 of an anim geometry, as CGameObjectInstance__BuildBoneMatrices/PoseBone read it:
    a set of bones (MtxPool_Alloc(count) matrices); bone b = set {0: children (CUSet array of s32
    bone ids), 1: bone data (8 bounding-box corners, a radius, ..., flags at +0x65)}."""
    bones = []
    for b in range(iset_count(g, o)):
        node = iset_block(g, o, b)
        c = iset_block(g, node, 0)
        n = be32(g, c + 4)
        children = list(struct.unpack_from(">%di" % n, g, c + 8))
        d, dsz = iset_block_size(g, node, 1)
        box = struct.unpack_from(">24f", g, d) if dsz >= 100 else ()
        bones.append({"children": children,
                      "box": [list(box[i:i + 3]) for i in range(0, len(box), 3)],
                      "radius": struct.unpack_from(">f", g, d + 96)[0] if dsz >= 100 else None,
                      "flags": g[d + 0x65] if dsz > 0x65 else None,
                      "data_size": dsz,
                      "data_tail": g[d + 100:d + dsz].hex()})
    return bones


def build_mesh(model, lod=0):
    """Runs the commands of one LOD through a 32-entry vertex cache, like the RSP does.
    Vertices load sequentially from the LOD's vertex list (vtxCounter in Model_Decompress).
    Returns (verts, parts): verts = [(x,y,z, s,t, rgba, matrix)], parts = [(material, [(i,j,k)])]."""
    vl = model["vertex_lists"][lod] if lod < len(model["vertex_lists"]) else model["vertex_lists"][0]
    verts, parts = [], []
    counter = 0
    for part in model["lods"][lod]:
        cache = [None] * 32
        mtx = 0
        tris = []
        for cmd in part["commands"]:
            if cmd[0] == "vtx":
                _, first, cnt = cmd
                for k in range(cnt):
                    x, y, z, flag, s, t, cn = vl[counter + k]
                    verts.append([x, y, z, s, t, cn, mtx])
                    cache[first + k] = len(verts) - 1
                counter += cnt
            elif cmd[0] == "mtx":
                mtx = cmd[1]
            elif cmd[0] == "st":       # copy-on-write: the cache slot gets new texture coords
                _, v, s, t = cmd
                src = verts[cache[v]]
                verts.append([src[0], src[1], src[2], s, t, src[5], src[6]])
                cache[v] = len(verts) - 1
            else:
                _, a, b, c = cmd
                if None not in (cache[a], cache[b], cache[c]):
                    tris.append((cache[a], cache[b], cache[c]))
        parts.append((part["material"], tris))
    return verts, parts


def apply_pose(verts, bone_matrices):
    """Moves bone-local vertices of an anim geometry into model space: bone_matrices[b] is the 4x4
    row-major matrix CGameObjectInstance__PoseBone builds for bone b (N64 row vectors: v' = v * M).
    The matrices come from the skeleton (parse_skeleton) and an animation (part a5)."""
    out = []
    for v in verts:
        m = bone_matrices[v[6]]
        x, y, z = v[0], v[1], v[2]
        out.append([x * m[0][0] + y * m[1][0] + z * m[2][0] + m[3][0],
                    x * m[0][1] + y * m[1][1] + z * m[2][1] + m[3][1],
                    x * m[0][2] + y * m[1][2] + z * m[2][2] + m[3][2]] + v[3:])
    return out


# ---------------------------------------------------------------- texture preview (stand-in for part a2)
def preview_texture(rom, tex_id, frame=0, palette=0):
    """Just enough of the texture format (Texture_Load, func_00206B30.c) to check UVs: the material's
    texture id indexes the cartridge's "texture sets index" (CScene__GetTextureSet); the resource is a
    set {0: info (u8 format, ?, log2 width, log2 height, ...), 1: frames, 2: palettes (RGBA16)}.
    Formats 0 = CI8, 1/3 = CI4 (3: IA16 palette), 2 = RGBA32, 4 = RGBA16; 5 (I16?) unsupported.
    Returns (w, h, [rgba tuples]) or None. Part a2 owns the real decoder (docs/assets/TEXTURES.md)."""
    d = rom.d
    a, sz = iset_block_size(d, rom.texsets_key, tex_id)
    g = rnc_unpack(d, a) if d[a:a + 3] == b"RNC" else d[a:a + sz]
    info = g[iset_block(g, 0, 0):]
    fmt, w, h = info[0], 1 << info[2], 1 << info[3]
    fr = iset_block(g, iset_block(g, 0, 1), frame % max(1, be32(g, iset_block(g, 0, 1))))
    pals = iset_block(g, 0, 2)
    pal = []
    if be32(g, pals):
        po = iset_block(g, pals, palette % be32(g, pals))
        for k in range(256 if fmt == 0 else 16):
            c = struct.unpack_from(">H", g, po + 2 * k)[0] if po + 2 * k + 2 <= len(g) else 0
            if fmt == 3:
                pal.append((c >> 8, c >> 8, c >> 8, c & 255))
            else:
                pal.append(((c >> 11 & 31) * 255 // 31, (c >> 6 & 31) * 255 // 31, (c >> 1 & 31) * 255 // 31,
                            255 * (c & 1)))
    px = []
    if fmt == 0:
        px = [pal[g[fr + i]] for i in range(w * h)]
    elif fmt in (1, 3):
        for i in range(w * h // 2):
            b = g[fr + i]
            px += [pal[b >> 4], pal[b & 15]]
    elif fmt == 2:
        px = [tuple(g[fr + 4 * i:fr + 4 * i + 4]) for i in range(w * h)]
    elif fmt == 4:
        for i in range(w * h):
            c = struct.unpack_from(">H", g, fr + 2 * i)[0]
            px.append(((c >> 11 & 31) * 255 // 31, (c >> 6 & 31) * 255 // 31, (c >> 1 & 31) * 255 // 31, 255))
    else:
        return None
    return w, h, px


_TEX_SIZE = {}


def texture_size(rom, tex_id):
    """(width, height, format) of texture set entry tex_id, from its info block."""
    if tex_id not in _TEX_SIZE:
        d = rom.d
        a, sz = iset_block_size(d, rom.texsets_key, tex_id)
        g = rnc_unpack(d, a) if d[a:a + 3] == b"RNC" else d[a:a + sz]
        i = iset_block(g, 0, 0)
        _TEX_SIZE[tex_id] = (1 << g[i + 2], 1 << g[i + 3], g[i])
    return _TEX_SIZE[tex_id]


def attach_texture_sizes(rom, parts):
    for mat, _ in parts:
        if mat["texture"] >= 0 and "tex_size" not in mat:
            try:
                mat["tex_size"] = texture_size(rom, mat["texture"])[:2]
            except Exception:  # noqa: BLE001
                pass


# ---------------------------------------------------------------- exporters
def uv(v, mat):
    """RSP texture coordinate: texel = s (S10.5) * scale / 65536 (gSPTexture), normalised by the
    texture's 2^info[2] x 2^info[3] size (Texture_Load). mat["tex_size"] is filled by
    attach_texture_sizes(); without it 32x32 is assumed."""
    ss, st = mat["scale_s"] or 0x10000, mat["scale_t"] or 0x10000
    tw, th = mat.get("tex_size") or (32, 32)
    return v[3] * ss / 65536.0 / 32.0 / tw, v[4] * st / 65536.0 / 32.0 / th


def write_obj(path, name, verts, parts):
    mtl = os.path.splitext(os.path.basename(path))[0] + ".mtl"
    with open(path, "w") as f, open(os.path.join(os.path.dirname(path), mtl), "w") as m:
        f.write("# %s\nmtllib %s\n" % (name, mtl))
        for v in verts:
            f.write("v %d %d %d %.4f %.4f %.4f\n" % (v[0], v[1], v[2], v[5][0] / 255, v[5][1] / 255, v[5][2] / 255))
        vt = 1
        for pi, (mat, tris) in enumerate(parts):
            m.write("newmtl part%d\n# texture set %d unk4 %d flags %#x scale %d,%d\nKd %.3f %.3f %.3f\n\n" % (
                pi, mat["texture"], mat["unk4"], mat["flags"], mat["scale_s"], mat["scale_t"],
                mat["prim"][0] / 255, mat["prim"][1] / 255, mat["prim"][2] / 255))
            f.write("usemtl part%d\n" % pi)
            for t in tris:
                for i in t:
                    u_, v_ = uv(verts[i], mat)
                    f.write("vt %.5f %.5f\n" % (u_, 1 - v_))
                f.write("f %d/%d %d/%d %d/%d\n" % (t[0] + 1, vt, t[1] + 1, vt + 1, t[2] + 1, vt + 2))
                vt += 3


def write_gltf(path, name, verts, parts, skinned=False):
    """glTF 2.0: one primitive per part (de-indexed per part so each keeps its own UV scale)."""
    binbuf = bytearray()
    accessors, views, prims, materials = [], [], [], []

    def add(data, ctype, count, typ, target, mn=None, mx=None):
        while len(binbuf) % 4:
            binbuf.append(0)
        views.append({"buffer": 0, "byteOffset": len(binbuf), "byteLength": len(data), "target": target})
        binbuf.extend(data)
        acc = {"bufferView": len(views) - 1, "componentType": ctype, "count": count, "type": typ}
        if mn is not None:
            acc["min"], acc["max"] = mn, mx
        if ctype == 5121:
            acc["normalized"] = True
        accessors.append(acc)
        return len(accessors) - 1

    for pi, (mat, tris) in enumerate(parts):
        if not tris:
            continue
        idx = sorted({i for t in tris for i in t})
        remap = {i: k for k, i in enumerate(idx)}
        pos = [verts[i][:3] for i in idx]
        pb = b"".join(struct.pack("<3f", *p) for p in pos)
        mn = [min(p[k] for p in pos) for k in range(3)]
        mx = [max(p[k] for p in pos) for k in range(3)]
        a_pos = add(pb, 5126, len(idx), "VEC3", 34962, mn, mx)
        a_uv = add(b"".join(struct.pack("<2f", *uv(verts[i], mat)) for i in idx), 5126, len(idx), "VEC2", 34962)
        a_col = add(b"".join(bytes(verts[i][5]) for i in idx), 5121, len(idx), "VEC4", 34962)
        a_bone = add(b"".join(struct.pack("<H", verts[i][6]) for i in idx), 5123, len(idx), "SCALAR", 34962)
        a_idx = add(b"".join(struct.pack("<3H", *(remap[i] for i in t)) for t in tris), 5123, len(tris) * 3,
                    "SCALAR", 34963)
        materials.append({"name": "part%d_texset%d" % (pi, mat["texture"]),
                          "pbrMetallicRoughness": {"baseColorFactor": [c / 255 for c in mat["prim"]],
                                                   "metallicFactor": 0, "roughnessFactor": 1},
                          "doubleSided": True,
                          "extras": mat})
        attrs = {"POSITION": a_pos, "TEXCOORD_0": a_uv, "COLOR_0": a_col}
        if skinned:                    # bone-local positions: _BONE is the gSPMatrix (bone) index
            attrs["_BONE"] = a_bone
        prims.append({"attributes": attrs,
                      "indices": a_idx, "material": len(materials) - 1})
    binname = os.path.splitext(os.path.basename(path))[0] + ".bin"
    doc = {"asset": {"version": "2.0", "generator": "turok2 tools/assets/models.py"},
           "scene": 0, "scenes": [{"nodes": [0]}],
           "nodes": [{"mesh": 0, "name": name}],
           "meshes": [{"name": name, "primitives": prims}],
           "materials": materials, "accessors": accessors, "bufferViews": views,
           "buffers": [{"uri": binname, "byteLength": len(binbuf)}]}
    if not prims:                      # placeholder objects: an empty node (glTF forbids empty meshes)
        doc = {"asset": doc["asset"], "scene": 0, "scenes": [{"nodes": [0]}], "nodes": [{"name": name}]}
    if prims:
        with open(os.path.join(os.path.dirname(path), binname), "wb") as f:
            f.write(binbuf)
    with open(path, "w") as f:
        json.dump(doc, f)


def png(path, w, h, rgb):
    raw = b"".join(b"\x00" + bytes(rgb[y * w * 3:(y + 1) * w * 3]) for y in range(h))

    def chunk(t, d):
        return struct.pack(">I", len(d)) + t + d + struct.pack(">I", zlib.crc32(t + d) & 0xFFFFFFFF)
    with open(path, "wb") as f:
        f.write(b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 2, 0, 0, 0))
                + chunk(b"IDAT", zlib.compress(raw, 6)) + chunk(b"IEND", b""))


def render(path, verts, parts, size=256, yaw=0.6, pitch=0.35, uvcheck=False, textures=None):
    """Tiny z-buffered software rasterizer: flat Lambert shading times vertex/prim colour."""
    if not verts:
        return False
    cy, sy, cp, sp = math.cos(yaw), math.sin(yaw), math.cos(pitch), math.sin(pitch)
    P = []
    for v in verts:
        x, y, z = v[0], v[1], v[2]
        x, z = x * cy - z * sy, x * sy + z * cy
        y, z = y * cp - z * sp, y * sp + z * cp
        P.append((x, y, z))
    xs = [p[0] for p in P]
    ys = [p[1] for p in P]
    span = max(max(xs) - min(xs), max(ys) - min(ys)) or 1
    sc = (size - 16) / span
    ox = (size - (max(xs) + min(xs)) * sc) / 2
    oy = (size + (max(ys) + min(ys)) * sc) / 2
    S = [(p[0] * sc + ox, oy - p[1] * sc, p[2]) for p in P]
    zb = [1e30] * (size * size)
    img = bytearray(b"\x30\x30\x38" * (size * size))
    L = (0.4, 0.7, -0.6)
    ln = math.sqrt(sum(c * c for c in L))
    for mat, tris in parts:
        for a, b, c in tris:
            pa, pb, pc = P[a], P[b], P[c]
            ux, uy, uz = pb[0] - pa[0], pb[1] - pa[1], pb[2] - pa[2]
            vx, vy, vz = pc[0] - pa[0], pc[1] - pa[1], pc[2] - pa[2]
            nx, ny, nz = uy * vz - uz * vy, uz * vx - ux * vz, ux * vy - uy * vx
            nl = math.sqrt(nx * nx + ny * ny + nz * nz) or 1
            lam = abs(nx * L[0] + ny * L[1] + nz * L[2]) / nl / ln
            k = 0.25 + 0.75 * lam
            if mat["flags"] & 2:       # vertex colour is shade (unlit models)
                col = [sum(verts[i][5][q] for i in (a, b, c)) / 3 for q in range(3)]
            else:
                col = [200, 200, 200]
            if mat["texture"] == -1:
                col = [col[q] * mat["prim"][q] / 255 for q in range(3)]
            rgb = bytes(min(255, int(col[q] * k)) for q in range(3))
            tex = textures.get(mat["texture"]) if textures and mat["texture"] >= 0 else None
            if (uvcheck or tex) and mat["texture"] >= 0:
                ua, ub, uc = uv(verts[a], mat), uv(verts[b], mat), uv(verts[c], mat)
            else:
                ua = None
            sa, sb, scc = S[a], S[b], S[c]
            minx = max(int(min(sa[0], sb[0], scc[0])), 0)
            maxx = min(int(max(sa[0], sb[0], scc[0])) + 1, size - 1)
            miny = max(int(min(sa[1], sb[1], scc[1])), 0)
            maxy = min(int(max(sa[1], sb[1], scc[1])) + 1, size - 1)
            area = (sb[0] - sa[0]) * (scc[1] - sa[1]) - (sb[1] - sa[1]) * (scc[0] - sa[0])
            if abs(area) < 1e-9:
                continue
            for y in range(miny, maxy + 1):
                for x in range(minx, maxx + 1):
                    w0 = ((sb[0] - x) * (scc[1] - y) - (sb[1] - y) * (scc[0] - x)) / area
                    w1 = ((scc[0] - x) * (sa[1] - y) - (scc[1] - y) * (sa[0] - x)) / area
                    w2 = 1 - w0 - w1
                    if w0 < 0 or w1 < 0 or w2 < 0:
                        continue
                    z = w0 * sa[2] + w1 * sb[2] + w2 * scc[2]
                    o = y * size + x
                    if z < zb[o]:
                        zb[o] = z
                        if tex:
                            tw, th, tp = tex
                            tu = (w0 * ua[0] + w1 * ub[0] + w2 * uc[0]) * tw
                            tv = (w0 * ua[1] + w1 * ub[1] + w2 * uc[1]) * th
                            iu, iv = int(math.floor(tu)), int(math.floor(tv))
                            if mat["flags"] & 0x800:
                                iu = iu % (2 * tw) if (iu // tw) % 2 == 0 else tw - 1 - iu % tw
                                iv = iv % (2 * th) if (iv // th) % 2 == 0 else th - 1 - iv % th
                            if mat["flags"] & 0x20:
                                iu, iv = min(max(iu, 0), tw - 1), min(max(iv, 0), th - 1)
                            t = tp[(iv % th) * tw + (iu % tw)]
                            lit = k if not (mat["flags"] & 2) else k
                            img[o * 3:o * 3 + 3] = bytes(min(255, int(t[q] * lit * (col[q] / 200 if mat["flags"] & 2 else 1))) for q in range(3))
                        elif ua is not None:   # 8x8 checker per texture repeat
                            tu = w0 * ua[0] + w1 * ub[0] + w2 * uc[0]
                            tv = w0 * ua[1] + w1 * ub[1] + w2 * uc[1]
                            on = (int(math.floor(tu * 8)) + int(math.floor(tv * 8))) & 1
                            f = (0.55 + 0.45 * on) * k
                            img[o * 3:o * 3 + 3] = bytes((int(250 * f), int((120 + 120 * on) * f), int(90 * f)))
                        else:
                            img[o * 3:o * 3 + 3] = rgb
    png(path, size, size, img)
    return True


# ---------------------------------------------------------------- CLI
def summary(rom, i, model, info):
    rom_addr, size, packed = info
    out = {"id": i, "type_id": rom.type_ids[i] if i < len(rom.type_ids) else None,
           "key": "%#x" % rom.object_key(i), "geometry_rom": "%#x" % rom_addr,
           "geometry_size": size, "packed": packed, "blocks": model["blocks"],
           "kind": "anim" if model["blocks"] == 6 else "simple",
           "lods": [], "vertex_lists": [len(v) for v in model["vertex_lists"]],
           "block1_size": len(model["block1"]), "extra_sizes": [len(e) for e in model["extra"]]}
    for parts in model["lods"]:
        out["lods"].append([{"material": p["material"],
                             "triangles": sum(1 for c in p["commands"] if c[0] == "tri"),
                             "matrices": sorted({c[1] for c in p["commands"] if c[0] == "mtx"})}
                            for p in parts])
    if "skeleton" in model:
        out["bones"] = len(model["skeleton"])
        out["skeleton"] = [b["children"] for b in model["skeleton"]]
    blocks = rom.object_blocks(i)
    out["object_blocks"] = [{"rom": "%#x" % a, "size": s} for a, s in blocks]
    return out


def parse_ids(s, n):
    if not s:
        return list(range(n))
    ids = []
    for tok in s.split(","):
        if "-" in tok:
            a, b = tok.split("-")
            ids += range(int(a, 0), int(b, 0) + 1)
        else:
            ids.append(int(tok, 0))
    return ids


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("rom")
    ap.add_argument("--out", required=True)
    ap.add_argument("--ids", help="object ids, e.g. 0,5,10-20 (default: all)")
    ap.add_argument("--limit", type=int)
    ap.add_argument("--obj", action="store_true")
    ap.add_argument("--gltf", action="store_true")
    ap.add_argument("--render", action="store_true")
    ap.add_argument("--uvcheck", action="store_true", help="render textured parts with a UV checker")
    ap.add_argument("--textured", action="store_true", help="render with a preview decode of the textures")
    ap.add_argument("--json", action="store_true", help="write index.json with every model's summary")
    ap.add_argument("--lod", type=int, default=0)
    a = ap.parse_args()
    rom = Rom(a.rom)
    os.makedirs(a.out, exist_ok=True)
    ids = parse_ids(a.ids, rom.nobjects)[:a.limit]
    index, errors = [], 0
    for i in ids:
        try:
            g, ra, sz, packed = rom.geometry_bytes(i)
            model = parse_geometry(g)
        except Exception as e:  # noqa: BLE001
            errors += 1
            index.append({"id": i, "error": str(e)})
            continue
        index.append(summary(rom, i, model, (ra, sz, packed)))
        if a.obj or a.gltf or a.render:
            lod = min(a.lod, len(model["lods"]) - 1)
            verts, parts = build_mesh(model, lod)
            attach_texture_sizes(rom, parts)
            name = "obj%04d" % i
            if a.obj:
                write_obj(os.path.join(a.out, name + ".obj"), name, verts, parts)
            if a.gltf:
                write_gltf(os.path.join(a.out, name + ".gltf"), name, verts, parts, "skeleton" in model)
            if a.render:
                texs = {}
                if a.textured:
                    for mat, _ in parts:
                        if mat["texture"] >= 0 and mat["texture"] not in texs:
                            try:
                                texs[mat["texture"]] = preview_texture(rom, mat["texture"])
                            except Exception:  # noqa: BLE001
                                texs[mat["texture"]] = None
                render(os.path.join(a.out, name + ".png"), verts, parts, uvcheck=a.uvcheck, textures=texs)
    if a.json:
        with open(os.path.join(a.out, "index.json"), "w") as f:
            json.dump(index, f, indent=1)
    print("%d models, %d errors" % (len(ids) - errors, errors))


if __name__ == "__main__":
    main()
