#!/usr/bin/env python3
"""Extract every Turok 2 asset from the player's ROM in one run.

    python tools/assets/extract_all.py baserom.us.z64 --out DIR [--jobs N] [--only AREA,...]
                                       [--no-streams] [--no-png]

1. romid.py identifies the image (.z64 / .v64 / .n64) and refuses anything but the supported
   US release (SHA1 fb0400f2...06f1). A byte-swapped image is normalised to DIR/.rom.z64 for the
   run and removed afterwards.
2. Every area's exporter runs as its own process (in parallel, --jobs):
     romid      DIR/romid/romid.json                       offsets and hashes only
     romfs      DIR/romfs/romfs_tree.tsv/.json, romfs_types.tsv (tree + per-type coverage)
     textures   DIR/textures/textures/*.png, graphics/*/*.png, textures.json, graphics.json
     models     DIR/models/objNNNN.obj/.gltf, index.json  (animated models posed at frame 0 of
                animation 0 through anim.py + models.apply_pose)
     anim       DIR/anim/objNNNN.json/.gltf, index.json
     levels     DIR/levels/levels/*.json, maps/*.png, warp_dests.json
     cinemas    DIR/cinemas/cinemas/*.json
     audio      DIR/audio/banks.json, soundtree.json, *_waves/*.wav, sequences/*.mid, streams/*.wav
3. Prints a summary (files and bytes per area) and the coverage table: every ROM byte of
   0x1E0000..0x1FF7AEC classified by area (from romfs: ROM-level leaves + index headers),
   with the exporter that turns it into files. Also written to DIR/summary.json.

The output is game data: keep it out of the repository (.gitignore has assets_out/).
"""
import argparse
import json
import os
import subprocess
import sys
import time
from concurrent.futures import ThreadPoolExecutor

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)
import romfs  # noqa: E402
import romid  # noqa: E402

# area -> (script, extra args)
AREAS = [
    ("audio", "audio.py", []),
    ("anim", "anim.py", ["--gltf"]),
    ("models", "models.py", ["--obj", "--gltf", "--json", "--pose"]),
    ("levels", "levels.py", ["--map", "-1"]),
    ("textures", "textures.py", []),
    ("romfs", "romfs.py", ["--coverage"]),
    ("cinemas", "cinemas.py", []),
    ("romid", "romid.py", []),
]

# Coverage classification of ROM-level nodes: (path prefix, area, exporter or None).
# Most specific first; matched on "/"-separated components.
CLASSES = [
    ("audioctl", "audio: control block (banks)", "audio"),
    ("audiotbl", "audio: wave table (VADPCM)", "audio"),
    ("sound", "audio: sound data file (tree, sequences, streams)", "audio"),
    ("cart/0/*/0", "animation: anim object info", "anim"),
    ("cart/0/*/1", "models: geometry", "models"),
    ("cart/0/*/2", "animation: animations", "anim"),
    ("cart/0/*/3", "models: static events", None),
    ("cart/2", "models: object types", "models"),
    ("cart/10", "models: object types 2", None),
    ("cart/3", "textures: texture banks", "textures"),
    ("cart/16", "textures: graphic sets", "textures"),
    ("cart/17", "textures: graphic set types", "textures"),
    ("cart/5", "levels", "levels"),
    ("cart/8", "levels: warp dests", "levels"),
    ("cart/18", "cinemas", "cinemas"),
    ("cart/1", "game data: intelligence", None),
    ("cart/4", "game data: particle effects", None),
    ("cart/6", "game data: load/save persistent bits", None),
    ("cart/7", "game data: game persistent bits", None),
    ("cart/9", "game data: binaries index", None),
    ("cart/11", "game data: particle swooshes", None),
    ("cart/12", "game data: dynamic lights", None),
    ("cart/13", "game data: curves", None),
    ("cart/14", "game data: rumbles", None),
    ("cart/15", "game data: rumble types", None),
    ("cart/19", "game data: link group types", None),
    ("cart/20", "game data: end marker", None),
]


def _match(path, pat):
    p, q = path.split("/"), pat.split("/")
    return len(p) >= len(q) and all(b == "*" or a == b for a, b in zip(p, q))


def classify(path):
    for pat, area, exp in CLASSES:
        if _match(path, pat):
            return area, exp
    return "unclassified", None


def coverage(fs):
    """{area: [bytes, nodes, exporter]} over ROM-level nodes; index headers/padding apart."""
    out = {}
    for r in fs.regions:
        for n in fs.walk(r.name, decompress=False):
            if n.kind == "iset":
                area, exp = "index structure (ISet headers + padding)", "romfs"
                size = n._offs[0] + (n.size - n._offs[-1])
            else:
                area, exp = classify(n.path)
                size = n.size
            e = out.setdefault(area, [0, 0, exp])
            e[0] += size
            e[1] += 1
    return out


def dir_stats(d):
    files, size, ext = 0, 0, {}
    for root, _, names in os.walk(d):
        for f in names:
            files += 1
            s = os.path.getsize(os.path.join(root, f))
            size += s
            x = os.path.splitext(f)[1].lower() or "(none)"
            ext[x] = ext.get(x, 0) + 1
    return files, size, ext


def run_area(name, script, extra, rom, out, log_dir):
    t = time.time()
    cmd = [sys.executable, os.path.join(HERE, script), rom, "--out", os.path.join(out, name)] + extra
    with open(os.path.join(log_dir, name + ".log"), "w") as log:
        rc = subprocess.call(cmd, stdout=log, stderr=subprocess.STDOUT)
    return name, rc, time.time() - t


def last_line(path):
    try:
        lines = [x.strip() for x in open(path) if x.strip()]
    except OSError:
        return ""
    keep = [x for x in lines if not x.startswith("stream ")]
    return keep[-1] if keep else ""


def main(argv=None):
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("rom")
    ap.add_argument("--out", required=True)
    ap.add_argument("--jobs", type=int, default=len(AREAS))
    ap.add_argument("--only", help="comma-separated areas (%s)" % ",".join(a for a, _, _ in AREAS))
    ap.add_argument("--no-streams", action="store_true", help="audio: skip the adec streams (the slow part)")
    ap.add_argument("--no-png", action="store_true", help="textures: JSON only; levels: no maps")
    a = ap.parse_args(argv)
    t0 = time.time()

    with open(a.rom, "rb") as f:
        z64, sha1, known = romid.identify(f.read())
    if z64 is None:
        print("%s: not an N64 ROM image" % a.rom, file=sys.stderr)
        return 2
    if not known or not known.get("supported"):
        print("%s: unsupported image (sha1 %s); this extractor needs %s" % (a.rom, sha1, romid.SUPPORTED_SHA1),
              file=sys.stderr)
        return 1
    print("ROM: %s (sha1 %s)" % (known["name"], sha1))
    os.makedirs(a.out, exist_ok=True)
    rom_path, tmp_rom = a.rom, None
    with open(a.rom, "rb") as f:
        if f.read(4) != z64[:4]:
            tmp_rom = rom_path = os.path.join(a.out, ".rom.z64")
            with open(tmp_rom, "wb") as g:
                g.write(z64)
    log_dir = os.path.join(a.out, "logs")
    os.makedirs(log_dir, exist_ok=True)

    areas = AREAS
    if a.only:
        want = set(a.only.split(","))
        areas = [x for x in AREAS if x[0] in want]
    jobs = []
    for name, script, extra in areas:
        extra = list(extra)
        if name == "audio" and a.no_streams:
            extra.append("--no-streams")
        if a.no_png and name == "textures":
            extra.append("--no-png")
        if a.no_png and name == "levels":
            extra = []
        jobs.append((name, script, extra))
    results = {}
    try:
        with ThreadPoolExecutor(max_workers=max(1, a.jobs)) as ex:
            futs = [ex.submit(run_area, n, s, e, rom_path, a.out, log_dir) for n, s, e in jobs]
            for fu in futs:
                name, rc, dt = fu.result()
                results[name] = (rc, dt)
                print("  %-9s %s in %6.1fs  %s" % (name, "ok    " if rc == 0 else "FAILED", dt,
                                                    last_line(os.path.join(log_dir, name + ".log"))))
        fs = romfs.RomFS(z64)
        cov = coverage(fs)
    finally:
        if tmp_rom:
            os.remove(tmp_rom)

    total_t = time.time() - t0
    summary = {"rom_sha1": sha1, "seconds": round(total_t, 1), "areas": {}, "coverage": {}}
    print("\n%-9s %8s %12s  %s" % ("area", "files", "bytes", "by extension"))
    tf = tb = 0
    for name, _, _ in jobs:
        files, size, ext = dir_stats(os.path.join(a.out, name))
        tf, tb = tf + files, tb + size
        rc, dt = results[name]
        summary["areas"][name] = {"rc": rc, "seconds": round(dt, 1), "files": files, "bytes": size, "ext": ext}
        print("%-9s %8d %12d  %s" % (name, files, size,
                                     ", ".join("%s %d" % kv for kv in sorted(ext.items(), key=lambda kv: -kv[1]))))
    print("%-9s %8d %12d" % ("total", tf, tb))

    span = romfs.ASSET_END - romfs.ASSET_START
    print("\ncoverage of ROM 0x%X-0x%X (%d bytes):" % (romfs.ASSET_START, romfs.ASSET_END, span))
    print("%-52s %10s %7s %8s  %s" % ("area", "bytes", "%", "nodes", "exported by"))
    acc = exp = 0
    for area, (size, nodes, exporter) in sorted(cov.items(), key=lambda kv: -kv[1][0]):
        acc += size
        if exporter:
            exp += size
        summary["coverage"][area] = {"bytes": size, "nodes": nodes, "exporter": exporter}
        print("%-52s %10d %6.2f%% %8d  %s" % (area, size, 100.0 * size / span, nodes, exporter or "-"))
    unc = cov.get("unclassified", [0])[0]
    print("classified %d / %d bytes (%.4f%%), exported to files %.2f%%" % (
        acc - unc, span, 100.0 * (acc - unc) / span, 100.0 * exp / span))
    summary.update(classified_bytes=acc - unc, span=span, exported_bytes=exp)
    with open(os.path.join(a.out, "summary.json"), "w") as f:
        json.dump(summary, f, indent=1)
    failed = [n for n, (rc, _) in results.items() if rc != 0]
    print("done in %.1fs%s" % (total_t, (", FAILED: " + ", ".join(failed)) if failed else ""))
    return 1 if failed else 0


if __name__ == "__main__":
    sys.exit(main())
