#!/usr/bin/env python3
"""Turok 2 (N64, US) textures, palettes, HUD images and fonts -> PNG.

usage: python tools/assets/textures.py baserom.us.z64 --out DIR [--only textures|graphics] [--limit N] [--no-png]

  DIR/textures/NNNN.png            texture-set entry NNNN: every animation step side by side (frame i%F, palette i%P)
  DIR/textures/NNNN/SS.png         (with --split) one PNG per animation step
  DIR/graphics/III_idNNNN/FFF.png  graphic set index III (type id NNNN; HUD / font / menu image), frame FFF
  DIR/textures.json                per texture: index, ROM range, format, size, frames, palettes, speed, lod, kind
  DIR/graphics.json                per graphic set: index, id, ROM range, frames (size, grid, opaque)

Formats are those the engine's loaders parse (see docs/assets/TEXTURES.md):
  Texture_Load / Texture_GetInfo / Texture_DecompressCallback  (texture banks, the "texture sets" index)
  CScene__LoadGraphicSet / Hud_GetImageFrame / Hud_DrawImageFrame  (graphic sets: tiled 2D images, fonts)

Locating: the cartridge index lives at ROM 0xFCBFB4 (CEngineApp__Main stores it in m_CartridgeIndex and passes it
to CScene__Construct). Entry 3 = texture sets, 16 = graphic sets, 17 = graphic-set type ids. This module carries a
minimal locator and an RNC1 unpacker until tools/assets/romfs.py / propack.py (part a1) are used instead; the
parsing functions below only take decompressed bytes.
"""
import argparse
import json
import math
import os
import struct
import sys
import zlib

CARTRIDGE_INDEX = 0xFCBFB4
CART_TEXTURE_SETS = 3
CART_GRAPHIC_SETS = 16
CART_GRAPHIC_SET_TYPES = 17

FORMAT_NAMES = {
    0: 'CI8 (RGBA16 TLUT)',
    1: 'CI4 (RGBA16 TLUT)',
    2: 'RGBA32',
    3: 'CI4 (IA16 TLUT)',
    4: 'RGBA16 mip chain 32x32',
    5: 'I4 mip chain 64x64',
}

# --------------------------------------------------------------------------- locating (temporary)


def _u32(b, o):
    return struct.unpack_from('>I', b, o)[0]


def iset_entries(buf, base):
    """CISet at buf[base]: u32 count, count+1 offsets relative to base. Returns [(start, size)]."""
    n = _u32(buf, base)
    offs = struct.unpack_from('>%dI' % (n + 1), buf, base + 4)
    return [(base + offs[i], offs[i + 1] - offs[i]) for i in range(n)]


def cartridge(rom):
    return iset_entries(rom, CARTRIDGE_INDEX)


def load_block(rom, start, size):
    """Bytes of a ROM block, unpacked when it starts with an RNC1 header (what ProPack__Decompress does)."""
    raw = rom[start:start + size]
    if raw[:4] == b'RNC\x01':
        return rnc_unpack(rom, start)
    return raw


try:  # prefer the shared decompressor once it exists
    sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
    from propack import decompress as _shared_unpack  # noqa: E402  (part a1's tools/assets/propack.py)

    def rnc_unpack(data, off=0):
        return _shared_unpack(data, off)
except Exception:  # pragma: no cover - fallback
    _shared_unpack = None

    def rnc_unpack(data, off=0):
        return _rnc1_unpack(data, off)


def _crc16(data):
    crc = 0
    for b in data:
        crc = (crc >> 8) ^ _CRC[(crc ^ b) & 0xFF]
    return crc


def _mkcrc():
    t = []
    for i in range(256):
        c = i
        for _ in range(8):
            c = (c >> 1) ^ 0xA001 if c & 1 else c >> 1
        t.append(c)
    return t


_CRC = _mkcrc()


def _rnc1_unpack(data, off=0):
    """RNC ProPack method 1. Header: 'RNC\\x01', u32 unpacked, u32 packed, u16 crc(unpacked), u16 crc(packed),
    u8 leeway, u8 chunks; bits LSB-first from little-endian 16-bit words, literal runs byte-aligned and spliced in."""
    if data[off:off + 4] != b'RNC\x01':
        raise ValueError('not RNC1')
    ulen, plen, ucrc, _ = struct.unpack_from('>IIHH', data, off + 4)
    chunks = data[off + 17]
    d = bytes(data[off + 18:off + 18 + plen]) + b'\0' * 8

    def lw(q):
        return d[q] | d[q + 1] << 8 | d[q + 2] << 16 | d[q + 3] << 24

    st = [lw(0), 16, 0]  # bit buffer, bits of the current+next word, address of the next word

    def bits(n):
        b, c, q = st
        v = b & ((1 << n) - 1)
        b >>= n
        c -= n
        if c < 16:
            q += 2
            b |= lw(q) << c
            c += 16
        st[0], st[1], st[2] = b, c, q
        return v

    def table():
        n = bits(5)
        lens = [bits(4) for _ in range(n)]
        tab = {}
        code = 0
        for L in range(1, 17):
            for i, ll in enumerate(lens):
                if ll == L:
                    v, r = code >> (16 - L), 0
                    for _ in range(L):
                        r = (r << 1) | (v & 1)
                        v >>= 1
                    tab[(L, r)] = i
                    code += 1 << (16 - L)
        return tab, (max(lens) if lens else 0)

    def huf(t):
        tab, maxl = t
        b = st[0]
        for L in range(1, maxl + 1):
            v = tab.get((L, b & ((1 << L) - 1)))
            if v is not None:
                bits(L)
                return (1 << (v - 1)) | bits(v - 1) if v >= 2 else v
        raise ValueError('bad huffman code')

    out = bytearray()
    bits(2)
    for _ in range(chunks):
        raw, dist, ln = table(), table(), table()
        sub = bits(16)
        while sub:
            sub -= 1
            n = huf(raw)
            if n:
                q = st[2]
                out += d[q:q + n]
                q += n
                c = st[1] - 16
                st[0] = (st[0] & ((1 << c) - 1)) | (lw(q) << c)
                st[1], st[2] = c + 16, q
            if sub:
                pos = huf(dist) + 1
                n = huf(ln) + 2
                if pos >= n:
                    out += out[-pos:len(out) - pos + n]
                else:
                    for _ in range(n):
                        out.append(out[-pos])
    out = bytes(out[:ulen])
    if _crc16(out) != ucrc:
        raise ValueError('RNC CRC mismatch at 0x%X' % off)
    return out


# --------------------------------------------------------------------------- pixel formats


def rgba5551(w):
    return (((w >> 11) & 31) * 255 // 31, ((w >> 6) & 31) * 255 // 31, ((w >> 1) & 31) * 255 // 31,
            255 if w & 1 else 0)


def ia16(w):
    i = w >> 8
    return (i, i, i, w & 0xFF)


def _nibbles(data):
    for b in data:
        yield b >> 4
        yield b & 15


def decode_ci(data, w, h, palette, bits):
    idx = list(_nibbles(data[:w * h // 2])) if bits == 4 else list(data[:w * h])
    return [palette[i] if i < len(palette) else (255, 0, 255, 255) for i in idx]


def decode_rgba16(data, w, h, stride=None):
    stride = stride or w * 2
    return [rgba5551(struct.unpack_from('>H', data, y * stride + 2 * x)[0]) for y in range(h) for x in range(w)]


def decode_rgba32(data, w, h):
    return [tuple(data[4 * i:4 * i + 4]) for i in range(w * h)]


def decode_i4(data, w, h, stride=None, alpha=True):
    stride = stride or w // 2
    px = []
    for y in range(h):
        row = data[y * stride:y * stride + (w + 1) // 2]
        for x, v in enumerate(_nibbles(row)):
            if x >= w:
                break
            i = v * 17
            px.append((i, i, i, i if alpha else 255))
    return px


# --------------------------------------------------------------------------- texture banks


def parse_texture_bank(blob):
    """A texture bank (Texture_Load): CISet of 3 blocks.
    block 0: 8 bytes  fmt, speed, log2 width, log2 height, prim LOD (gDPSetPrimColor m), kind, 0, 0
    block 1: CISet of frames (pixel data)       block 2: CISet of palettes (count 0 when none)"""
    blocks = iset_entries(blob, 0)
    if len(blocks) != 3 or blocks[0][1] < 6:
        raise ValueError('not a texture bank')
    i0 = blocks[0][0]
    info = blob[i0:i0 + 8]
    frames = [blob[s:s + n] for s, n in iset_entries(blob, blocks[1][0])]
    pal_base = blocks[2][0]
    palettes = [blob[s:s + n] for s, n in iset_entries(blob, pal_base)] if _u32(blob, pal_base) else []
    return {
        'format': info[0], 'speed': info[1], 'width': 1 << info[2], 'height': 1 << info[3],
        'lod': info[4], 'kind': info[5], 'info': info.hex(), 'frames': frames, 'palettes': palettes,
    }


def _mip_level0(fmt, data):
    """Top level of the fixed mip chains Texture_Load loads through sTextureLoadDL_* (rows padded to 8 bytes)."""
    if fmt == 4:
        return 32, 32, decode_rgba16(data, 32, 32, 64)
    return 64, 64, decode_i4(data, 64, 64, 32)


def decode_texture_frame(bank, frame, palette=0):
    """(width, height, [RGBA tuples]) for one frame with one palette."""
    fmt, w, h = bank['format'], bank['width'], bank['height']
    data = bank['frames'][frame]
    if fmt in (0, 1, 3):
        praw = bank['palettes'][palette % len(bank['palettes'])] if bank['palettes'] else b''
        n = 256 if fmt == 0 else 16
        praw = praw[:2 * n].ljust(2 * n, b'\0')
        conv = ia16 if fmt == 3 else rgba5551
        pal = [conv(v) for v in struct.unpack('>%dH' % n, praw)]
        return w, h, decode_ci(data, w, h, pal, 8 if fmt == 0 else 4)
    if fmt == 2:
        return w, h, decode_rgba32(data, w, h)
    if fmt in (4, 5):
        return _mip_level0(fmt, data)
    raise ValueError('unknown texture format %d' % fmt)


def animation_steps(bank, cap=64):
    """The (frame, palette) pairs the engine shows: step t -> frame t%F, palette t%P (CRender material code)."""
    F = len(bank['frames'])
    P = max(1, len(bank['palettes']))
    n = min(F * P // math.gcd(F, P), cap)
    return [(t % F, t % P) for t in range(max(n, 1))]


# --------------------------------------------------------------------------- graphic sets


def parse_graphic_set(blob):
    """A graphic set (Hud_GetImageFrame): s16 frame count, u16 id, pad to 8, then per frame {u32 offset from the
    start, u8 opaque, 3 pad}. A frame is a tile grid: u16 columns, u16 rows, u16 width, u16 height, then
    columns*rows blocks row by row, each {u32 w, u32 h, pixels}: opaque -> I4 (drawn with the prim colour);
    otherwise RGBA16 texels followed by an I4 opacity mask (Hud_DrawMaskedBlock)."""
    n, gid = struct.unpack_from('>hH', blob, 0)
    frames = []
    for k in range(n):
        off, opaque = struct.unpack_from('>IB', blob, 8 + 8 * k)
        cols, rows, width, height = struct.unpack_from('>4H', blob, off)
        blocks = []
        p = off + 8
        for _ in range(cols * rows):
            bw, bh = struct.unpack_from('>II', blob, p)
            size = bw * bh // 2 if opaque else bw * bh * 2 + bw * bh // 2
            blocks.append((bw, bh, p + 8))
            p += 8 + size
        frames.append({'offset': off, 'opaque': bool(opaque), 'columns': cols, 'rows': rows,
                       'width': width, 'height': height, 'blocks': blocks, 'end': p})
    return {'count': n, 'header_id': gid, 'frames': frames}


def decode_graphic_frame(blob, fr):
    """(width, height, pixels) of a whole frame, its blocks pasted on the grid."""
    cols, rows = fr['columns'], fr['rows']
    widths = [fr['blocks'][c][0] for c in range(cols)] if fr['blocks'] else []
    heights = [fr['blocks'][r * cols][1] for r in range(rows)] if fr['blocks'] else []
    W, H = sum(widths), sum(heights)
    canvas = [(0, 0, 0, 0)] * (W * H)
    y0 = 0
    for r in range(rows):
        x0 = 0
        for c in range(cols):
            bw, bh, p = fr['blocks'][r * cols + c]
            if fr['opaque']:
                px = decode_i4(blob[p:p + bw * bh // 2], bw, bh)
            else:
                col = decode_rgba16(blob[p:p + bw * bh * 2], bw, bh)
                mask = decode_i4(blob[p + bw * bh * 2:p + bw * bh * 2 + bw * bh // 2], bw, bh)
                px = [(c_[0], c_[1], c_[2], m[0]) for c_, m in zip(col, mask)]
            for y in range(bh):
                canvas[(y0 + y) * W + x0:(y0 + y) * W + x0 + bw] = px[y * bw:(y + 1) * bw]
            x0 += bw
        y0 += heights[r] if r < len(heights) else 0
    # crop to the declared image size (edge blocks may be padded)
    w, h = min(fr['width'] or W, W), min(fr['height'] or H, H)
    if (w, h) != (W, H):
        canvas = [canvas[y * W + x] for y in range(h) for x in range(w)]
    return w, h, canvas


# --------------------------------------------------------------------------- PNG


def write_png(path, w, h, px):
    raw = bytearray()
    for y in range(h):
        raw.append(0)
        for p in px[y * w:(y + 1) * w]:
            raw += bytes(p)

    def chunk(t, d):
        c = struct.pack('>I', len(d)) + t + d
        return c + struct.pack('>I', zlib.crc32(t + d) & 0xFFFFFFFF)

    with open(path, 'wb') as f:
        f.write(b'\x89PNG\r\n\x1a\n' + chunk(b'IHDR', struct.pack('>IIBBBBB', w, h, 8, 6, 0, 0, 0))
                + chunk(b'IDAT', zlib.compress(bytes(raw), 6)) + chunk(b'IEND', b''))


def hstack(images, gap=1):
    """Side-by-side strip of (w, h, px) images."""
    W = sum(w for w, _, _ in images) + gap * (len(images) - 1)
    H = max(h for _, h, _ in images)
    out = [(0, 0, 0, 0)] * (W * H)
    x0 = 0
    for w, h, px in images:
        for y in range(h):
            out[y * W + x0:y * W + x0 + w] = px[y * w:(y + 1) * w]
        x0 += w + gap
    return W, H, out


# --------------------------------------------------------------------------- CLI


def export_textures(rom, out, limit=None, png=True, split=False):
    cart = cartridge(rom)
    base, size = cart[CART_TEXTURE_SETS]
    entries = iset_entries(rom, base)
    os.makedirs(os.path.join(out, 'textures'), exist_ok=True)
    meta = []
    for i, (start, n) in enumerate(entries[:limit]):
        blob = load_block(rom, start, n)
        bank = parse_texture_bank(blob)
        rec = {'index': i, 'rom': '0x%07X' % start, 'rom_size': n, 'compressed': rom[start:start + 4] == b'RNC\x01',
               'unpacked_size': len(blob), 'format': bank['format'], 'format_name': FORMAT_NAMES.get(bank['format']),
               'width': bank['width'], 'height': bank['height'], 'frames': len(bank['frames']),
               'palettes': len(bank['palettes']), 'speed': bank['speed'], 'lod': bank['lod'], 'kind': bank['kind'],
               'info': bank['info']}
        meta.append(rec)
        if png:
            imgs = [decode_texture_frame(bank, f, p) for f, p in animation_steps(bank)]
            if split and len(imgs) > 1:
                d = os.path.join(out, 'textures', '%04d' % i)
                os.makedirs(d, exist_ok=True)
                for k, im in enumerate(imgs):
                    write_png(os.path.join(d, '%02d.png' % k), *im)
            write_png(os.path.join(out, 'textures', '%04d.png' % i), *hstack(imgs))
    with open(os.path.join(out, 'textures.json'), 'w') as f:
        json.dump({'texture_sets_rom': '0x%07X' % base, 'texture_sets_size': size, 'count': len(entries),
                   'textures': meta}, f, indent=1)
    return meta


def export_graphics(rom, out, limit=None, png=True):
    cart = cartridge(rom)
    base, size = cart[CART_GRAPHIC_SETS]
    tbase, _ = cart[CART_GRAPHIC_SET_TYPES]
    stride, count = struct.unpack_from('>ii', rom, tbase)
    ids = [_u32(rom, tbase + 8 + stride * k) if stride == 4 else struct.unpack_from('>H', rom, tbase + 8 + stride * k)[0]
           for k in range(count)]
    entries = iset_entries(rom, base)
    meta = []
    for i, (start, n) in enumerate(entries[:limit]):
        blob = load_block(rom, start, n)
        gs = parse_graphic_set(blob)
        gid = ids[i] if i < len(ids) else gs['header_id']
        rec = {'index': i, 'id': gid, 'header_id': gs['header_id'], 'rom': '0x%07X' % start, 'rom_size': n,
               'unpacked_size': len(blob), 'frames': []}
        d = os.path.join(out, 'graphics', '%03d_id%04d' % (i, gid))
        if png:
            os.makedirs(d, exist_ok=True)
        for k, fr in enumerate(gs['frames']):
            rec['frames'].append({'opaque_i4': fr['opaque'], 'width': fr['width'], 'height': fr['height'],
                                  'grid': [fr['columns'], fr['rows']],
                                  'block': list(fr['blocks'][0][:2]) if fr['blocks'] else None})
            if png and fr['blocks']:
                write_png(os.path.join(d, '%03d.png' % k), *decode_graphic_frame(blob, fr))
        meta.append(rec)
    with open(os.path.join(out, 'graphics.json'), 'w') as f:
        json.dump({'graphic_sets_rom': '0x%07X' % base, 'graphic_sets_size': size, 'count': len(entries),
                   'graphic_sets': meta}, f, indent=1)
    return meta


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument('rom')
    ap.add_argument('--out', required=True)
    ap.add_argument('--only', choices=['textures', 'graphics'])
    ap.add_argument('--limit', type=int)
    ap.add_argument('--no-png', action='store_true')
    ap.add_argument('--split', action='store_true', help='also one PNG per animation step')
    a = ap.parse_args()
    rom = open(a.rom, 'rb').read()
    if a.only != 'graphics':
        t = export_textures(rom, a.out, a.limit, not a.no_png, a.split)
        print('textures: %d banks, %d frames' % (len(t), sum(r['frames'] for r in t)))
    if a.only != 'textures':
        g = export_graphics(rom, a.out, a.limit, not a.no_png)
        print('graphic sets: %d, %d frames' % (len(g), sum(len(r['frames']) for r in g)))


if __name__ == '__main__':
    main()
