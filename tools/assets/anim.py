#!/usr/bin/env python3
"""Turok 2 animated objects: skeletons and animations -> JSON (+ glTF, PNG previews).

The engine code this follows (src/us/code; names from docs/NAMES.md):

  CScene__Construct          cartridge index (ROM 0xFCBFB4, CEngineApp__Main) entry 0 = "objects index"
  CGameObjectInstance__SetupAnimation
                             an object is a 4-entry set: [0] "anim object info", [1] "anim geometry",
                             [2] the animations set ("animations index"), [3] 8 bytes (unused here)
  CGameObjectInstance__PickRandomAnim
                             anim object info block 1 = sorted action id per animation
  CGameObjectInstance__PoseBone / __BuildBoneMatrices
                             geometry block 3 = skeleton: one set per bone {children, bone data}
  CAnimationState__Load / __FindKeys / __GetRootMotion / __GetRootYaw / __GetEvents
                             an animation = RNC-packed 6-chunk set
  Anim_Decompress / Anim_DecodeAnimation / Anim_DecodeTracks
  Anim_DecodeCurveChannel / AnimCurve_Init / AnimCurve_LoadSegment / AnimCurve_Evaluate
  Anim_DecodeQuantizedChannel / Anim_DecodeRawChannel / Bits_Read / Bits_ReadFloat
                             the compressed tracks (chunk 5) -> per-frame float keys

See docs/assets/ANIMATION.md for the format.

CLI:  python tools/assets/anim.py baserom.us.z64 --out DIR [--objects 83,84] [--gltf] [--png]
"""
import argparse
import json
import math
import os
import struct
import sys
import zlib

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import propack  # noqa: E402  (tools/assets/propack.py, part a1)

# ----------------------------------------------------------------------------------------------
# Locating: tools/assets/romfs.py (cartridge index -> objects index -> object), kept apart from
# the parsing; (rom_offset, size) pairs below are ROM nodes' ranges.
# ----------------------------------------------------------------------------------------------
import romfs  # noqa: E402

CARTRIDGE_INDEX = romfs.CARTRIDGE  # CEngineApp__Main: e->m_CartridgeIndex = 0xFCBFB4
CART_OBJECTS = 0             # CScene__Construct: CISet__GetROMAddress(cart, file, 0) = objects index
OBJ_INFO, OBJ_GEOMETRY, OBJ_ANIMATIONS, OBJ_STATIC_EVENTS = range(4)  # CGameObjectInstance__SetupAnimation


def u32(b, o):
    return struct.unpack_from(">I", b, o)[0]


def iset(b, o=0):
    """CISet: s32 count, s32 offsets[count + 1] (byte offsets from the set start; the last is the end).
    -> [(abs_offset, size)] (CISet__GetBlockAndSize)."""
    n = u32(b, o)
    offs = struct.unpack_from(">%dI" % (n + 1), b, o + 4)
    return [(o + offs[i], offs[i + 1] - offs[i]) for i in range(n)]


def uset(b, o):
    """CUSet: s32 element size, s32 count, then count elements. -> (size, count, data_offset)."""
    size, count = struct.unpack_from(">II", b, o)
    return size, count, o + 8


def objects(rom):
    """-> list of (rom_offset, size) of every object in the objects index (rom = bytes or RomFS)."""
    fs = rom if isinstance(rom, romfs.RomFS) else romfs.RomFS(rom)
    return [(n.rom, n.size) for n in (fs.object(i) for i in range(fs.count("objects")))]


def object_entries(rom, obj_off):
    """The object's 4 entries [(rom_offset, size)]: info, geometry, animations, extra."""
    return iset(rom, obj_off)


def is_animated(rom, obj_off):
    e = object_entries(rom, obj_off)
    return len(e) > OBJ_ANIMATIONS and e[OBJ_ANIMATIONS][1] > 8


def unpack(rom, off, size):
    """ProPackDecompress: RNC -> unpacked bytes, otherwise the raw bytes."""
    return propack.maybe_decompress(rom, off, size)


# Action ids (anim object info block 1) as the enemy AI picks them: the m_Anim column of the
# sEnemyAnim<Kind>_<addr> CEnemyAnimPick tables (include/globals/enemy.h, rodata 0x800AF6F0..).
# id // 10 * 10 groups up to 5 variants (+0..+4); the hundreds pick the situation.
ACTION_GROUPS = [
    (1, 124, "Idle"), (200, 224, "Alert"), (300, 324, "TakeCover"),
    (2000, 2224, "Move/Patrol"), (2700, 2724, "Flee"), (3120, 3724, "Move"),
    (4000, 4024, "Combat"), (4100, 4124, "Leap"), (4200, 4214, "Dart"), (4300, 4314, "Projectile"),
    (4400, 4504, "Weapon"), (5000, 5024, "ComboEnd"), (7000, 7042, "NormalDeath"),
    (7100, 7124, "ViolentDeath"), (7200, 7224, "ExplosiveDeath"), (7400, 7404, "NormalDeath"),
    (7500, 7524, "MovingDeath"), (8000, 8124, "Evade"), (8200, 8324, "Move"),
    (9600, 9624, "LeftArmWound"), (9630, 9654, "RightArmWound"), (11300, 11312, "JoypadTest"),
]


def action_group(action):
    for lo, hi, name in ACTION_GROUPS:
        if lo <= action <= hi:
            return name
    return None


# ----------------------------------------------------------------------------------------------
# Anim object info (CGameObjectInstance__SetupAnimation, __PickRandomAnim, __Draw, Sweep code)
# ----------------------------------------------------------------------------------------------
def half_to_float(h):
    """HalfToFloat: the top 16 bits of an IEEE single."""
    return struct.unpack(">f", struct.pack(">I", h << 16))[0]


def parse_object_info(b):
    """Set of 2: [0] 0x20 bytes: f32 bbox min[3], max[3] (CMtxF__TransformBox, the Sweep tests),
    s8 neck bone, s8 head bone, s8 head-turn bone count (AI_TurnHeadToTarget; -1/-1/0: none),
    u8 ?, u16 intelligence id (CScene__GetIntelligence), u16 scale (HalfToFloat, func_0021F2B0);
    [1] CUSet of s32: the action id of every animation, sorted (CGameObjectInstance__PickRandomAnim
    BinarySearch-es it; GetBlock(1)[3] = the count = actor animation count 0xBA)."""
    blk = iset(b)
    o0, s0 = blk[0]
    box = struct.unpack_from(">6f", b, o0)
    neck, head, nturn, b1b, intel, scale = struct.unpack_from(">bbbBHH", b, o0 + 0x18)
    esize, count, d = uset(b, blk[1][0])
    actions = list(struct.unpack_from(">%di" % count, b, d)) if esize == 4 else None
    return {
        "bbox_min": box[:3], "bbox_max": box[3:],
        "neck_bone": neck, "head_bone": head, "head_turn_bones": nturn, "byte1b": b1b,
        "intelligence": intel, "scale": half_to_float(scale),
        "actions": actions,
    }


# ----------------------------------------------------------------------------------------------
# Skeleton (anim geometry block 3; CGameObjectInstance__PoseBone)
# ----------------------------------------------------------------------------------------------
GEOM_SKELETON = 3


def parse_skeleton(geom):
    """geom = RNC-unpacked "anim geometry" (6-block set). Block 3: set of bones; bone i =
    set {[0] CUSet of s32 child bone indices, [1] bone data (0x70 bytes)}.
    Bone data: 0x00 8 corner points (24 floats: the bone-space box used by the hit tests),
    0x60 f32 radius (node->size), 0x64 u8 parent bone (0xFF: root), 0x65 u8 part flags
    (big-head cheats test flags & 0xC7 == 0xC3/0xC7/0xC2/0xC6, & 3 == 1), 0x66 u8, 0x67 u8,
    0x68 f32, 0x6C u32 hit mask (CGameObjectInstance__FireAnimEvents: & obj->boneMask)."""
    gb = iset(geom)
    bones = []
    for i, (o, s) in enumerate(iset(geom, gb[GEOM_SKELETON][0])):
        nb = iset(geom, o)
        esize, count, d = uset(geom, nb[0][0])
        children = list(struct.unpack_from(">%di" % count, geom, d)) if count else []
        bo, bs = nb[1]
        if bs < 0x70:
            raise ValueError("bone %d: data 0x%x bytes" % (i, bs))
        corners = struct.unpack_from(">24f", geom, bo)
        radius, = struct.unpack_from(">f", geom, bo + 0x60)
        parent, flags, b66, b67 = geom[bo + 0x64:bo + 0x68]
        f68, mask = struct.unpack_from(">fI", geom, bo + 0x68)
        bones.append({
            "index": i,
            "parent": -1 if parent == 0xFF else parent,
            "children": children,
            "corners": [corners[k:k + 3] for k in range(0, 24, 3)],
            "radius": radius, "flags": flags, "byte66": b66, "byte67": b67,
            "float68": f68, "mask": mask,
        })
    for b in bones:  # the stored parent agrees with the child lists (checked on every object)
        for c in b["children"]:
            if bones[c]["parent"] != b["index"]:
                raise ValueError("bone %d: parent byte %d, child of %d" % (c, bones[c]["parent"], b["index"]))
    return bones


# ----------------------------------------------------------------------------------------------
# Bit stream (Bits_Read: LSB-first inside big-endian 32-bit words)
# ----------------------------------------------------------------------------------------------
class Bits:
    def __init__(self, buf, byte_off):
        assert byte_off % 4 == 0
        n = (len(buf) - byte_off) // 4
        self.w = list(struct.unpack_from(">%dI" % n, buf, byte_off)) + [0, 0]
        self.pos = 0  # bit address relative to byte_off

    def peek(self, width, pos=None):
        p = self.pos if pos is None else pos
        i, sh = p >> 5, p & 31
        v = (self.w[i] | (self.w[i + 1] << 32)) >> sh
        return v & 0xFFFFFFFF if width >= 32 else v & ((1 << width) - 1)

    def read(self, width):
        v = self.peek(width)
        self.pos += width
        return v


def f32_from_bits(v):
    return struct.unpack(">f", struct.pack(">I", v))[0]


def f32(x):
    """Round a Python float to single precision (the N64 computes in f32)."""
    try:
        return struct.unpack(">f", struct.pack(">f", x))[0]
    except OverflowError:
        return x


def normalize(v, bits):          # Bits_Normalize
    return v / float((1 << bits) - 1)


def remap(v, lo, hi):           # Math_Remap
    return v * (hi - lo) + lo


def read_range(bs, float_bounds, rmin, rmax):
    """Range header shared by both lossy encodings: 32-bit float bounds (positions) or two 6-bit
    values normalized and remapped into [rmin, rmax] (rotations, [-1, 1])."""
    if float_bounds:
        lo = f32_from_bits(bs.read(32))
        hi = f32_from_bits(bs.read(32))
    else:
        lo = remap(normalize(bs.read(6), 6), rmin, rmax)
        hi = remap(normalize(bs.read(6), 6), rmin, rmax)
    return f32(lo), f32(hi)


def decode_curve_channel(bs, frames, float_bounds, rmin, rmax):
    """Anim_DecodeCurveChannel: piecewise cubic. Header: 6-bit segment count, range, 6-bit
    width; then per segment a 6-bit span and 4 width-bit control values (AnimCurve_LoadSegment
    turns them into cubic coefficients); the curve is sampled at every frame."""
    nseg = bs.read(6)
    lo, hi = read_range(bs, float_bounds, rmin, rmax)
    base, rng = lo, f32(hi - lo)
    width = bs.read(6)
    data = bs.pos
    rec = width * 4 + 6

    def segment(index, base, rng):
        p = data + index * rec
        span = bs.peek(6, p)
        p += 6
        p0 = bs.peek(width, p)
        p1 = bs.peek(width, p + width)
        p2 = bs.peek(width, p + 2 * width)
        p3 = bs.peek(width, p + 3 * width)
        c1 = (32 * p1 - 21 * p0 - 12 * p2 + p3) * 0.33333334
        c2 = float(14 * p0 - 32 * p1 + 20 * p2 - 2 * p3)
        c3 = (64 * p1 - 24 * p0 - 48 * p2 + 8 * p3) * 0.33333334
        if width != 0:
            scale = rng / float((1 << width) - 1)
        else:  # constant channel: the midpoint (the engine folds it into the base, range -> 0)
            base = base + rng * 0.5
            rng = 0.0
            scale = 0.0
        inv = 1.0 / (span - 1.0) if span >= 2 else 1.0
        k = inv * scale
        coef = (p0 * scale + base, c1 * k, c2 * k * inv, c3 * k * inv * inv)
        return span, coef, base, rng

    out = []
    seg, start = 0, 0
    span, coef, base, rng = segment(0, base, rng)
    for f in range(frames):
        while f >= start + span and seg + 1 < nseg:   # AnimCurve_Evaluate walks forward
            start += span
            seg += 1
            span, coef, base, rng = segment(seg, base, rng)
        dt = f - start
        out.append(f32(coef[0] + coef[1] * dt + coef[2] * dt * dt + coef[3] * dt * dt * dt))
    bs.pos = data + nseg * rec
    return out, {"segments": nseg, "width": width, "lo": lo, "hi": hi}


def decode_quantized_channel(bs, frames, float_bounds, rmin, rmax):
    """Anim_DecodeQuantizedChannel: range, 6-bit width, then one width-bit value per frame."""
    lo, hi = read_range(bs, float_bounds, rmin, rmax)
    rng = f32(hi - lo)
    width = bs.read(6)
    out = []
    for _ in range(frames):
        v = bs.read(width)
        out.append(f32(v / float((1 << width) - 1) * rng + lo) if width else lo)
    return out, {"width": width, "lo": lo, "hi": hi}


def decode_raw_channel(bs, frames, float_bounds, rmin, rmax):
    """Anim_DecodeRawChannel: one 32-bit float per frame."""
    return [f32_from_bits(bs.read(32)) for _ in range(frames)], {}


DECODERS = {0: decode_curve_channel, 1: decode_quantized_channel, 2: decode_raw_channel}
ENCODING_NAMES = {0: "cubic", 1: "quantized", 2: "raw"}


def normalize_quat(q):
    n = math.sqrt(sum(c * c for c in q))
    return tuple(c / n for c in q) if n > 0 else (0.0, 0.0, 0.0, 1.0)


def decode_tracks(buf, off, tracks, channels, frames, radius, encoding, float_bounds, rmin, rmax,
                  meta=None):
    """Anim_DecodeTracks: for every track, for every channel: skip 31 bits (unused header word),
    decode `frames` values. Rotations (4 channels) get a wrapping box blur of +-radius frames and
    are renormalized. -> list of tracks, each a list of per-frame tuples."""
    dec = DECODERS[encoding]
    bs = Bits(buf, off)
    result = []
    for t in range(tracks):
        chans = []
        for c in range(channels):
            hdr = bs.read(31)
            vals, info = dec(bs, frames, float_bounds, rmin, rmax)
            if meta is not None:
                info["skip31"] = hdr
                meta.append(info)
            chans.append(vals)
        result.append([list(x) for x in zip(*chans)])
    if channels == 4:
        for keys in result:
            if radius:
                for k in range(4):
                    for j in range(frames):  # in place, like the C (already-blurred keys feed later ones)
                        tot = 0.0
                        wgt = 0.0
                        for d in range(-radius, radius + 1):
                            wgt += 1.0
                            tot += keys[(j + d + frames) % frames][k]
                        keys[j][k] = tot / wgt
            for j in range(frames):
                keys[j] = list(normalize_quat(keys[j]))
    return [[tuple(f) for f in keys] for keys in result], bs.pos


# ----------------------------------------------------------------------------------------------
# Animation resource (CAnimationState__*, Anim_Decompress)
# ----------------------------------------------------------------------------------------------
def parse_animation(b, decode=True):
    """b = RNC-unpacked animation (a 6-chunk set):
      [0] CUSet s16: root yaw per frame (pi/32768 units)       CAnimationState__GetRootYaw
      [1] CUSet {s16 pos_track, s16 rot_track} per bone (-1 = use the default frame)
      [2] CUSet {f32 pos[3], s16 rot[4] (/32767)} per bone: the default (rest) frame
      [3] set: [0] clip header {u16 blend_frames, u16 loop_start (0xFFFF none), u8 ?,
               u8 blend_start, u8 ?, u8 blend_end, f32 rate (frames per 1/15 s tick * 15)}, [1] ?
      [4] CUSet of 28-byte events {u16 frame, u16 effect (impact event id), s16 bone (-1: object
          space, y/z swapped), s16 pad, f32 local[3], s32 params[2]} (FireAnimEvents)
      [5] set: [0] header 0x20 {radius, frames, encoding, tolerance, rot_tracks, pos_tracks, -, -},
               [1] position track stream, [2] rotation track stream (Anim_DecodeAnimation)"""
    ch = iset(b)
    if len(ch) != 6:
        raise ValueError("animation: %d chunks" % len(ch))
    a = {}
    es, n, d = uset(b, ch[0][0])
    a["frames"] = n
    a["root_yaw"] = [v * 9.587673e-05 for v in struct.unpack_from(">%dh" % n, b, d)] if es == 2 else None
    es, n, d = uset(b, ch[1][0])
    a["joints"] = [struct.unpack_from(">hh", b, d + 4 * i) for i in range(n)]
    es, n, d = uset(b, ch[2][0])
    rest = []
    for i in range(n):
        o = d + es * i
        p = struct.unpack_from(">3f", b, o)
        q = struct.unpack_from(">4h", b, o + 12)
        rest.append({"pos": p, "rot": tuple(c / 32767.0 for c in q)})
    a["rest"] = rest
    c3 = iset(b, ch[3][0])
    o3 = c3[0][0]
    bf, loop = struct.unpack_from(">HH", b, o3)
    a["clip"] = {
        "blend_frames": bf,
        "loop_start": None if loop == 0xFFFF else loop,
        "byte4": b[o3 + 4], "blend_start": b[o3 + 5], "byte6": b[o3 + 6], "blend_end": b[o3 + 7],
        "rate": struct.unpack_from(">f", b, o3 + 8)[0],
        "raw": b[o3:o3 + c3[0][1]].hex(),
        "block1": b[c3[1][0]:c3[1][0] + c3[1][1]].hex() if len(c3) > 1 else None,
    }
    es, n, d = uset(b, ch[4][0])
    ev = []
    for i in range(n):  # CROMEventEntry (CGameObjectInstance__FireAnimEvents), 28 bytes
        frame, effect, bone, pad = struct.unpack_from(">HHhh", b, d + es * i)
        local = struct.unpack_from(">3f", b, d + es * i + 8)
        params = struct.unpack_from(">ii", b, d + es * i + 20)
        ev.append({"frame": frame, "effect": effect, "bone": bone, "pad": pad,
                   "local": local, "params": params})
    a["events"] = ev
    c5 = iset(b, ch[5][0])
    radius, frames, encoding, tol, rot_tracks, pos_tracks = struct.unpack_from(">iiifii", b, c5[0][0])
    a["compressed"] = {"radius": radius, "frames": frames, "encoding": encoding,
                       "encoding_name": ENCODING_NAMES.get(encoding), "tolerance": tol,
                       "rot_tracks": rot_tracks, "pos_tracks": pos_tracks,
                       "pos_stream_bytes": c5[1][1], "rot_stream_bytes": c5[2][1]}
    if decode:
        pm, rm = [], []
        # Anim_DecodeAnimation: positions with float bounds (remap range +-2000, unused there),
        # rotations with 6-bit bounds remapped to [-1, 1].
        a["pos_tracks"], pbits = decode_tracks(b, c5[1][0], pos_tracks, 3, frames, radius, encoding,
                                               True, -2000.0, 2000.0, pm)
        a["rot_tracks"], rbits = decode_tracks(b, c5[2][0], rot_tracks, 4, frames, radius, encoding,
                                               False, -1.0, 1.0, rm)
        a["compressed"]["pos_bits_used"] = pbits
        a["compressed"]["rot_bits_used"] = rbits
        a["channel_info"] = {"pos": pm, "rot": rm}
    return a


def bone_local(anim, bone, frame):
    """The bone's local translation and rotation quaternion (x, y, z, w) at an integer frame
    (PoseBone's trackPos / lerpRot with blend 0)."""
    pt, rt = anim["joints"][bone]
    pos = anim["pos_tracks"][pt][frame] if pt != -1 else anim["rest"][bone]["pos"]
    rot = anim["rot_tracks"][rt][frame] if rt != -1 else anim["rest"][bone]["rot"]
    return pos, rot


def quat_mtx(q, t):
    """PoseBone's quatToMtx: row-vector matrix (translation in the last row)."""
    x, y, z, w = q
    xx, yy, zz, ww = x * x, y * y, z * z, w * w
    xy, wz, xz, wy, yz, wx = 2 * x * y, 2 * w * z, 2 * x * z, 2 * w * y, 2 * y * z, 2 * w * x
    return [[ww + xx - yy - zz, xy + wz, xz - wy, 0.0],
            [xy - wz, ww - xx + yy - zz, yz + wx, 0.0],
            [xz + wy, yz - wx, ww - xx - yy + zz, 0.0],
            [t[0], t[1], t[2], 1.0]]


def mmul(a, b):
    return [[sum(a[i][k] * b[k][j] for k in range(4)) for j in range(4)] for i in range(4)]


IDENTITY = [[1.0 if i == j else 0.0 for j in range(4)] for i in range(4)]


def pose(anim, bones, frame):
    """World matrices of every bone (cur = local * parent, recursing from bone 0)."""
    mats = [None] * len(bones)

    def rec(i, parent):
        p, q = bone_local(anim, i, frame)
        mats[i] = mmul(quat_mtx(q, p), parent)
        for c in bones[i]["children"]:
            rec(c, mats[i])
    rec(0, IDENTITY)
    return mats


# ----------------------------------------------------------------------------------------------
# glTF 2.0 export (skeleton nodes + animations; mesh left to the models exporter)
# ----------------------------------------------------------------------------------------------
def gltf_export(path, name, bones, anims, fps=15.0):
    """Writes path.gltf + path.bin: one node per bone (rest pose from the first animation),
    a skin-less node tree and one glTF animation per clip (translation + rotation samplers,
    linear, `fps` frames per second scaled by the clip rate)."""
    binbuf = bytearray()
    accessors, views = [], []

    def add(data_floats, comps, typ, minmax=False):
        off = len(binbuf)
        binbuf.extend(struct.pack("<%df" % len(data_floats), *data_floats))
        views.append({"buffer": 0, "byteOffset": off, "byteLength": len(data_floats) * 4})
        acc = {"bufferView": len(views) - 1, "componentType": 5126,
               "count": len(data_floats) // comps, "type": typ}
        if minmax:
            acc["min"] = [min(data_floats[i::comps]) for i in range(comps)]
            acc["max"] = [max(data_floats[i::comps]) for i in range(comps)]
        accessors.append(acc)
        return len(accessors) - 1

    first = anims[0][1] if anims else None
    nodes = []
    for b in bones:
        n = {"name": "bone%02d" % b["index"]}
        if b["children"]:
            n["children"] = list(b["children"])
        if first is not None and b["index"] < len(first["joints"]):
            p, q = bone_local(first, b["index"], 0)
            n["translation"] = list(p)
            n["rotation"] = list(q)
        nodes.append(n)
    root = len(nodes)
    # the models are Z-up; glTF is Y-up: rotate the root -90 degrees about X
    nodes.append({"name": name, "children": [0], "rotation": [-0.70710678, 0.0, 0.0, 0.70710678]})
    gl_anims = []
    for idx, a in anims:
        frames = a["frames"]
        rate = a["clip"]["rate"] or 15.0
        times = [f / rate for f in range(frames)]
        tacc = add(times, 1, "SCALAR", True)
        samplers, channels = [], []
        for b in bones:
            i = b["index"]
            if i >= len(a["joints"]):
                continue
            pt, rt = a["joints"][i]
            if pt != -1:
                vals = [c for f in a["pos_tracks"][pt] for c in f]
                samplers.append({"input": tacc, "output": add(vals, 3, "VEC3"), "interpolation": "LINEAR"})
                channels.append({"sampler": len(samplers) - 1, "target": {"node": i, "path": "translation"}})
            if rt != -1:
                vals = [c for f in a["rot_tracks"][rt] for c in f]
                samplers.append({"input": tacc, "output": add(vals, 4, "VEC4"), "interpolation": "LINEAR"})
                channels.append({"sampler": len(samplers) - 1, "target": {"node": i, "path": "rotation"}})
        if channels:
            gl_anims.append({"name": "anim%03d" % idx, "samplers": samplers, "channels": channels})
    doc = {
        "asset": {"version": "2.0", "generator": "turok2 tools/assets/anim.py"},
        "scene": 0, "scenes": [{"nodes": [root]}],
        "nodes": nodes, "animations": gl_anims,
        "buffers": [{"uri": os.path.basename(path) + ".bin", "byteLength": len(binbuf)}],
        "bufferViews": views, "accessors": accessors,
    }
    if not gl_anims:
        del doc["animations"]
    if not accessors:
        del doc["buffers"], doc["bufferViews"], doc["accessors"]
    with open(path + ".bin", "wb") as f:
        f.write(binbuf)
    with open(path + ".gltf", "w") as f:
        json.dump(doc, f)


# ----------------------------------------------------------------------------------------------
# PNG preview: stick figures of a few frames (front and side views)
# ----------------------------------------------------------------------------------------------
def write_png(path, w, h, rgb):
    raw = b"".join(b"\0" + bytes(rgb[y * w * 3:(y + 1) * w * 3]) for y in range(h))

    def chunk(t, d):
        c = struct.pack(">I", len(d)) + t + d
        return c + struct.pack(">I", zlib.crc32(t + d) & 0xFFFFFFFF)
    with open(path, "wb") as f:
        f.write(b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 2, 0, 0, 0))
                + chunk(b"IDAT", zlib.compress(raw, 9)) + chunk(b"IEND", b""))


def render_png(path, anim, bones, nframes=6, cell=160):
    frames = anim["frames"]
    sel = sorted(set(int(round(i * (frames - 1) / max(1, nframes - 1))) for i in range(nframes)))
    poses = [pose(anim, bones, f) for f in sel]
    pts = [[(m[3][0], m[3][1], m[3][2]) for m in p] for p in poses]
    allp = [q for p in pts for q in p]
    lo = [min(q[k] for q in allp) for k in range(3)]
    hi = [max(q[k] for q in allp) for k in range(3)]
    mid = [(lo[k] + hi[k]) / 2 for k in range(3)]
    ext = max(hi[k] - lo[k] for k in range(3)) or 1.0
    W, H = cell * len(sel), cell * 2
    img = bytearray([255] * (W * H * 3))

    def plot(x, y, col):
        if 0 <= x < W and 0 <= y < H:
            o = (y * W + x) * 3
            img[o:o + 3] = bytes(col)

    def line(x0, y0, x1, y1, col):
        n = int(max(abs(x1 - x0), abs(y1 - y0))) + 1
        for i in range(n + 1):
            t = i / n
            plot(int(x0 + (x1 - x0) * t), int(y0 + (y1 - y0) * t), col)

    s = cell * 0.9 / ext
    for ci, p in enumerate(pts):
        # the engine's models are Z-up: row 0 = front (x right, z up), row 1 = side (y right, z up)
        for row, ax in enumerate((0, 1)):
            cx, cy = ci * cell + cell // 2, row * cell + cell // 2

            def pr(q):
                return cx + (q[ax] - mid[ax]) * s, cy - (q[2] - mid[2]) * s
            for b in bones:
                if b["parent"] >= 0:
                    (x0, y0), (x1, y1) = pr(p[b["parent"]]), pr(p[b["index"]])
                    line(x0, y0, x1, y1, (200, 30, 30) if b["parent"] == 0 else (20, 20, 20))
            for q in p:
                x, y = pr(q)
                for dx in (-1, 0, 1):
                    for dy in (-1, 0, 1):
                        plot(int(x) + dx, int(y) + dy, (30, 30, 220))
        for y in range(H):
            plot(ci * cell, y, (180, 180, 180))
    write_png(path, W, H, img)


# ----------------------------------------------------------------------------------------------
# Driver
# ----------------------------------------------------------------------------------------------
def load_object(rom, index, obj_off, decode=True):
    ent = object_entries(rom, obj_off)
    info = parse_object_info(unpack(rom, *ent[OBJ_INFO]))
    geom = unpack(rom, *ent[OBJ_GEOMETRY])
    bones = parse_skeleton(geom)
    aset = iset(rom, ent[OBJ_ANIMATIONS][0])
    anims = []
    for i, (o, s) in enumerate(aset):
        a = parse_animation(unpack(rom, o, s), decode)
        a["rom"] = o
        a["rom_size"] = s
        a["action"] = info["actions"][i] if info["actions"] and i < len(info["actions"]) else None
        a["action_group"] = action_group(a["action"]) if a["action"] is not None else None
        anims.append((i, a))
    return {
        "object": index, "rom": obj_off,
        "entries": [{"rom": o, "size": s} for o, s in ent],
        "info": info, "bones": bones, "animations": anims,
    }


def main(argv=None):
    ap = argparse.ArgumentParser(description="Turok 2 skeletons and animations")
    ap.add_argument("rom")
    ap.add_argument("--out", required=True)
    ap.add_argument("--objects", help="comma-separated object indices (default: every animated one)")
    ap.add_argument("--gltf", action="store_true", help="also write glTF (skeleton + animations)")
    ap.add_argument("--png", action="store_true", help="also write a stick-figure PNG of one animation")
    ap.add_argument("--anim", type=int, default=0, help="animation index for --png (default 0)")
    ap.add_argument("--no-keys", action="store_true", help="JSON without the decoded per-frame keys")
    a = ap.parse_args(argv)
    rom = open(a.rom, "rb").read()
    os.makedirs(a.out, exist_ok=True)
    objs = objects(rom)
    sel = [int(x, 0) for x in a.objects.split(",")] if a.objects else range(len(objs))
    summary = []
    for i in sel:
        off, _ = objs[i]
        if not is_animated(rom, off):
            continue
        try:
            o = load_object(rom, i, off)
        except Exception as e:  # report and keep going
            print("object %d: %s" % (i, e), file=sys.stderr)
            summary.append({"object": i, "error": str(e)})
            continue
        anims = o["animations"]
        base = os.path.join(a.out, "obj%04d" % i)
        js = dict(o)
        js["animations"] = []
        for idx, an in anims:
            d = dict(an)
            if a.no_keys:
                for k in ("pos_tracks", "rot_tracks", "channel_info"):
                    d.pop(k, None)
            d["index"] = idx
            js["animations"].append(d)
        with open(base + ".json", "w") as f:
            json.dump(js, f)
        if a.gltf:
            gltf_export(base, "obj%04d" % i, o["bones"], anims)
        if a.png and a.anim < len(anims):
            render_png(base + "_anim%03d.png" % a.anim, anims[a.anim][1], o["bones"])
        summary.append({"object": i, "bones": len(o["bones"]), "animations": len(anims)})
    with open(os.path.join(a.out, "index.json"), "w") as f:
        json.dump(summary, f, indent=1)
    ok = [s for s in summary if "error" not in s]
    print("%d animated objects, %d animations, %d errors" % (
        len(ok), sum(s["animations"] for s in ok), len(summary) - len(ok)))


if __name__ == "__main__":
    main()
