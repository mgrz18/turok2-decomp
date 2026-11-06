#!/usr/bin/env python3
"""List every CCache request call site in src/us/code with its description string.

CCache__Request(0, key, size, mode, owner, decompressCallback, description, direct)
CCache__RequestDecompression(0, key, mem, mem2, mode, owner, callback, description, async)
CCache__LoadPersistantIndex(0, &out, key, description)
CCache__LoadPersistantSubBlock(0, set, index, setKey, owner, callback, description)

Descriptions given as D_800Axxxx symbols are resolved from the engine .rodata
in the ROM (ROM = VRAM - 0x80000000 + 0xC00 for the engine's .rodata).

CLI: python tools/assets/cache_calls.py baserom.us.z64 [--src src/us/code] [--markdown]
"""
import glob
import os
import re
import sys

PAT = re.compile(r"\b(CCache__Request|CCache__RequestDecompression|"
                 r"CCache__LoadPersistantIndex|CCache__LoadPersistantSubBlock)\s*\(")
FUNC = re.compile(r"^[A-Za-z_][\w \*]*?\b(\w+)\s*\([^;{]*\)\s*\{", re.M)
ARGPOS = {"CCache__Request": (8, 6, 5),
          "CCache__RequestDecompression": (9, 7, 6),
          "CCache__LoadPersistantIndex": (4, 3, None),
          "CCache__LoadPersistantSubBlock": (7, 6, 5)}


def split_args(s):
    parts, depth, cur = [], 0, ""
    for c in s:
        if c in "([":
            depth += 1
        elif c in ")]":
            depth -= 1
        if c == "," and depth == 0:
            parts.append(cur.strip())
            cur = ""
        else:
            cur += c
    parts.append(cur.strip())
    return [re.sub(r"\s+", " ", p) for p in parts]


def rodata_string(rom, vram):
    o = vram - 0x80000000 + 0xC00
    if not 0 <= o < len(rom):
        return None
    e = rom.index(b"\0", o)
    s = rom[o:e]
    return s.decode("latin1") if s and all(32 <= b < 127 for b in s) else None


def scan(rom, srcdir):
    rows = []
    for f in sorted(glob.glob(os.path.join(srcdir, "*.c"))):
        src = open(f, encoding="latin1").read()
        funcs = [(m.start(), m.group(1)) for m in FUNC.finditer(src)]
        for m in PAT.finditer(src):
            i = j = m.end()
            depth = 1
            while depth and j < len(src):
                depth += (src[j] == "(") - (src[j] == ")")
                j += 1
            parts = split_args(src[i:j - 1])
            fn = m.group(1)
            n, di, ci = ARGPOS[fn]
            if len(parts) != n or re.match(r"^(const )?(s32|u32|void|char|int)\b", parts[0]):
                continue  # prototype
            owner = [nm for p, nm in funcs if p < m.start()]
            owner = owner[-1] if owner else "?"
            if owner in ("CCache__LoadPersistantIndex", "CCache__LoadPersistantSubBlock"):
                continue  # the helpers themselves
            desc = parts[di]
            mm = re.search(r"D_(800A[0-9A-F]{4})(?:\s*\+\s*(\d+))?", desc)
            if mm:
                s = rodata_string(rom, int(mm.group(1), 16) + int(mm.group(2) or 0))
                if s is not None:
                    desc = '"%s"' % s
            cb = parts[ci] if ci is not None else "-"
            cb = cb.lstrip("&").replace("(s32) ", "")
            if cb in ("0", "NULL"):
                cb = "-"
            rows.append(dict(file=os.path.basename(f), func=owner, call=fn[len("CCache__"):],
                             desc=desc, callback=cb, key=parts[1]))
    return rows


def main(argv=None):
    import argparse
    ap = argparse.ArgumentParser()
    ap.add_argument("rom")
    ap.add_argument("--src", default="src/us/code")
    ap.add_argument("--markdown", action="store_true")
    a = ap.parse_args(argv)
    rom = open(a.rom, "rb").read()
    rows = scan(rom, a.src)
    if a.markdown:
        print("| description | call | decompress callback | caller | file |")
        print("|---|---|---|---|---|")
        for r in sorted(rows, key=lambda r: (r["desc"].lower(), r["func"])):
            print("| %s | %s | %s | %s | %s |" % (r["desc"], r["call"], r["callback"], r["func"], r["file"]))
    else:
        for r in rows:
            print("\t".join(r[k] for k in ("desc", "call", "callback", "func", "file", "key")))


if __name__ == "__main__":
    main()
