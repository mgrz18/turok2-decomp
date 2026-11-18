#!/usr/bin/env python3
"""Compare the port's ABI1 interpreter with the reference VADPCM decoder.

    python tools/port/audio_compare.py baserom.us.z64 K left.raw

left.raw is `t2audio_test baserom.us.z64 --direct K left.raw` (the left channel, host s16): sfx
bank instrument 0 sound K played at pitch 1.0 and full volume through A_LOADADPCM, A_LOADBUFF,
A_ADPCM, A_RESAMPLE, A_ENVMIXER, A_INTERLEAVE, A_SAVEBUFF (docs/port/AUDIO.md §6). It must
equal tools/assets/audio.py's vadpcm_decode of the same wave through the microcode's resample filter
at phase 0 (round 20 i5: the ROM's table at 0xC0C10, taps on the 3 history samples and the current
one), within the envelope mixer's Q15 rounding (two gain stages of 0x7FFF/0x8000).
"""
import os
import struct
import sys

sys.path.insert(0, os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "assets"))
import audio  # noqa: E402

DELAY = 3
LUT_ROM = 0xC0C10  # aspMain's data + 0xC0: 64 phases x 4 Q15 taps (t2asp.c)


def main():
    rom = open(sys.argv[1], "rb").read()
    k = int(sys.argv[2])
    raw = open(sys.argv[3], "rb").read()
    got = struct.unpack("<%dh" % (len(raw) // 2), raw)
    sfx = audio.parse_ctl(rom)[1]
    snd = sfx["file"]["banks"][0]["instruments"][0]["sounds"][k]
    w = sfx["waves"][snd["wave"]]
    ref = audio.vadpcm_decode(rom[w["tbl_rom"]:w["tbl_rom"] + w["len"]], w["book"])
    lut = struct.unpack(">4h", rom[LUT_ROM:LUT_ROM + 8])  # phase 0 (pitch 1.0)
    hist = [0, 0, 0, 0] + list(ref) + [0, 0, 0]
    n = min(len(ref) - 2, len(got) - DELAY)  # the last output needs input i + 2
    worst = 0
    for i in range(n):
        # output i + DELAY = taps on input i - 1 .. i + 2 (the main tap on input i)
        x = sum(hist[i + 3 + t] * lut[t] for t in range(4)) >> 15
        x = max(-32768, min(32767, x))
        want = (((x * 0x7FFF) >> 15) * 0x7FFF) >> 15
        worst = max(worst, abs(got[i + DELAY] - want))
    peak = max(abs(v) for v in ref[:n]) if n else 0
    ok = n > 0 and worst <= 1
    print("sound %d: wave ROM %X, %d samples compared, reference peak %d, max difference %d: %s"
          % (k, w["tbl_rom"], n, peak, worst, "ok" if ok else "MISMATCH"))
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
