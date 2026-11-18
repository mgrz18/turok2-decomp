#!/usr/bin/env python3
"""shotcmp.py - compare screenshots pixel by pixel (Mac vs Linux, port vs N64), no numpy.

    shotcmp.py A B [--diff OUT.png]           one pair (PPM P6 or 8-bit PNG)
    shotcmp.py --dirs DIR_A DIR_B [--diff-dir D] [--glob 's.ppm.*.ppm']
                                              every file present in both directories

For each pair: size, the number of differing pixels (any channel > 0, and > --tol), the max
channel difference, PSNR over RGB (inf when identical), and the bounding box of the pixels over
--tol. Images of different sizes are compared after a nearest-neighbour resize of B to A's size
(--fit; for the N64 reference shots, which are 640x480 or 320x240). With --diff a PNG is written:
A | B | |A-B| x --gain (red where over --tol). Exit status 0, also when images differ.
"""
import argparse
import glob
import math
import os
import struct
import sys
import zlib


def read_ppm(path):
    with open(path, "rb") as f:
        data = f.read()
    # P6\n<w> <h>\n255\n<rgb>
    parts = []
    i = 0
    while len(parts) < 4:
        while data[i:i + 1].isspace():
            i += 1
        if data[i:i + 1] == b"#":
            while data[i:i + 1] != b"\n":
                i += 1
            continue
        j = i
        while not data[j:j + 1].isspace():
            j += 1
        parts.append(data[i:j])
        i = j
    i += 1
    if parts[0] != b"P6" or parts[3] != b"255":
        raise ValueError(f"{path}: not an 8-bit P6 PPM")
    w, h = int(parts[1]), int(parts[2])
    return w, h, bytes(data[i:i + w * h * 3])


def read_png(path):
    with open(path, "rb") as f:
        data = f.read()
    if data[:8] != b"\x89PNG\r\n\x1a\n":
        raise ValueError(f"{path}: not a PNG")
    pos, idat, w = 8, [], None
    while pos < len(data):
        ln, typ = struct.unpack(">I4s", data[pos:pos + 8])
        body = data[pos + 8:pos + 8 + ln]
        if typ == b"IHDR":
            w, h, depth, ctype, _, _, interlace = struct.unpack(">IIBBBBB", body)
            if depth != 8 or ctype not in (2, 6) or interlace:
                raise ValueError(f"{path}: only 8-bit RGB/RGBA non-interlaced PNGs")
        elif typ == b"IDAT":
            idat.append(body)
        pos += 12 + ln
    bpp = 3 if ctype == 2 else 4
    raw = zlib.decompress(b"".join(idat))
    stride = w * bpp
    out = bytearray(h * stride)
    prev = bytearray(stride)
    for y in range(h):
        ft = raw[y * (stride + 1)]
        line = bytearray(raw[y * (stride + 1) + 1:(y + 1) * (stride + 1)])
        if ft == 1:
            for x in range(bpp, stride):
                line[x] = (line[x] + line[x - bpp]) & 255
        elif ft == 2:
            for x in range(stride):
                line[x] = (line[x] + prev[x]) & 255
        elif ft == 3:
            for x in range(stride):
                left = line[x - bpp] if x >= bpp else 0
                line[x] = (line[x] + ((left + prev[x]) >> 1)) & 255
        elif ft == 4:
            for x in range(stride):
                a = line[x - bpp] if x >= bpp else 0
                b = prev[x]
                c = prev[x - bpp] if x >= bpp else 0
                p = a + b - c
                pa, pb, pc = abs(p - a), abs(p - b), abs(p - c)
                pr = a if pa <= pb and pa <= pc else (b if pb <= pc else c)
                line[x] = (line[x] + pr) & 255
        out[y * stride:(y + 1) * stride] = line
        prev = line
    if bpp == 4:
        rgb = bytearray(w * h * 3)
        rgb[0::3], rgb[1::3], rgb[2::3] = out[0::4], out[1::4], out[2::4]
        out = rgb
    return w, h, bytes(out)


def read_image(path):
    with open(path, "rb") as f:
        magic = f.read(2)
    return read_png(path) if magic == b"\x89P" else read_ppm(path)


def write_png(path, w, h, rgb):
    raw = b"".join(b"\x00" + rgb[y * w * 3:(y + 1) * w * 3] for y in range(h))

    def chunk(t, b):
        return struct.pack(">I", len(b)) + t + b + struct.pack(">I", zlib.crc32(t + b) & 0xFFFFFFFF)

    with open(path, "wb") as f:
        f.write(b"\x89PNG\r\n\x1a\n" + chunk(b"IHDR", struct.pack(">IIBBBBB", w, h, 8, 2, 0, 0, 0))
                + chunk(b"IDAT", zlib.compress(raw, 6)) + chunk(b"IEND", b""))


def fit(w, h, rgb, tw, th):
    if (w, h) == (tw, th):
        return rgb
    out = bytearray(tw * th * 3)
    for y in range(th):
        sy = y * h // th
        for x in range(tw):
            sx = x * w // tw
            o, s = (y * tw + x) * 3, (sy * w + sx) * 3
            out[o:o + 3] = rgb[s:s + 3]
    return bytes(out)


def compare(pa, pb, tol, diff=None, gain=4, mean=False):
    wa, ha, a = read_image(pa)
    wb, hb, b = read_image(pb)
    b = fit(wb, hb, b, wa, ha)
    w, h = wa, ha
    n = w * h
    if a == b:
        if diff:
            write_png(diff, w * 3, h, b"".join(a[y * w * 3:(y + 1) * w * 3] * 2 + bytes(w * 3) for y in range(h)))
        return dict(w=w, h=h, ndiff=0, ntol=0, maxd=0, psnr=math.inf, box=None, mean_a=None, mean_b=None)
    sq = 0
    maxd = 0
    ndiff = ntol = 0
    x0 = y0 = 1 << 30
    x1 = y1 = -1
    dimg = bytearray(n * 3) if diff else None
    for i in range(n):
        o = i * 3
        d0 = abs(a[o] - b[o])
        d1 = abs(a[o + 1] - b[o + 1])
        d2 = abs(a[o + 2] - b[o + 2])
        m = max(d0, d1, d2)
        if m:
            ndiff += 1
            sq += d0 * d0 + d1 * d1 + d2 * d2
            if m > maxd:
                maxd = m
            if m > tol:
                ntol += 1
                y, x = divmod(i, w)
                x0, x1, y0, y1 = min(x0, x), max(x1, x), min(y0, y), max(y1, y)
            if dimg is not None:
                if m > tol:
                    dimg[o] = min(255, 64 + m * gain)
                    dimg[o + 1] = dimg[o + 2] = min(255, m * gain // 2)
                else:
                    dimg[o] = dimg[o + 1] = dimg[o + 2] = min(255, m * gain)
    mse = sq / (n * 3)
    psnr = 10 * math.log10(255 * 255 / mse) if mse else math.inf
    if diff:
        write_png(diff, w * 3, h, b"".join(a[y * w * 3:(y + 1) * w * 3] + b[y * w * 3:(y + 1) * w * 3]
                                           + bytes(dimg[y * w * 3:(y + 1) * w * 3]) for y in range(h)))
    res = dict(w=w, h=h, ndiff=ndiff, ntol=ntol, maxd=maxd, psnr=psnr,
               box=(x0, y0, x1, y1) if x1 >= 0 else None)
    if mean:
        res["mean_a"] = tuple(round(sum(a[c::3]) / n, 1) for c in range(3))
        res["mean_b"] = tuple(round(sum(b[c::3]) / n, 1) for c in range(3))
    return res


def line(name, r):
    box = "-" if r["box"] is None else "%d,%d-%d,%d" % r["box"]
    psnr = "inf" if r["psnr"] == math.inf else "%.2f" % r["psnr"]
    extra = ""
    if r.get("mean_a"):
        extra = "  mean %s vs %s" % (r["mean_a"], r["mean_b"])
    return "%-28s %dx%d  diff %7d  >tol %7d  max %3d  psnr %6s  box %s%s" % (
        name, r["w"], r["h"], r["ndiff"], r["ntol"], r["maxd"], psnr, box, extra)


def main():
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("a", nargs="?")
    ap.add_argument("b", nargs="?")
    ap.add_argument("--dirs", nargs=2, metavar=("DIR_A", "DIR_B"))
    ap.add_argument("--glob", default="*.ppm")
    ap.add_argument("--tol", type=int, default=8, help="channel difference counted as a real one (default 8)")
    ap.add_argument("--gain", type=int, default=4)
    ap.add_argument("--diff", help="diff PNG for a single pair")
    ap.add_argument("--diff-dir", help="with --dirs: a diff PNG per differing pair")
    ap.add_argument("--mean", action="store_true", help="also print each image's mean RGB")
    args = ap.parse_args()
    if args.dirs:
        da, db = args.dirs
        names = sorted({os.path.basename(p) for p in glob.glob(os.path.join(da, args.glob))}
                       & {os.path.basename(p) for p in glob.glob(os.path.join(db, args.glob))},
                       key=lambda s: [int(t) if t.isdigit() else t for t in s.replace(".", " ").split()])
        same = 0
        for nm in names:
            dpath = None
            if args.diff_dir:
                os.makedirs(args.diff_dir, exist_ok=True)
                dpath = os.path.join(args.diff_dir, os.path.splitext(nm)[0] + ".diff.png")
            r = compare(os.path.join(da, nm), os.path.join(db, nm), args.tol, None, args.gain, args.mean)
            if r["ndiff"] == 0:
                same += 1
            elif dpath:
                compare(os.path.join(da, nm), os.path.join(db, nm), args.tol, dpath, args.gain)
            print(line(nm, r))
        print(f"shotcmp: {len(names)} pairs, {same} identical")
        return 0
    if not (args.a and args.b):
        ap.error("two images or --dirs")
    print(line(os.path.basename(args.a), compare(args.a, args.b, args.tol, args.diff, args.gain, args.mean)))
    return 0


if __name__ == "__main__":
    sys.exit(main())
