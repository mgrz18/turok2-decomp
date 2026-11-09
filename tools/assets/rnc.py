#!/usr/bin/env python3
"""RNC ProPack decompressor (methods 1 and 2) for Turok 2's assets.

Original code for this repository (same license as the repo). It was written from Turok 2's
own routines, not from any other RNC implementation:

    ProPackDecompress     src/us/code/func_00200B00.c   header test, picks the method
    RNC_UnpackMethod1     us/asm/1840.s (0x200C40)      + func_00200CB0 (main loop),
                          func_00200E1C (Huffman symbol), func_00200EC0/EE8 (bit reader),
                          func_00200F20 (table build)
    RNC_UnpackMethod2     us/asm/1840.s (0x2010C0)      inner entry at +0xA8

plus the publicly documented RNC header layout and its CRC-16 (reflected 0xA001, init 0).

Stream = 18-byte header + packed data:
    0x00 'RNC'    0x03 method (1 or 2)    0x04 u32 unpacked size    0x08 u32 packed size
    0x0C u16 CRC of the unpacked bytes    0x0E u16 CRC of the packed data (bytes 0x12..)
    0x10 u8 leeway                        0x11 u8 chunk count            (all big-endian)

Method 1 (Huffman + LZ, 16-bit little-endian words, bits taken from the low end):
    2 flag bits; then per chunk: three code tables (literal-run lengths, match distances,
    match lengths), each = 5-bit count + count 4-bit code lengths, canonical codes assigned
    from the shortest length up and stored bit-reversed; a 16-bit sub-chunk count n; then
    n x (literal run) and n-1 x (match) interleaved: run, match, run, ..., run. A symbol s
    < 2 is the value itself, otherwise (1 << (s-1)) | (s-1 more bits). A match copies
    length+2 bytes from out - distance - 1. Literal bytes are read straight from the input
    at the reader's look-ahead word, after which the look-ahead is reloaded (the routine
    keeps 16 + k bits: k left-over bits below the next whole word). Chunks repeat until
    the input pointer reaches the end of the packed data.

Method 2 (bit-per-flag LZ, one byte of flags with a sentinel bit, MSB first):
    2 flag bits; 0 = literal byte; 1 = match, length code:
        10 -> 2 (distance = one byte, no high part)      110 -> 3
        111 + byte n -> n + 8; n == 0 ends a chunk: one more bit, 1 = next chunk, 0 = end
        0ab -> 4 + a (b == 0), else 2*(3 + a) + c; that value 9 means a literal run of
               4 * (4-bit x + 3) bytes instead of a match
    distance = (high << 8 | next byte) + 1, high from a short bit code (0..15).

Python API:
    HEADER_LEN, MAGIC, RncError
    is_rnc(buf, off=0)                 'RNC' + method 1 or 2 and a whole header present
    header(buf, off=0) -> dict(method, unpacked, packed, crc_unpacked, crc_packed, leeway, chunks)
    packed_total(buf, off=0)           18 + packed size
    crc16(data)
    decompress(buf, off=0, verify=True) -> bytes
    maybe_decompress(buf, off=0, size=None, verify=True)  ProPackDecompress: RNC -> unpacked,
                                                          anything else -> the raw bytes
    unpack_m1(stream, packed, unpacked), unpack_m2(stream, packed, unpacked)
                                       stream = packed data (method 1) / whole stream with
                                       header (method 2), as the game passes them

CLI: rnc.py FILE [--offset N] [--out OUT]
"""
import argparse
import struct
import sys

HEADER_LEN = 0x12
MAGIC = b"RNC"
# bytes the readers may look past the packed data (method 1 keeps one 16-bit word ahead)
_SLACK = 8


class RncError(Exception):
    pass


def _make_crc_table():
    t = []
    for i in range(256):
        c = i
        for _ in range(8):
            c = (c >> 1) ^ 0xA001 if c & 1 else c >> 1
        t.append(c)
    return t


_CRC = _make_crc_table()


def crc16(data):
    c = 0
    t = _CRC
    for b in data:
        c = (c >> 8) ^ t[(c ^ b) & 0xFF]
    return c


def is_rnc(buf, off=0):
    return len(buf) - off >= HEADER_LEN and buf[off:off + 3] == MAGIC and buf[off + 3] in (1, 2)


def header(buf, off=0):
    if bytes(buf[off:off + 3]) != MAGIC:
        raise RncError("no RNC magic")
    if len(buf) - off < HEADER_LEN:
        raise RncError("truncated RNC header")
    unp, pk, cu, cp, lw, ch = struct.unpack_from(">IIHHBB", buf, off + 4)
    return {"method": buf[off + 3], "unpacked": unp, "packed": pk, "crc_unpacked": cu,
            "crc_packed": cp, "leeway": lw, "chunks": ch}


def packed_total(buf, off=0):
    return HEADER_LEN + header(buf, off)["packed"]


def _window(stream, n):
    """The first n + _SLACK bytes of stream (real bytes where present, zeros past the end)."""
    b = bytes(stream[:n + _SLACK])
    if len(b) < n + _SLACK:
        b += bytes(n + _SLACK - len(b))
    return b


def _copy_match(out, dist, length):
    """Append `length` bytes copied from len(out) - dist (byte by byte: may overlap)."""
    start = len(out) - dist
    if start < 0:
        raise RncError("match before the start of the output")
    if dist >= length:
        out += out[start:start + length]
    else:
        pat = out[start:]
        q, r = divmod(length, dist)
        out += pat * q + pat[:r]


# ------------------------------------------------------------------------------------------
# method 1
# ------------------------------------------------------------------------------------------
def unpack_m1(stream, packed, unpacked):
    src = _window(stream, packed)
    out = bytearray()
    # bit reader (func_00200EC0 / func_00200EE8): `buf` holds `cnt` left-over bits below the
    # look-ahead word, which sits at src[pos:pos + 2].
    buf = src[0] | src[1] << 8
    cnt = 0
    pos = 0
    # three tables of 16 slots: (mask, code, length, symbol); slots survive across chunks
    # like the routine's stack area does (only the slots a chunk defines are rewritten)
    tabs = [[None] * 16 for _ in range(3)]

    def getbits(n):
        nonlocal buf, cnt, pos
        v = buf & ((1 << n) - 1)
        cnt -= n
        if cnt < 0:
            left = cnt + n
            buf >>= left
            pos += 2
            if pos + 1 >= len(src):
                raise RncError("method 1: read past the packed data")
            buf |= (src[pos] | src[pos + 1] << 8) << 16
            k = n - left
            cnt = 16 - k
            buf >>= k
        else:
            buf >>= n
        return v

    def build(tab):
        # func_00200F20
        num = getbits(5)
        if num == 0:
            return
        lens = [getbits(4) for _ in range(num)]
        slot = 0
        acc = 0
        step = 0x80000000
        for L in range(1, 17):
            for sym in range(num):
                if lens[sym] != L:
                    continue
                c = acc >> (32 - L)
                rev = int(format(c, "0%db" % L)[::-1], 2)
                if slot >= 16:
                    raise RncError("method 1: more than 16 codes")
                tab[slot] = ((1 << L) - 1, rev, L, sym)
                slot += 1
                acc = (acc + step) & 0xFFFFFFFF
            step >>= 1

    def symbol(tab):
        # func_00200E1C: first slot whose (bits & mask) == code
        for e in tab:
            if e is None:
                break
            if buf & e[0] == e[1]:
                getbits(e[2])
                s = e[3]
                if s < 2:
                    return s
                return (1 << (s - 1)) | getbits(s - 1)
        raise RncError("method 1: no Huffman code matches")

    end = packed
    getbits(2)
    while True:
        build(tabs[0])
        build(tabs[1])
        build(tabs[2])
        n = (getbits(16) - 1) & 0xFFFF
        while True:
            run = symbol(tabs[0])
            if run:
                if pos + run + 1 >= len(src):
                    raise RncError("method 1: literal run past the packed data")
                out += src[pos:pos + run]
                pos += run
                # reload the look-ahead word above the left-over bits
                buf = (buf & ((1 << cnt) - 1)) | ((src[pos] | src[pos + 1] << 8) << cnt)
            if n == 0:
                break
            n -= 1
            dist = symbol(tabs[1]) + 1
            length = symbol(tabs[2]) + 2
            _copy_match(out, dist, length)
            if len(out) > unpacked:
                raise RncError("method 1: output longer than the header says")
        if pos >= end:
            break
    return bytes(out)


# ------------------------------------------------------------------------------------------
# method 2
# ------------------------------------------------------------------------------------------
def unpack_m2(stream, packed, unpacked):
    src = _window(stream, HEADER_LEN + packed)
    nsrc = len(src)
    out = bytearray()
    p = HEADER_LEN
    # flag byte with a sentinel 1 below the remaining bits; shifting the sentinel out means
    # "empty": the next byte is loaded (with a new sentinel) and its top bit is the result
    v = ((src[p] << 1) | 1) & 0xFF      # the top bit (lock flag) is already gone
    p += 1

    def bit():
        nonlocal v, p
        v <<= 1
        b = v >> 8
        v &= 0xFF
        if v == 0:
            if p >= nsrc:
                raise RncError("method 2: read past the packed data")
            v = (src[p] << 1) | 1
            p += 1
            b = v >> 8
            v &= 0xFF
        return b

    bit()                                # second flag bit (key flag)
    while True:
        if not bit():
            out.append(src[p])
            p += 1
            continue
        length = 2
        hi = -1                          # -1: read the distance high part
        if bit():
            if not bit():
                hi = 0                   # "10": length 2, one-byte distance
            else:
                length = 3
                if bit():
                    n = src[p]
                    p += 1
                    if n == 0:           # end of chunk
                        if bit():
                            continue
                        break
                    length = n + 8
        else:
            length = 4 + bit()
            if bit():
                length = (length - 1) * 2 + bit()
                if length == 9:          # literal run
                    x = 0
                    for _ in range(4):
                        x = x * 2 + bit()
                    k = 4 * (x + 3)
                    out += src[p:p + k]
                    p += k
                    if p > nsrc:
                        raise RncError("method 2: literal run past the packed data")
                    continue
        if hi < 0:
            hi = 0
            if bit():
                h = bit()
                if not bit():
                    if h == 0:
                        h = 2 + bit()
                else:
                    h = (h * 2 + bit()) | 4
                    if not bit():
                        h = h * 2 + bit()
                hi = h
        if p >= nsrc:
            raise RncError("method 2: read past the packed data")
        dist = ((hi << 8) | src[p]) + 1
        p += 1
        _copy_match(out, dist, length)
        if len(out) > unpacked:
            raise RncError("method 2: output longer than the header says")
    return bytes(out)


# ------------------------------------------------------------------------------------------
def decompress(buf, off=0, verify=True):
    """Unpack the RNC stream at buf[off:] and return the unpacked bytes."""
    h = header(buf, off)
    body = memoryview(buf)[off + HEADER_LEN:]
    if verify:
        if len(body) < h["packed"]:
            raise RncError("packed data truncated")
        if crc16(body[:h["packed"]]) != h["crc_packed"]:
            raise RncError("packed CRC mismatch")
    if h["method"] == 1:
        out = unpack_m1(body, h["packed"], h["unpacked"])
    elif h["method"] == 2:
        out = unpack_m2(memoryview(buf)[off:], h["packed"], h["unpacked"])
    else:
        raise RncError("unsupported RNC method %d" % h["method"])
    if len(out) != h["unpacked"]:
        raise RncError("unpacked %d bytes, header says %d" % (len(out), h["unpacked"]))
    if verify and crc16(out) != h["crc_unpacked"]:
        raise RncError("unpacked CRC mismatch")
    return out


def maybe_decompress(buf, off=0, size=None, verify=True):
    """ProPackDecompress: an RNC stream is unpacked, anything else is returned as stored."""
    if is_rnc(buf, off):
        return decompress(buf, off, verify)
    return bytes(buf[off:] if size is None else buf[off:off + size])


def main(argv=None):
    ap = argparse.ArgumentParser(description="RNC ProPack decompressor (methods 1 and 2), "
                                 "written from Turok 2's ProPackDecompress routines.")
    ap.add_argument("file")
    ap.add_argument("--offset", type=lambda s: int(s, 0), default=0)
    ap.add_argument("--out")
    a = ap.parse_args(argv)
    data = open(a.file, "rb").read()
    h = header(data, a.offset)
    out = decompress(data, a.offset, verify=True)
    print("method %d packed 0x%x unpacked 0x%x crc ok" % (h["method"], h["packed"], h["unpacked"]))
    if a.out:
        with open(a.out, "wb") as f:
            f.write(out)
    return 0


if __name__ == "__main__":
    sys.exit(main())
