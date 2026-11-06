#!/usr/bin/env python3
"""Identify a Turok 2 ROM image and list the asset region's load roots.

The port ships no game data: the extractor (and the port itself) start from
the player's own ROM.  This module is the gate in front of that:

  * normalize(data)  -> big-endian (.z64) bytes from a .z64 / .v64 / .n64 image
  * identify(data)   -> (z64 bytes, sha1, KNOWN entry or None)
  * ROOTS            -> the ROM ranges the engine starts reading assets from,
                        with the C code that hard-codes each one
                        (docs/assets/PORT.md, "Where the engine reads assets")
  * cartridge_table  -> the 21 children of the cartridge index

CLI:  python tools/assets/romid.py ROM [--out DIR]
      prints the identification and the roots; with --out writes
      DIR/romid.json (offsets and hashes only, no ROM bytes).
"""
import argparse
import hashlib
import json
import os
import struct
import sys

# The only release the decomp builds (Makefile EXPECTED_SHA1, versions/turok2.us.yaml).
SUPPORTED_SHA1 = "fb0400f21e3f043939ab56500c7b12a3231006f1"
KNOWN = {
    SUPPORTED_SHA1: {"name": "Turok 2: Seeds of Evil (USA)", "supported": True},
}

ROM_SIZE = 0x2000000
ASSET_REGION = (0x20F178, 0x1FF7AEC)  # docs/SEGMENTS.md

# Where the engine starts reading assets.  Every other asset address is computed
# from these through the sets: CISet__GetROMAddress(table, base, i) = base + table[1 + i].
ROOTS = [
    # (name, rom start, rom end, how the C reaches it)
    ("audio_window", 0x1E0000, 0x20F194,
     "CAudioWorld__Initialize: VM_Alloc(0x2F194, 0, 0x1E0000) (bank file, sequence data)"),
    ("audio_data", 0x20F194, 0xBE1814,
     "gAudioRomBase = 0x20F194; read by the audio DMA callback (func_002700D0) via CLoader__LoadAsync"),
    ("sound_data", 0xBE1814, 0xFCBFB4,
     "CAudioWorld__LoadSfxDataFile: VM_Alloc(0x3EA7A0, 0, 0xBE1814)"),
    ("cartridge", 0xFCBFB4, 0x1FF7AEC,
     "m_CartridgeIndex = 0xFCBFB4 (func_00286674) -> CScene__Construct, 'cartridge index'"),
]

# Children of the cartridge index, as CScene__Construct (func_00429FA0.c) uses them.
CARTRIDGE_CHILDREN = {
    0: "objects index", 1: "intelligence", 2: "object types", 3: "texture sets index",
    4: "particle effects", 5: "levels index",
    6: "load/save persistent defaults (CLoader__LoadSync)",
    7: "game persistent defaults (CLoader__LoadSync)",
    8: "warp dests (func_0042C194 / func_0042C560, sub-block 8)", 9: "binaries index", 10: "object types 2",
    11: "particle swooshes", 12: "dynamic lights", 13: "curves", 14: "rumbles",
    15: "rumble types", 16: "graphicset index", 17: "graphicset type",
    18: "Cinemas index", 19: "link group types", 20: "(empty)",
}


def normalize(data):
    """The image in big-endian (.z64) byte order, or None if it is not an N64 ROM."""
    head = bytes(data[:4])
    if head == b"\x80\x37\x12\x40":
        return bytes(data)
    b = bytearray(data)
    if head == b"\x37\x80\x40\x12":  # .v64: every 16-bit half swapped
        b[0::2], b[1::2] = data[1::2], data[0::2]
        return bytes(b)
    if head == b"\x40\x12\x37\x80":  # .n64: little-endian 32-bit words
        for i in range(4):
            b[i::4] = data[3 - i::4]
        return bytes(b)
    return None


def identify(data):
    z64 = normalize(data)
    if z64 is None:
        return None, None, None
    sha1 = hashlib.sha1(z64).hexdigest()
    return z64, sha1, KNOWN.get(sha1)


def u32(b, o):
    return struct.unpack_from(">I", b, o)[0]


def cartridge_table(rom, base=0xFCBFB4):
    """The cartridge index: a CISet (u32 count, count + 1 offsets from base)."""
    n = u32(rom, base)
    offs = [u32(rom, base + 4 + 4 * i) for i in range(n + 1)]
    return [{"index": i, "rom": base + offs[i], "size": offs[i + 1] - offs[i],
             "rnc": rom[base + offs[i]:base + offs[i] + 3] == b"RNC",
             "what": CARTRIDGE_CHILDREN.get(i, "?")} for i in range(n)]


def main(argv=None):
    ap = argparse.ArgumentParser(description="Identify a Turok 2 ROM and list the asset roots.")
    ap.add_argument("rom")
    ap.add_argument("--out", help="directory for romid.json")
    a = ap.parse_args(argv)
    with open(a.rom, "rb") as f:
        rom, sha1, known = identify(f.read())
    if rom is None:
        print("not an N64 ROM image")
        return 2
    print("sha1", sha1, "->", known["name"] if known else "unknown image")
    if not known or not known["supported"]:
        print("unsupported image: the decomp is built against", SUPPORTED_SHA1)
        return 1
    for name, s, e, how in ROOTS:
        print("%-13s 0x%07X-0x%07X %9d  %s" % (name, s, e, e - s, how))
    cart = cartridge_table(rom)
    for c in cart:
        print("  cart[%2d] 0x%07X %8d %s %s" % (c["index"], c["rom"], c["size"],
                                             "RNC" if c["rnc"] else "   ", c["what"]))
    if a.out:
        os.makedirs(a.out, exist_ok=True)
        with open(os.path.join(a.out, "romid.json"), "w") as f:
            json.dump({"sha1": sha1,
                       "roots": [dict(zip(("name", "start", "end", "how"), r)) for r in ROOTS],
                       "cartridge": cart}, f, indent=1)
    return 0


if __name__ == "__main__":
    sys.exit(main())
