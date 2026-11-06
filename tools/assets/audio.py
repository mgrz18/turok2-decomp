#!/usr/bin/env python3
"""Turok 2 (N64, US) audio extractor.

    python tools/assets/audio.py baserom.us.z64 --out DIR [--no-wav] [--no-streams] [--only-stats]

Writes (never commit the output):
    DIR/banks.json              the two libaudio banks (music, sfx): instruments, sounds, envelopes,
                                key maps, wave tables (ROM offsets, loops, ADPCM books)
    DIR/music_waves/*.wav       VADPCM-decoded samples of the music bank (22050 Hz = pitch 1.0)
    DIR/sfx_waves/*.wav         VADPCM-decoded samples of the sfx bank (22050 Hz = pitch 1.0)
    DIR/sequences/*.mid         the alCSeq (compressed MIDI) songs as standard MIDI files
    DIR/streams/*.wav           the Acclaim "adec" MDCT streams (speech / ambience / jingles)
    DIR/soundtree.json          the sound tree file (sfx effect table, envelopes, song table)
    DIR/stats.json              listen-free checks (duration, peak, RMS, clipping, zero crossings)

Formats and how they were established: docs/assets/AUDIO.md.

LOCATOR NOTE: the shared ROM-filesystem module (tools/assets/romfs.py, part a1) was not available
when this was written, so the few ROM addresses this module needs are constants below, taken from
CAudioWorld__Initialize / CAudioWorld__LoadSfxDataFile / __amDMA. Parsing never uses them directly:
every parser takes (rom bytes, offset) so a romfs-based locator can be swapped in.
"""
import argparse
import json
import math
import os
import struct
import sys

# --------------------------------------------------------------------------------------------
# Locator (temporary, see LOCATOR NOTE)
# --------------------------------------------------------------------------------------------
# CAudioWorld__Initialize: VM_Alloc(0x2F194, 0, 0x1E0000) maps the "ctl region" (two bank files);
# its pointers are pre-relocated to the VM address the pager gives it, 0x00480000.
CTL_ROM = 0x1E0000
CTL_SIZE = 0x2F194
CTL_VADDR = 0x480000
# CAudioWorld__Initialize: gAudioRomBase = 0x20F194; __amDMA reads ROM offset + gAudioRomBase.
TBL_ROM = 0x20F194
# CAudioWorld__LoadSfxDataFile: VM_Alloc(0x3EA7A0, 0, 0xBE1814) -> the sound tree file.
TREE_ROM = 0xBE1814
TREE_SIZE = 0x3EA7A0
# The adec decoder's tables live in the code's .rodata (us/asm/data/ABCA8.rodata.s).
CODE_VRAM_DELTA = 0x7FFFF400          # vram - rom for that segment
ADEC_SEGMENTS_VRAM = 0x800B8C54       # gAdecHuffSegments
ADEC_BANDENDS_VRAM = 0x800B8A88       # gAdecBandEnds (u16[16])
ADEC_STEPS_VRAM = 0x800B8AA8          # gAdecStepSizes (f32[64])

# Same values as tools/assets/romfs.py (part a1); take them from there when it is present.
try:
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    import romfs as _romfs
    CTL_ROM, CTL_SIZE, CTL_VADDR = _romfs.AUDIO_CTL, _romfs.AUDIO_CTL_SIZE, _romfs.AUDIO_CTL_VRAM
    TBL_ROM, TREE_ROM, TREE_SIZE = _romfs.AUDIO_TBL, _romfs.SOUND_DATA, _romfs.SOUND_DATA_SIZE
except (ImportError, AttributeError):
    pass

OUTPUT_RATE = 22050                   # ai.rate = 0x5622 in CAudioWorld__Initialize


def u8(r, a): return r[a]
def s8(r, a): return struct.unpack_from(">b", r, a)[0]
def u16(r, a): return struct.unpack_from(">H", r, a)[0]
def s16(r, a): return struct.unpack_from(">h", r, a)[0]
def u32(r, a): return struct.unpack_from(">I", r, a)[0]
def s32(r, a): return struct.unpack_from(">i", r, a)[0]
def f32(r, a): return struct.unpack_from(">f", r, a)[0]


def vrom(p):
    """ctl-region VM pointer -> ROM offset"""
    if not (CTL_VADDR <= p < CTL_VADDR + CTL_SIZE):
        raise ValueError("pointer %08X outside the ctl region" % p)
    return p - CTL_VADDR + CTL_ROM


def cset(r, a):
    """CISet: s32 count; s32 offs[count+1] (relative to the set; the last is the size).
    Returns [(rom, size)] (CISet__GetBlock / CISet__GetBlockAndSize)."""
    n = u32(r, a)
    offs = [u32(r, a + 4 + 4 * i) for i in range(n + 1)]
    return [(a + offs[i], offs[i + 1] - offs[i]) for i in range(n)]


# --------------------------------------------------------------------------------------------
# Output helpers: WAV, MIDI
# --------------------------------------------------------------------------------------------
def write_wav(path, samples, rate, loop=None):
    data = struct.pack("<%dh" % len(samples), *samples)
    chunks = [b"fmt " + struct.pack("<IHHIIHH", 16, 1, 1, rate, rate * 2, 2, 16),
              b"data" + struct.pack("<I", len(data)) + data]
    if loop is not None:
        start, end, count = loop
        smpl = struct.pack("<9I", 0, 0, int(1e9 / rate), 60, 0, 0, 0, 1, 0)
        smpl += struct.pack("<6I", 0, 0, start, max(start, end - 1), 0,
                            0 if count == 0xFFFFFFFF else count)
        chunks.append(b"smpl" + struct.pack("<I", len(smpl)) + smpl)
    body = b"WAVE" + b"".join(chunks)
    with open(path, "wb") as f:
        f.write(b"RIFF" + struct.pack("<I", len(body)) + body)


def wave_stats(s, rate):
    n = len(s)
    if n == 0:
        return {"samples": 0}
    peak = max(abs(x) for x in s)
    rms = math.sqrt(sum(x * x for x in s) / n)
    clip = sum(1 for x in s if x >= 32767 or x <= -32768)
    zc = sum(1 for i in range(1, n) if (s[i - 1] < 0) != (s[i] < 0))
    return {"samples": n, "seconds": round(n / rate, 3), "peak": peak, "rms": round(rms, 1),
            "clip_pct": round(100.0 * clip / n, 3), "zc_per_s": round(zc * rate / n, 1)}


def varlen(v):
    out = [v & 0x7F]
    v >>= 7
    while v:
        out.append(0x80 | (v & 0x7F))
        v >>= 7
    return bytes(reversed(out))


# --------------------------------------------------------------------------------------------
# libaudio banks (.ctl) and VADPCM (.tbl)
# --------------------------------------------------------------------------------------------
def parse_wavetable(r, a):
    base, ln, typ, flags = struct.unpack_from(">IiBB", r, a)
    w = {"rom_struct": a, "base": base, "tbl_rom": TBL_ROM + base, "len": ln, "type": typ,
         "flags": flags, "loop": None, "book": None}
    lp, bk = u32(r, a + 12), u32(r, a + 16)
    if typ == 0:   # AL_ADPCM_WAVE
        if lp:
            la = vrom(lp)
            w["loop"] = {"start": u32(r, la), "end": u32(r, la + 4), "count": u32(r, la + 8),
                         "state": [s16(r, la + 12 + 2 * i) for i in range(16)]}
        ba = vrom(bk)
        order, npred = s32(r, ba), s32(r, ba + 4)
        w["book"] = {"order": order, "npredictors": npred,
                     "book": [s16(r, ba + 8 + 2 * i) for i in range(order * npred * 8)]}
    elif typ == 1 and lp:  # AL_RAW16_WAVE
        la = vrom(lp)
        w["loop"] = {"start": u32(r, la), "end": u32(r, la + 4), "count": u32(r, la + 8)}
    return w


def parse_sound(r, a):
    env, km, wt = vrom(u32(r, a)), vrom(u32(r, a + 4)), vrom(u32(r, a + 8))
    return {"rom": a,
            "envelope": {"attackTime": s32(r, env), "decayTime": s32(r, env + 4),
                         "releaseTime": s32(r, env + 8), "attackVolume": r[env + 12],
                         "decayVolume": r[env + 13]},
            "keymap": {"velocityMin": r[km], "velocityMax": r[km + 1], "keyMin": r[km + 2],
                       "keyMax": r[km + 3], "keyBase": r[km + 4], "detune": s8(r, km + 5)},
            "wave_rom": wt, "samplePan": r[a + 12], "sampleVolume": r[a + 13], "flags": r[a + 14]}


def parse_instrument(r, a):
    n = s16(r, a + 14)
    return {"rom": a, "volume": r[a], "pan": r[a + 1], "priority": r[a + 2], "flags": r[a + 3],
            "trem": list(r[a + 4:a + 8]), "vib": list(r[a + 8:a + 12]), "bendRange": s16(r, a + 12),
            "sound_ptrs": [u32(r, a + 16 + 4 * i) for i in range(n)]}


def parse_bankfile(r, a):
    rev, nb = s16(r, a), s16(r, a + 2)
    assert rev == 0x4231, "not a B1 bank file at %X" % a
    banks = []
    for b in range(nb):
        ba = vrom(u32(r, a + 4 + 4 * b))
        ic, fl, rate, perc = s16(r, ba), r[ba + 2], s32(r, ba + 4), u32(r, ba + 8)
        insts = [parse_instrument(r, vrom(u32(r, ba + 12 + 4 * i))) for i in range(ic)]
        banks.append({"rom": ba, "flags": fl, "sampleRate": rate,
                      "percussion": parse_instrument(r, vrom(perc)) if perc else None,
                      "instruments": insts})
    return {"rom": a, "revision": "B1", "banks": banks}


def parse_ctl(r):
    """The ctl region header: s32 n; {ALBankFile *file; s32 tblOffset}[n]."""
    n = u32(r, CTL_ROM)
    out = []
    for i in range(n):
        p, tbloff = u32(r, CTL_ROM + 4 + 8 * i), u32(r, CTL_ROM + 8 + 8 * i)
        bf = parse_bankfile(r, vrom(p))
        waves, wave_index = [], {}
        for bank in bf["banks"]:
            insts = bank["instruments"] + ([bank["percussion"]] if bank["percussion"] else [])
            for inst in insts:
                inst["sounds"] = []
                for sp in inst.pop("sound_ptrs"):
                    snd = parse_sound(r, vrom(sp))
                    snd["vaddr"] = sp
                    wr = snd.pop("wave_rom")
                    if wr not in wave_index:
                        wave_index[wr] = len(waves)
                        waves.append(parse_wavetable(r, wr))
                    snd["wave"] = wave_index[wr]
                    inst["sounds"].append(snd)
        out.append({"name": ("music", "sfx")[i] if n == 2 else "bank%d" % i,
                    "tbl_offset": tbloff, "file": bf, "waves": waves})
    return out


def adpcm_tables(book):
    order, npred, b = book["order"], book["npredictors"], book["book"]
    tables = []
    for p in range(npred):
        t = [[0] * (order + 8) for _ in range(8)]
        for j in range(order):
            for k in range(8):
                t[k][j] = b[p * order * 8 + j * 8 + k]
        for k in range(1, 8):
            t[k][order] = t[k - 1][order - 1]
        t[0][order] = 1 << 11
        for k in range(1, 8):
            for j in range(8):
                t[j][k + order] = 0 if j < k else t[j - k][order]
        tables.append(t)
    return tables


def vadpcm_decode(data, book):
    """N64 VADPCM: 9-byte frames (header: scale<<4 | predictor, 16 signed nibbles) -> 16 samples."""
    order = book["order"]
    tables = adpcm_tables(book)
    state = [0] * 16
    out = []
    for f in range(len(data) // 9):
        fr = data[f * 9:f * 9 + 9]
        scale = 1 << (fr[0] >> 4)
        tab = tables[min(fr[0] & 0xF, len(tables) - 1)]
        ix = []
        for c in fr[1:]:
            for nib in (c >> 4, c & 0xF):
                ix.append((nib - 16 if nib >= 8 else nib) * scale)
        for j in range(2):
            inv = [state[(16 if j == 0 else 8) - order + i] for i in range(order)]
            for i in range(8):
                ind = j * 8 + i
                inv.append(ix[ind])
                row = tab[i]
                acc = 0
                for k in range(order + i):
                    acc += row[k] * inv[k]
                v = (acc >> 11) + ix[ind]   # floor division by 2048, as inner_product()
                state[ind] = max(-32768, min(32767, v))
        out.extend(state)
    return out


# --------------------------------------------------------------------------------------------
# Sound tree (CAudioWorld__LoadSfxDataFile)
# --------------------------------------------------------------------------------------------
SFX_INFO_FIELDS = ("group", "sample", "priority", "chance", "flags", "unk8", "unkA", "unkC",
                   "pitch", "unkF", "unk10", "randomRange0", "randomRange1")


def table_hdr(r, a):
    """{u32 elemSize, u32 count, data[]} (m->ranges/weights/table2B7x = block + 8)."""
    return u32(r, a), u32(r, a + 4), a + 8


def parse_tree(r, a=TREE_ROM):
    top = cset(r, a)
    sfx = cset(r, top[0][0])
    tree = {"rom": a, "size": TREE_SIZE}
    # 0 ranges (effect ids, sorted, BinaryRange), 1 weights, 2 effects, 3/4/5 envelopes
    es, n, d = table_hdr(r, sfx[0][0])
    tree["effect_table0"] = [s32(r, d + 4 * i) for i in range(n)]   # m->ranges (all 10 in the US ROM)
    es, n, d = table_hdr(r, sfx[1][0])
    tree["effect_table1"] = [s32(r, d + 4 * i) for i in range(n)]   # m->weights
    effects = []
    for (ea, esz) in cset(r, sfx[2][0]):
        parts = cset(r, ea)
        cue_a, cue_sz = parts[0]
        lay_a = parts[1][0]
        stride, cnt = u32(r, lay_a), u32(r, lay_a + 4)
        layers = []
        for i in range(cnt):
            la = lay_a + 8 + i * stride
            vals = struct.unpack_from(">hhbbHhhhbbhhh", r, la)
            layers.append(dict(zip(SFX_INFO_FIELDS, vals)))
        effects.append({"rom": ea, "cue": r[cue_a:cue_a + cue_sz].hex(), "layer_stride": stride,
                        "layers": layers})
    tree["effects"] = effects
    for name, k in (("volume_envelopes", 3), ("pitch_envelopes", 4), ("pan_envelopes", 5)):
        es, n, d = table_hdr(r, sfx[k][0])
        tree[name] = [dict(zip(("level", "unk2", "target", "frames", "delay", "unkA"),
                               struct.unpack_from(">HHHhhH", r, d + es * i))) for i in range(n)]
    songs_set = cset(r, top[1][0])
    za = songs_set[0][0]
    keys = [s32(r, za + 8 + 4 * i) for i in range(u32(r, za + 4))]
    blocks = cset(r, songs_set[1][0])
    songs = []
    for i, key in enumerate(keys):
        ha, hs = blocks[2 * i]
        da, ds = blocks[2 * i + 1]
        kind = "cseq" if u32(r, da) == 0x44 else "adec"
        songs.append({"index": i, "key": key, "kind": kind, "header": r[ha:ha + hs].hex(),
                      "volume": s32(r, ha), "h4": u16(r, ha + 4), "h6": u16(r, ha + 6),
                      "rom": da, "size": ds})
    tree["songs"] = songs
    return tree


# --------------------------------------------------------------------------------------------
# alCSeq (compressed MIDI) -> SMF
# --------------------------------------------------------------------------------------------
class CSeqTrack:
    def __init__(self, data, pos):
        self.d, self.pos, self.bu, self.bulen = data, pos, 0, 0

    def byte(self):   # __getTrackByte
        if self.bulen:
            b = self.d[self.bu]
            self.bu += 1
            self.bulen -= 1
            return b
        b = self.d[self.pos]
        self.pos += 1
        if b == 0xFE:
            nb = self.d[self.pos]
            self.pos += 1
            if nb != 0xFE:
                back = (nb << 8) | self.d[self.pos]
                ln = self.d[self.pos + 1]
                self.pos += 2
                self.bu = self.pos - (back + 4)
                self.bulen = ln
                b = self.d[self.bu]
                self.bu += 1
                self.bulen -= 1
        return b

    def varlen(self):
        v = self.byte()
        if v & 0x80:
            v &= 0x7F
            while True:
                c = self.byte()
                v = (v << 7) + (c & 0x7F)
                if not c & 0x80:
                    break
        return v


def cseq_to_midi(data):
    """Returns (smf bytes, info). One SMF track per valid cseq track (format 1)."""
    offs = [u32(data, 4 * i) for i in range(16)]
    division = u32(data, 64)
    tracks, info = [], {"division": division, "tracks": 0, "notes": 0, "loops": 0}
    for t in range(16):
        if not offs[t]:
            continue
        tr = CSeqTrack(data, offs[t])
        ev = []   # (abs tick, order, bytes)
        tick = tr.varlen()
        last = 0
        seq = 0
        for _ in range(200000):
            st = tr.byte()
            if st == 0xFF:
                typ = tr.byte()
                if typ == 0x51:
                    ev.append((tick, seq, b"\xFF\x51\x03" + bytes(tr.byte() for _ in range(3))))
                    last = 0
                elif typ == 0x2F:
                    break
                elif typ == 0x2E:   # loop start, 2 bytes
                    a, b = tr.byte(), tr.byte()
                    ev.append((tick, seq, b"\xFF\x06" + varlen(9) + b"loopStart"))
                    info["loops"] += 1
                    last = 0
                elif typ == 0x2D:   # loop end: raw loopCt, curLpCt, s32 offset (read off curLoc)
                    cnt = data[tr.pos]
                    tr.pos += 6
                    txt = b"loopEnd %d" % cnt
                    ev.append((tick, seq, b"\xFF\x06" + varlen(len(txt)) + txt))
                    last = 0
                else:
                    raise ValueError("unknown cseq meta %02X" % typ)
            else:
                if st & 0x80:
                    status, b1 = st, tr.byte()
                    last = st
                else:
                    status, b1 = last, st
                hi = status & 0xF0
                if hi in (0xC0, 0xD0):
                    ev.append((tick, seq, bytes((status, b1))))
                else:
                    b2 = tr.byte()
                    if hi == 0x90:
                        dur = tr.varlen()
                        ev.append((tick, seq, bytes((status, b1, b2))))
                        ev.append((tick + dur, -1, bytes((0x80 | (status & 0xF), b1, 0))))
                        info["notes"] += 1
                    else:
                        ev.append((tick, seq, bytes((status, b1, b2))))
            seq += 1
            tick += tr.varlen()
        ev.sort(key=lambda e: (e[0], e[1] >= 0, e[1]))   # note-offs first at a tick
        body, prev = b"", 0
        for tk, _, b in ev:
            body += varlen(tk - prev) + b
            prev = tk
        body += b"\x00\xFF\x2F\x00"
        tracks.append(b"MTrk" + struct.pack(">I", len(body)) + body)
        info["tracks"] += 1
        info["length_ticks"] = max(info.get("length_ticks", 0), prev)
    smf = b"MThd" + struct.pack(">IHHH", 6, 1, len(tracks), division) + b"".join(tracks)
    return smf, info


# --------------------------------------------------------------------------------------------
# Acclaim "adec" MDCT streams (adecStreamOpen / adecNextFrame / adecDecodeFrame /
# adecHuffDecode / adecDequantize / adecImdctFrame)
# --------------------------------------------------------------------------------------------
class AdecTables:
    def __init__(self, r):
        rom = lambda v: v - CODE_VRAM_DELTA
        self.r = r
        self.band_ends = [u16(r, rom(ADEC_BANDENDS_VRAM) + 2 * i) for i in range(16)]
        self.steps = [f32(r, rom(ADEC_STEPS_VRAM) + 4 * i) for i in range(64)]
        assert self.band_ends[-1] == 0x100 and self.steps[-1] == 1.0, "adec tables not found"
        segs, a, covered = [], rom(ADEC_SEGMENTS_VRAM), 0
        while True:
            kind, start, ln, pos, tab = struct.unpack_from(">iiiiI", r, a)
            t = rom(tab)
            segs.append({"kind": kind, "start": start, "len": ln, "width": u16(r, t + 0xE),
                         "phrases": rom(u32(r, t + 0x1C)) if u32(r, t + 0x1C) else 0,
                         "tree": rom(u32(r, t + 0x20))})
            a += 0x14
            if kind == 0:
                covered = start + ln
                if covered >= 256:
                    break
        self.segs = segs
        self.scale_segs = segs                          # decode starts at seg 0 (kind 1)
        self.coef_segs = [s for s in segs if s["kind"] == 0]
        self._phr = {}

    def phrase(self, seg, i):
        key = (seg["phrases"], i)
        if key not in self._phr:
            pa = seg["phrases"] + i * 0x1B8
            n = s32(self.r, pa + 0x1A0)
            self._phr[key] = [s16(self.r, pa + 0xA0 + 4 * j + 2) for j in range(n)]
        return self._phr[key]


class BitReader:
    def __init__(self, data, pos):
        self.d, self.bit = data, pos * 8

    def bits(self, n):
        v = 0
        for _ in range(n):
            byte = self.d[self.bit >> 3] if (self.bit >> 3) < len(self.d) else 0
            v = (v << 1) | ((byte >> (7 - (self.bit & 7))) & 1)
            self.bit += 1
        return v


def _huff(T, segs, n, br):
    out, b = [], 0
    r = T.r
    seg = segs[0]
    tree = seg["tree"]
    node = 0
    while len(out) < n:
        bit = br.bits(1)
        child = u16(r, tree + 4 * node + (2 if bit else 0))
        if u16(r, tree + 4 * child) != 0x800:
            node = child
            continue
        value = u16(r, tree + 4 * child + 2)
        node = 0
        m = value & 0xF00
        if m == 0 or m == 0xF00:
            out.append(value - 0x10000 if value & 0x8000 else value)
        elif value == 0x900:
            v = br.bits(seg["width"])
            out.append(v - 0x10000 if v & 0x8000 else v)
        elif m == 0xA00:
            out.extend(T.phrase(seg, value & 0xFF))
        elif m == 0xB00:
            out.extend([0] * (n - len(out)))
            return out
        if len(out) >= seg["start"] + seg["len"] and b + 1 < len(segs):
            b += 1
            seg = segs[b]
            tree = seg["tree"]
    return out[:n]


def _fft_tables(n=128):
    cos_t = [math.cos(i * 2 * math.pi / n) for i in range(n // 2)]
    sin_t = [-math.sin(i * 2 * math.pi / n) for i in range(n // 2)]
    order = list(range(n))
    j = 1
    for k in range(1, n * 2 + 1, 2):
        if k < j:
            order[(j - 1) // 2], order[(k - 1) // 2] = order[(k - 1) // 2], order[(j - 1) // 2]
        m = n
        while m >= 2 and j > m:
            j -= m
            m >>= 1
        j += m
    return cos_t, sin_t, order


def _fft(d, n, sign, cos_t, sin_t, bitrev):   # mdctFft, d = interleaved complex
    for i in range(n):
        j = bitrev[i]
        if i < j:
            d[2 * i], d[2 * j] = d[2 * j], d[2 * i]
            d[2 * i + 1], d[2 * j + 1] = d[2 * j + 1], d[2 * i + 1]
    dist, half, stride, nn = 2, 1, n >> 1, n << 1
    while stride > 0:
        w = 0
        for k in range(half):
            wc, ws = cos_t[w], sign * sin_t[w]
            for idx in range(k * 2, nn, dist * 2):
                xr, xi = d[idx + dist], d[idx + dist + 1]
                tr, ti = wc * xr - ws * xi, wc * xi + ws * xr
                d[idx + dist], d[idx + dist + 1] = d[idx] - tr, d[idx + 1] - ti
                d[idx], d[idx + 1] = d[idx] + tr, d[idx + 1] + ti
            w += stride
        stride >>= 1
        half <<= 1
        dist <<= 1


class AdecDecoder:
    def __init__(self, T):
        self.T = T
        pc = [math.cos((i * 4.0 + 1.0) * 0.0030679617) for i in range(128)]
        ps = [math.sin((i * 4.0 + 1.0) * 0.0030679617) for i in range(128)]
        self.pre = (pc, ps)
        self.post = ([math.cos(i * 0.012271847) + math.sin(i * 0.012271847) for i in range(128)],
                     [math.sin(i * 0.012271847) - math.cos(i * 0.012271847) for i in range(128)])
        self.fft = _fft_tables(128)

    @staticmethod
    def header(data):
        return {"rate": u32(data, 0), "frames": u32(data, 4), "bytes": u32(data, 8)}

    def decode(self, data):
        T = self.T
        h = self.header(data)
        br = BitReader(data, 12)
        prev, overlap, out = None, [0.0] * 128, []
        pc, ps = self.pre
        qc, qs = self.post
        for f in range(h["frames"]):
            if prev is None:
                band = [br.bits(6) for _ in range(16)]
            else:
                delta = _huff(T, T.scale_segs, 16, br)
                band = [(p + d) & 0xFFFF for p, d in zip(prev, delta)]
                band = [b - 0x10000 if b & 0x8000 else b for b in band]
            prev = band
            coef = _huff(T, T.coef_segs, 256, br)
            deq, start = [0.0] * 256, 0
            for b in range(16):
                step = T.steps[max(0, min(63, band[b]))]
                for i in range(start, T.band_ends[b]):
                    deq[i] = coef[i] / step * 3.0517578e-5
                start = T.band_ends[b]
            buf = [0.0] * 256
            for k in range(128):
                re, im = deq[2 * k], deq[255 - 2 * k]
                buf[2 * k] = re * pc[k] - im * ps[k]
                buf[2 * k + 1] = re * ps[k] + im * pc[k]
            _fft(buf, 128, -1, *self.fft)
            for k in range(128):
                re, im = buf[2 * k], buf[2 * k + 1]
                buf[2 * k] = re * qc[k] - im * qs[k]
                buf[2 * k + 1] = re * qs[k] + im * qc[k]
            fr = [0] * 256
            for k in range(128):
                ov, im = overlap[k], buf[2 * k + 1]
                t1 = ov * pc[k] + im * ps[k]
                t2 = ov * ps[k] - im * pc[k]
                overlap[k] = buf[2 * k]
                fr[2 * k] = _clamp(23170.475 * t1)
                fr[255 - 2 * k] = _clamp(23170.475 * t2)
            out.extend(fr)
        return out, h, (br.bit + 7) // 8 - 12


def _clamp(x):
    y = x + 0.5 if x >= 0 else x - 0.5
    return int(max(-32768.0, min(32767.0, y)))


# --------------------------------------------------------------------------------------------
# CLI
# --------------------------------------------------------------------------------------------
def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("rom")
    ap.add_argument("--out", required=True)
    ap.add_argument("--no-wav", action="store_true", help="skip bank sample WAVs")
    ap.add_argument("--no-streams", action="store_true", help="skip adec stream decoding (slow)")
    ap.add_argument("--only", type=int, default=None, help="decode only this song index (debug)")
    a = ap.parse_args()
    r = open(a.rom, "rb").read()
    os.makedirs(a.out, exist_ok=True)
    stats = {"music_waves": [], "sfx_waves": [], "streams": [], "sequences": []}

    ctl = parse_ctl(r)
    for b in ctl:
        if not a.no_wav and a.only is None:
            d = os.path.join(a.out, b["name"] + "_waves")
            os.makedirs(d, exist_ok=True)
            for i, w in enumerate(b["waves"]):
                pcm = vadpcm_decode(r[w["tbl_rom"]:w["tbl_rom"] + w["len"]], w["book"])
                lp = w["loop"]
                loop = (lp["start"], lp["end"], lp["count"]) if lp and lp["end"] > lp["start"] else None
                write_wav(os.path.join(d, "%s_%03d_%07X.wav" % (b["name"], i, w["tbl_rom"])),
                          pcm, OUTPUT_RATE, loop)
                st = wave_stats(pcm, OUTPUT_RATE)
                st["wave"] = i
                stats[b["name"] + "_waves"].append(st)
    # the game sorts the sfx instrument's soundArray by pointer at start-up (CAudioWorld__Initialize):
    sfx = ctl[1]["file"]["banks"][0]["instruments"][0]
    order = sorted(range(len(sfx["sounds"])), key=lambda i: sfx["sounds"][i]["vaddr"])
    for runtime_idx, rom_idx in enumerate(order):
        sfx["sounds"][rom_idx]["runtime_index"] = runtime_idx
    with open(os.path.join(a.out, "banks.json"), "w") as f:
        json.dump(ctl, f, indent=1)

    tree = parse_tree(r)
    with open(os.path.join(a.out, "soundtree.json"), "w") as f:
        json.dump(tree, f, indent=1)

    os.makedirs(os.path.join(a.out, "sequences"), exist_ok=True)
    os.makedirs(os.path.join(a.out, "streams"), exist_ok=True)
    dec = AdecDecoder(AdecTables(r))
    for s in tree["songs"]:
        if a.only is not None and s["index"] != a.only:
            continue
        data = r[s["rom"]:s["rom"] + s["size"]]
        if s["kind"] == "cseq":
            smf, info = cseq_to_midi(data)
            with open(os.path.join(a.out, "sequences", "song_%03d_key%03d.mid" % (s["index"], s["key"])), "wb") as f:
                f.write(smf)
            info.update(index=s["index"], key=s["key"])
            stats["sequences"].append(info)
        elif not a.no_streams:
            pcm, h, used = dec.decode(data)
            write_wav(os.path.join(a.out, "streams", "stream_%03d_key%03d.wav" % (s["index"], s["key"])),
                      pcm, h["rate"])
            st = wave_stats(pcm, h["rate"])
            st.update(index=s["index"], key=s["key"], rate=h["rate"], frames=h["frames"],
                      hdr_bytes=h["bytes"], bytes_consumed=used)
            stats["streams"].append(st)
            print("stream %d key %d: %s" % (s["index"], s["key"], st), file=sys.stderr)
    with open(os.path.join(a.out, "stats.json"), "w") as f:
        json.dump(stats, f, indent=1)
    print("banks: %s; songs: %d cseq, %d adec" % (
        ", ".join("%s %d sounds / %d waves" % (b["name"], sum(len(i["sounds"]) for bk in b["file"]["banks"] for i in bk["instruments"]), len(b["waves"])) for b in ctl),
        sum(s["kind"] == "cseq" for s in tree["songs"]), sum(s["kind"] == "adec" for s in tree["songs"])))


if __name__ == "__main__":
    main()
