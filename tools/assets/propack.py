#!/usr/bin/env python3
"""RNC ProPack decompressor (methods 1 and 2), as used by Turok 2's ProPackDecompress.

The game (src/us/code/func_00200B00.c, ProPackDecompress) checks the first word
for 'RNC' (`(w & ~0xFF) == 0x524E4300`), reads the unpacked size at +4 and calls
RNC_UnpackMethod1(hdr + 0x12, dst, packed, unpacked) or RNC_UnpackMethod2(hdr, ...).
Both decoders are hand-written asm in the ROM (0x2010C0..); this is a port of the
functionally equivalent C in references/rw_junk/docs/hand_asm_c/rnc.c (same engine
family, differential-tested there against the original machine code).

Header (18 bytes, big endian):
  +0  'RNC' + method (1 or 2)   +4 unpacked size   +8 packed size (stream only)
  +C  crc16 unpacked (poly 0xA001, not checked by the game)   +E crc16 packed
  +10 leeway   +11 chunk count   +12 packed stream

The CRC of the unpacked data is checked here (verify=True) as a self-test.

CLI: python tools/assets/propack.py FILE [--offset N] [--out OUT]
"""
import struct
import sys

HEADER_LEN = 0x12
MAGIC = b"RNC"

_CRC_TABLE = []
for _i in range(256):
    _c = _i
    for _ in range(8):
        _c = (_c >> 1) ^ 0xA001 if _c & 1 else _c >> 1
    _CRC_TABLE.append(_c)


def crc16(data):
    crc = 0
    for b in data:
        crc = _CRC_TABLE[(crc ^ b) & 0xFF] ^ (crc >> 8)
    return crc


class RncError(Exception):
    pass


def is_rnc(buf, off=0):
    return buf[off:off + 3] == MAGIC and buf[off + 3] in (1, 2)


def header(buf, off=0):
    """-> dict(method, unpacked, packed, crc_unpacked, crc_packed, leeway, chunks)"""
    if buf[off:off + 3] != MAGIC:
        raise RncError("no RNC magic")
    m = buf[off + 3]
    unp, pk, cu, cp, lw, ch = struct.unpack_from(">IIHHBB", buf, off + 4)
    return dict(method=m, unpacked=unp, packed=pk, crc_unpacked=cu,
                crc_packed=cp, leeway=lw, chunks=ch)


def packed_total(buf, off=0):
    """Bytes occupied by the RNC chunk including the header."""
    return HEADER_LEN + header(buf, off)["packed"]


# ---------------------------------------------------------------- method 1
class _B1:
    __slots__ = ("win", "left", "p", "d")


def _m1_refill(b, n):
    b.left += n
    b.win >>= (b.left & 31)
    d, p = b.d, b.p
    b.win = (b.win | (d[p + 3] << 24) | (d[p + 2] << 16)) & 0xFFFFFFFF
    b.p = p + 2
    n -= b.left
    b.left = 16 - n
    return n


def _m1_bits(b, mask, n):
    r = b.win & mask
    b.left -= n
    if b.left < 0:
        n = _m1_refill(b, n)
    b.win >>= (n & 31)
    return r


def _m1_table(b):
    n = _m1_bits(b, 0x1F, 5)
    if n == 0:
        return None
    if n > 16:
        raise RncError("huffman table too large")
    lens = [_m1_bits(b, 0xF, 4) for _ in range(n)]
    tab = []
    code = 0
    inc = 0x80000000
    for ln in range(1, 17):
        for i in range(n):
            if lens[i] != ln:
                continue
            top = code >> 16
            rev = 0
            for k in range(ln):
                rev |= ((top >> (15 - k)) & 1) << k
            tab.append(((1 << ln) - 1, rev, ln, i))
            code = (code + inc) & 0xFFFFFFFF
        inc >>= 1
    return tab


def _m1_huff(b, tab):
    if not tab:
        raise RncError("empty huffman table used")
    w = b.win
    for mask, code, ln, idx in tab:
        if (w & mask) == code:
            break
    else:
        raise RncError("huffman decode failed")
    n = ln
    b.left -= n
    if b.left < 0:
        n = _m1_refill(b, n)
    b.win >>= (n & 31)
    if idx < 2:
        return idx
    nb = idx - 1
    val = b.win & ((1 << nb) - 1)
    n = nb
    b.left -= n
    if b.left < 0:
        n = _m1_refill(b, n)
    b.win >>= (n & 31)
    return val | (1 << nb)


def unpack_m1(stream, packed, unpacked):
    d = bytes(stream) + b"\0" * 16
    out = bytearray()
    b = _B1()
    b.d, b.p = d, 0
    b.win = d[0] | (d[1] << 8)
    b.left = 0
    _m1_bits(b, 2, 2)
    raw = dist = lent = None
    while True:
        t = _m1_table(b)
        raw = t if t is not None else raw
        t = _m1_table(b)
        dist = t if t is not None else dist
        t = _m1_table(b)
        lent = t if t is not None else lent
        sub = (_m1_bits(b, 0xFFFFFFFF, 16) - 1) & 0xFFFF
        while True:
            n = _m1_huff(b, raw)
            if n:
                if len(out) + n > unpacked:
                    raise RncError("literal overrun")
                out += d[b.p:b.p + n]
                b.p += n
                w = d[b.p] | (d[b.p + 1] << 8)
                sh = b.left & 31
                b.win = ((b.win & ((1 << sh) - 1)) | (w << sh)) & 0xFFFFFFFF
            if sub == 0:
                break
            sub -= 1
            ds = _m1_huff(b, dist)
            ln = _m1_huff(b, lent) + 2
            src = len(out) - ds - 1
            if src < 0 or len(out) + ln > unpacked:
                raise RncError("bad back reference")
            for _ in range(ln):
                out.append(out[src])
                src += 1
        if b.p >= packed:
            break
    return bytes(out)


# ---------------------------------------------------------------- method 2
def unpack_m2(stream, packed, unpacked):
    d = bytes(stream) + b"\0" * 16
    out = bytearray()
    p = 1
    t2 = (((d[0] << 1) + 1) << 1)

    def getbit():
        nonlocal t2, p
        t2 <<= 1
        bit = (t2 >> 8) & 1
        t2 &= 0xFF
        if t2 == 0:
            t2 = (d[p] << 1) + bit
            p += 1
            bit = (t2 >> 8) & 1
            t2 &= 0xFF
        return bit

    while True:
        while getbit() == 0:
            out.append(d[p])
            p += 1
            if len(out) > unpacked:
                raise RncError("literal overrun")
        if getbit() == 0:
            ln = 4 + getbit()
            if getbit():
                ln = (ln - 1) * 2 + getbit()
                if ln == 9:
                    n = 0
                    for _ in range(4):
                        n = n * 2 + getbit()
                    n = 4 * (n + 3)
                    out += d[p:p + n]
                    p += n
                    continue
            short = False
        else:
            if getbit() == 0:
                ln = 2
                short = True
            else:
                ln = 3
                short = False
                if getbit():
                    ln = d[p]
                    p += 1
                    if ln == 0:
                        if getbit():
                            continue
                        break
                    ln += 8
        hi = 0
        if not short and getbit():
            hi = getbit()
            if getbit():
                hi = (hi * 2 + getbit()) | 4
                if getbit() == 0:
                    hi = hi * 2 + getbit()
            elif hi == 0:
                hi = 2 + getbit()
        ds = (hi << 8) | d[p]
        p += 1
        src = len(out) - ds - 1
        if src < 0 or len(out) + ln > unpacked:
            raise RncError("bad back reference")
        for _ in range(ln):
            out.append(out[src])
            src += 1
        if p > packed + 16:
            raise RncError("input overrun")
    return bytes(out)


def decompress(buf, off=0, verify=True):
    """Decompress the RNC chunk at buf[off:]. Returns the unpacked bytes."""
    h = header(buf, off)
    stream = buf[off + HEADER_LEN: off + HEADER_LEN + h["packed"]]
    if len(stream) != h["packed"]:
        raise RncError("truncated stream")
    if h["method"] == 1:
        out = unpack_m1(stream, h["packed"], h["unpacked"])
    elif h["method"] == 2:
        out = unpack_m2(stream, h["packed"], h["unpacked"])
    else:
        raise RncError("unknown method %d" % h["method"])
    if len(out) != h["unpacked"]:
        raise RncError("size mismatch %d != %d" % (len(out), h["unpacked"]))
    if verify and crc16(out) != h["crc_unpacked"]:
        raise RncError("crc mismatch")
    return out


def maybe_decompress(buf, off=0, size=None, verify=True):
    """Like ProPackDecompress: RNC -> unpacked, otherwise the raw bytes."""
    if is_rnc(buf, off):
        return decompress(buf, off, verify)
    return bytes(buf[off:off + size]) if size is not None else bytes(buf[off:])


def main(argv=None):
    import argparse
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("file")
    ap.add_argument("--offset", type=lambda s: int(s, 0), default=0)
    ap.add_argument("--out")
    a = ap.parse_args(argv)
    data = open(a.file, "rb").read()
    h = header(data, a.offset)
    out = decompress(data, a.offset)
    print("method %d packed 0x%x unpacked 0x%x crc ok" % (h["method"], h["packed"], len(out)))
    if a.out:
        open(a.out, "wb").write(out)


if __name__ == "__main__":
    main()
