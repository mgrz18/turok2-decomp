"""Minimal RNC ProPack method 1 unpacker (stand-in until tools/assets/propack.py from part a1 lands).

Port of the algorithm ProPackDecompress -> RNC_UnpackMethod1 implements (ROM 0x200B00, hand asm):
an 18-byte header 'RNC' 01, u32 unpacked size, u32 packed size, u16 unpacked crc, u16 packed crc,
u8 leeway, u8 block count; then per block three Huffman tables (literal run lengths, match
distances, match lengths), a 16-bit chunk count and the chunks. Bits are read LSB-first from
little-endian 16-bit words; literal bytes sit inline in the stream after the look-ahead word.
"""
import struct


def _mirror(v, n):
    r = 0
    for _ in range(n):
        r = (r << 1) | (v & 1)
        v >>= 1
    return r


class _Bits:
    __slots__ = ("d", "p", "buf", "cnt")

    def __init__(self, d, p):
        self.d, self.p = d, p
        self.buf = self._w(p)
        self.cnt = 16

    def _w(self, p):
        d = self.d
        return (d[p] if p < len(d) else 0) | ((d[p + 1] if p + 1 < len(d) else 0) << 8)

    def adv(self, n):
        self.buf >>= n
        self.cnt -= n
        if self.cnt < 16:
            self.p += 2
            self.buf |= self._w(self.p) << self.cnt
            self.cnt += 16

    def read(self, n):
        v = self.buf & ((1 << n) - 1)
        self.adv(n)
        return v

    def fix(self):
        self.cnt -= 16
        self.buf &= (1 << self.cnt) - 1
        self.buf |= self._w(self.p) << self.cnt
        self.cnt += 16


def _table(b):
    num = b.read(5)
    if not num:
        return []
    lens = [b.read(4) for _ in range(num)]
    tab = []
    code = 0
    for ln in range(1, 17):
        for j in range(num):
            if lens[j] == ln:
                tab.append(((1 << ln) - 1, _mirror(code, ln), ln, j))
                code += 1
        code <<= 1
    return tab


def _huf(b, tab):
    buf = b.buf
    for mask, code, ln, val in tab:
        if buf & mask == code:
            b.adv(ln)
            if val >= 2:
                val = (1 << (val - 1)) | b.read(val - 1)
            return val
    raise ValueError("bad huffman code")


def is_rnc(data, off=0):
    return data[off:off + 3] == b"RNC"


def unpack(data, off=0):
    """Returns the unpacked bytes of the RNC chunk at data[off]."""
    if data[off:off + 4] != b"RNC\x01":
        raise ValueError("not RNC method 1 at %#x" % off)
    ulen, plen = struct.unpack(">II", data[off + 4:off + 12])
    b = _Bits(data, off + 18)
    b.adv(2)
    out = bytearray()
    while len(out) < ulen:
        raw, dist, lng = _table(b), _table(b), _table(b)
        count = b.read(16)
        while True:
            n = _huf(b, raw)
            if n:
                out += data[b.p:b.p + n]
                b.p += n
                b.fix()
            count -= 1
            if count <= 0:
                break
            pos = _huf(b, dist) + 1
            n = _huf(b, lng) + 2
            if pos >= n:
                out += out[-pos:len(out) - pos + n]
            else:
                for _ in range(n):
                    out.append(out[-pos])
    return bytes(out[:ulen])
