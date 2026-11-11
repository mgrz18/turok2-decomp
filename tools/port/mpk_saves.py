#!/usr/bin/env python3
"""List the Turok 2 notes of a Controller Pak image (.mpk, 32 KB, the console's byte order) and
check them the way the console reads them (docs/port/INPUT.md section 7).

    python3 tools/port/mpk_saves.py turok2_pak1.mpk [--dump-save out.bin]

For every note: game/company code, name, size. A "TUROK 2" save game (structs/save.h) is checked
big-endian: the size words, the Crc32_Compute word over the rest (seed 0xB: zlib's crc32 with that
start value), the level-flag ISet headers; then the player's health, lives, warps and play clock are
printed. A "T2 MULTI" profile (structs/pak.h): its checksum (seed 4) and name. A note that only
checks in little-endian order was written by a port build from before round 8 (host order).
Exit status 1 if a Turok 2 note does not check big-endian.
"""
import argparse
import struct
import sys
import zlib

PAK_CHARS = ("\0" + " " * 14 + " " + "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ!\"#'*+,-./:=?@")


def crc(data, seed):
    return zlib.crc32(data, seed) & 0xFFFFFFFF


def text(b):
    """Pak font codes to text, up to the first 0 (the game copies 16 bytes past its string's end)."""
    b = bytes(b).split(b"\0", 1)[0]
    return "".join(PAK_CHARS[c] if c < len(PAK_CHARS) else "~" for c in b).rstrip(" ")


def note_data(pak, start_page):
    """The note's pages, following the inode table (page 1, 1-bank pak)."""
    out = bytearray()
    page = start_page
    seen = 0
    while 5 <= page < 128 and seen < 128:
        out += pak[page * 256:(page + 1) * 256]
        nxt = struct.unpack_from(">H", pak, 0x100 + 2 * page)[0] & 0xFF
        page = nxt
        seen += 1
    return bytes(out)


KIDS = {"top": ["levels", "section"], "levels": "level", "level": "section", "section": None}


def walk_iset(b, o, size, kind="top"):
    """Check the persistent bit sections' ISets (swap_spec.json "load/save persistent bit sections":
    top = [ISet of levels, section], level = ISet of sections, section = [bit count, bytes]); an ISet
    is u32 n, u32 offsets[n + 1]. Returns the number of ISets."""
    if size < 8:
        raise ValueError("ISet of %d bytes at 0x%X" % (size, o))
    n = struct.unpack_from(">I", b, o)[0]
    if n > 0x10000 or 8 + 4 * n > size:
        raise ValueError("bad ISet count %d at 0x%X" % (n, o))
    offs = struct.unpack_from(">%dI" % (n + 1), b, o + 4)
    count = 1
    kids = KIDS[kind]
    for i in range(n):
        if offs[i] < 8 + 4 * n or offs[i + 1] < offs[i] or offs[i + 1] > size:
            raise ValueError("bad ISet offsets at 0x%X" % o)
        sub = kids[i] if isinstance(kids, list) and i < len(kids) else kids if isinstance(kids, str) else None
        if sub is not None and offs[i + 1] > offs[i]:
            count += walk_iset(b, o + offs[i], offs[i + 1] - offs[i], sub)
    return count


def check_save(d, out):
    total, flags = struct.unpack_from(">II", d, 0)
    order = ">"
    if not (0x688 <= total <= len(d) and flags == total - 0x688):
        total, flags = struct.unpack_from("<II", d, 0)
        order = "<"
        if not (0x688 <= total <= len(d) and flags == total - 0x688):
            return "not a save (size words)", False
    stored = struct.unpack_from(order + "I", d, total - 4)[0]
    ok = crc(d[:total - 4], 0xB) == stored
    frames, secs, mins, hours = struct.unpack_from(order + "4f", d, 8)
    health, = struct.unpack_from(order + "i", d, 0x18 + 4)
    life, lives = struct.unpack_from(order + "hh", d, 0x18 + 8)
    cp, sp = struct.unpack_from(order + "ii", d, 0x18 + 0x40C)
    weapon, = struct.unpack_from(order + "h", d, 0x18 + 0x476)
    diff, = struct.unpack_from(order + "I", d, 0x500 + 8)
    msg = "%s-endian, %d bytes, checksum %s; health %d lives %d life force %d weapon %d difficulty %d " \
          "checkpoint warp %d save warp %d clock %d:%02d:%02d" % (
              "big" if order == ">" else "LITTLE (host order: an old port save)", total,
              "OK" if ok else "BAD", health >> 8, lives, life, weapon, diff, cp, sp, hours, mins, secs)
    if order == ">" and ok:
        try:
            msg += "; level flags: %d ISets" % walk_iset(d, 0x684, flags)
        except ValueError as e:
            msg += "; level flags: %s" % e
            ok = False
    if out:
        open(out, "wb").write(d[:total])
    return msg, ok and order == ">"


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("mpk")
    ap.add_argument("--dump-save", help="write the first save game's bytes here")
    a = ap.parse_args()
    pak = open(a.mpk, "rb").read()
    if len(pak) < 0x8000:
        sys.exit("%s: %d bytes, not a 32 KB pak image" % (a.mpk, len(pak)))
    bad = 0
    for i in range(16):
        e = pak[0x300 + 32 * i:0x320 + 32 * i]
        game, company, start = struct.unpack_from(">IHH", e, 0)
        if game == 0:
            continue
        name = text(e[16:32])
        ext = text(e[12:13])
        d = note_data(pak, start)
        gc = struct.pack(">I", game).decode("latin-1")
        cc = struct.pack(">H", company).decode("latin-1")
        line = "note %2d: %s %s \"%s\" ext \"%s\" %d pages" % (i + 1, gc, cc, name, ext, len(d) // 256)
        if gc == "NT2E" and name == "TUROK 2":
            msg, ok = check_save(d, a.dump_save)
            line += ": " + msg
            bad += not ok
        elif gc == "NT2E":
            ok = crc(d[:0x14], 4) == struct.unpack_from(">I", d, 0x14)[0]
            n = struct.unpack_from(">4H", d, 0)
            line += ": profile checksum %s, suicides/deaths/kills/tags %s" % ("OK" if ok else "BAD", n)
            bad += not ok
        print(line)
    sys.exit(1 if bad else 0)


if __name__ == "__main__":
    main()
