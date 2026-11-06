#!/usr/bin/env python3
"""Turok 2 (N64) cinemas (in-engine cutscenes): decode the cartridge's cinema set to JSON.

    python tools/assets/cinemas.py baserom.us.z64 --out DIR

Cartridge entry 18 "Cinemas index" (CScene__Construct) is a set: block 0 is the cinema table
(Cinema_Load / Cinema_RequestById / Cinema_RequestByName), block i+1 the track of cinema i
(Cinema_Load -> CCache__Request(..., Cinema_PrepareResource, "Cinema")).  Writes
DIR/cinemas/index.json and DIR/cinemas/NNN_<name>.json.  Formats: docs/assets/LEVELS.md.
"""
import argparse
import json
import os
import re
import struct
import sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from levels import CARTRIDGE_INDEX, blocks, cstr, f, rom_set, table, unpack  # noqa: E402


def parse_table(b):
    """{0x4C, n} + n CinemaEntry (structs/cinema.h): s32 warp id, s32 id4, s32 id, u32 flags
    (1 follow target, 2 follow player, 8 save start), 8 unused bytes, char name[0x20],
    f32 far clip (/10.24), f32 aspect, 3 s32."""
    w, c, d = table(b)
    out = []
    for i in range(c):
        r = d[i * w:(i + 1) * w]
        id0, id4, cid, flags = struct.unpack_from(">iiiI", r, 0)
        sc, asp, a, b_, c_ = struct.unpack_from(">2f3i", r, 0x38)
        out.append({"index": i, "name": cstr(r[0x18:0x38]), "id": cid, "warp_id": id0, "id4": id4,
                    "flags": flags, "far_clip": f(sc), "aspect": f(asp), "unk40": [a, b_, c_]})
    return out


def vec_table(b, n_per):
    w, c, d = table(b)
    return [[f(v) for v in struct.unpack_from(">%df" % n_per, d, i * w)] for i in range(c)]


def parse_track(d):
    """A cinema track after RNC: a set of 4.
      0 clips: a set of clips, each a set of 5 plain tables: eye points {0xC, n, f32 xyz},
        eye knot values {4, n, f32}, target points, target knots, 8 bytes (Cinema_PrepareResource
        builds the cubic splines, Cinema_SplineBuild).
      1 fade keys {0x14, n}: f32 r, g, b, alpha, time (Cinema_GetFadeColor / Cinema_GetFadeAlpha).
      2 cues {0xC, n}: f32 time, s32 list (0 records, 1 alt records), s32 event index into the
        level's links block (Cinema_FireCues fires the LevelEvent).
      3 fov keys {8, n}: f32 fov, f32 time (Cinema_GetFOV).  Times are frames (15 per second)."""
    bl = blocks(d)
    clips = []
    for clip in blocks(bl[0]):
        cb = blocks(clip)
        clips.append({"eye": vec_table(cb[0], 3), "eye_knots": [v[0] for v in vec_table(cb[1], 1)],
                      "target": vec_table(cb[2], 3), "target_knots": [v[0] for v in vec_table(cb[3], 1)],
                      "extra": cb[4].hex() if len(cb) > 4 else None})
    fade = [{"color": v[0:3], "alpha": v[3], "time": v[4]} for v in vec_table(bl[1], 5)]
    w, c, dd = table(bl[2])
    cues = []
    for i in range(c):
        t, lst, idx = struct.unpack_from(">fii", dd, i * w)
        cues.append({"time": f(t), "list": "alt_records" if lst else "records", "event": idx})
    fov = [{"fov": v[0], "time": v[1]} for v in vec_table(bl[3], 2)]
    return {"clips": clips, "fade": fade, "cues": cues, "fov": fov}


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("rom")
    ap.add_argument("--out", required=True)
    a = ap.parse_args(argv)
    rom = open(a.rom, "rb").read()
    cart = rom_set(rom, CARTRIDGE_INDEX)
    cs = rom_set(rom, cart[18][0])
    t0, s0 = cs[0]
    entries = parse_table(rom[t0:t0 + s0])
    d = os.path.join(a.out, "cinemas")
    os.makedirs(d, exist_ok=True)
    for e in entries:
        k = e["index"] + 1
        if k >= len(cs):
            continue
        ro, sz = cs[k]
        e["rom"], e["size"], e["compressed"] = hex(ro), sz, rom[ro:ro + 3] == b"RNC"
        tr = parse_track(unpack(rom[ro:ro + sz]))
        e["clips"], e["cues"] = len(tr["clips"]), len(tr["cues"])
        e["length_frames"] = f(sum(cl["eye_knots"][-1] for cl in tr["clips"] if cl["eye_knots"]))
        fn = "%03d_%s.json" % (e["index"], re.sub(r"[^A-Za-z0-9]+", "_", e["name"]).strip("_"))
        e["file"] = fn
        with open(os.path.join(d, fn), "w") as fp:
            json.dump(dict(e, track=tr), fp, indent=1)
    with open(os.path.join(d, "index.json"), "w") as fp:
        json.dump(entries, fp, indent=1)
    print("%d cinemas -> %s" % (len(entries), d))


if __name__ == "__main__":
    main()
