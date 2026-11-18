#!/usr/bin/env python3
"""Compare the port's audio (turok2 --audio --wav) with the N64's (refshots.py --audio-dump), round 20
i5, docs/port/AUDIO.md section 12.

Both are stereo s16 at ~22050 Hz (the N64: VI clock / (dacrate + 1) = 22048 Hz for dacrate 2207;
the port: 22050). The two runs do not start a sound at the same sample, so the tool aligns them:
first coarsely by the cross-correlation of the 10 ms RMS envelopes (+-MAXLAG s), then to the sample
by the cross-correlation of the mono signals around the best coarse lag. Then, over the overlap:

  rms / peak (each channel), the port/N64 rms ratio in dB, the L/R balance (dB) of each,
  the waveform correlation (mono, after alignment), the envelope correlation,
  the band energies (octaves from 86 Hz) and the spectral centroid, the pitch ratio (the lag
  of the log-frequency spectrum's cross-correlation, in cents), the tail (the decay of the last
  second after the loudest point: reverb), the DC offset.

    python3 tools/port/audiocmp.py N64.raw PORT.wav [--n64-from S] [--port-from S] [--len S]
        [--maxlag S] [--windows W]

--windows W also prints the same numbers for consecutive windows of W seconds (where a sound
goes missing). Never commit the recordings.
"""
import argparse
import sys
import wave

import numpy as np

N64_RATE = 48681812 / 2208.0


def load_raw(path):
    a = np.fromfile(path, dtype="<i2")
    return a[: len(a) // 2 * 2].reshape(-1, 2).astype(np.float64)


def load_wav(path):
    with wave.open(path) as w:
        n, ch, rate = w.getnframes(), w.getnchannels(), w.getframerate()
        a = np.frombuffer(w.readframes(n), dtype="<i2").astype(np.float64)
    return a.reshape(-1, ch), rate


def env(x, rate, hop=0.01):
    h = int(rate * hop)
    m = x.mean(axis=1)
    n = len(m) // h
    return np.sqrt((m[: n * h].reshape(n, h) ** 2).mean(axis=1) + 1e-9), h


def xcorr_lag(a, b, maxlag):
    """lag L (in samples of a/b) maximising sum a[i] b[i - L] (b delayed by L matches a)"""
    n = 1
    while n < len(a) + len(b):
        n <<= 1
    fa, fb = np.fft.rfft(a - a.mean(), n), np.fft.rfft(b - b.mean(), n)
    c = np.fft.irfft(fa * np.conj(fb), n)
    lags = np.concatenate([np.arange(0, maxlag + 1), np.arange(-maxlag, 0)])
    vals = np.concatenate([c[: maxlag + 1], c[n - maxlag:]])
    k = int(np.argmax(vals))
    den = np.sqrt((a - a.mean()) @ (a - a.mean()) * ((b - b.mean()) @ (b - b.mean()))) + 1e-9
    return int(lags[k]), float(vals[k] / den)


def slide(a, b, K):
    """lag L in [0, K] maximising the normalised correlation of b with a[L: L + len(b)]"""
    a = np.asarray(a, dtype=np.float64)
    b = np.asarray(b, dtype=np.float64) - np.mean(b)
    m = len(b)
    K = max(0, min(K, len(a) - m))
    n = 1
    while n < len(a) + m:
        n <<= 1
    c = np.fft.irfft(np.fft.rfft(a, n) * np.conj(np.fft.rfft(b, n)), n)[: K + 1]
    cs = np.concatenate([[0.0], np.cumsum(a)])
    cs2 = np.concatenate([[0.0], np.cumsum(a * a)])
    s1 = cs[m: m + K + 1] - cs[: K + 1]
    s2 = cs2[m: m + K + 1] - cs2[: K + 1]
    var = np.maximum(s2 - s1 * s1 / m, 1e-9)
    r = c / np.sqrt(var * (b @ b + 1e-9))
    k = int(np.argmax(r))
    return k, float(r[k])


def db(x):
    return 20 * np.log10(max(x, 1e-9))


def bands(m, rate):
    spec = np.abs(np.fft.rfft(m * np.hanning(len(m)))) ** 2
    f = np.fft.rfftfreq(len(m), 1 / rate)
    out, lo = [], 86.0
    while lo < rate / 2:
        hi = lo * 2
        out.append(spec[(f >= lo) & (f < hi)].sum())
        lo = hi
    tot = sum(out) + 1e-9
    cen = (spec * f).sum() / (spec.sum() + 1e-9)
    return [10 * np.log10(v / tot + 1e-12) for v in out], cen, spec, f


def pitch_cents(sa, sb, f):
    """the shift of b's spectrum against a's on a log-frequency axis (cents; + = b higher)"""
    lf = np.linspace(np.log2(100), np.log2(8000), 4800)  # 1/400 octave = 3 cents
    ga = np.interp(lf, np.log2(f[1:]), np.log(sa[1:] + 1e-3))
    gb = np.interp(lf, np.log2(f[1:]), np.log(sb[1:] + 1e-3))
    lag, c = xcorr_lag(gb, ga, 200)
    return lag * 1200 / 400.0, c


def stats(name, x, rate):
    rms = np.sqrt((x ** 2).mean(axis=0))
    peak = np.abs(x).max(axis=0)
    return {"rms": rms, "peak": peak, "bal": db(rms[0]) - db(rms[1]), "dc": x.mean(axis=0)}


def compare(n, p, rate, label=""):
    sn, sp = stats("n64", n, rate), stats("port", p, rate)
    mn, mp = n.mean(axis=1), p.mean(axis=1)
    wc = float(np.corrcoef(mn, mp)[0, 1]) if mn.std() > 0 and mp.std() > 0 else 0.0
    en, _ = env(n, rate)
    ep, _ = env(p, rate)
    k = min(len(en), len(ep))
    ec = float(np.corrcoef(np.log(en[:k]), np.log(ep[:k]))[0, 1]) if k > 2 else 0.0
    bn, cn, spn, f = bands(mn, rate)
    bp, cp, spp, _ = bands(mp, rate)
    cents, pc = pitch_cents(spn, spp, f)
    rn, rp = np.sqrt((sn["rms"] ** 2).mean()), np.sqrt((sp["rms"] ** 2).mean())
    print("%s rms n64 %.0f/%.0f port %.0f/%.0f (port %+.2f dB); peak n64 %.0f/%.0f port %.0f/%.0f; "
          "L-R n64 %+.2f port %+.2f dB; dc n64 %.1f/%.1f port %.1f/%.1f" % (
              label, sn["rms"][0], sn["rms"][1], sp["rms"][0], sp["rms"][1], db(rp) - db(rn),
              sn["peak"][0], sn["peak"][1], sp["peak"][0], sp["peak"][1], sn["bal"], sp["bal"],
              sn["dc"][0], sn["dc"][1], sp["dc"][0], sp["dc"][1]))
    print("%s corr wave %.3f env %.3f; centroid n64 %.0f port %.0f Hz; pitch port %+.0f cents (c %.2f)" % (
        label, wc, ec, cn, cp, cents, pc))
    print("%s bands dB (86 Hz octaves) n64 %s" % (label, " ".join("%5.1f" % v for v in bn)))
    print("%s bands dB (86 Hz octaves) port %s" % (label, " ".join("%5.1f" % v for v in bp)))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("n64")
    ap.add_argument("port")
    ap.add_argument("--n64-from", type=float, default=0.0)
    ap.add_argument("--port-from", type=float, default=0.0)
    ap.add_argument("--len", type=float, default=0.0, help="seconds compared (default: the overlap)")
    ap.add_argument("--maxlag", type=float, default=10.0)
    ap.add_argument("--windows", type=float, default=0.0)
    ap.add_argument("--no-align", action="store_true")
    a = ap.parse_args()

    n = load_raw(a.n64) if not a.n64.endswith(".wav") else load_wav(a.n64)[0]
    p, prate = load_wav(a.port)
    rate = 22050.0
    n = n[int(a.n64_from * N64_RATE):]
    p = p[int(a.port_from * prate):]
    print("n64 %.2f s, port %.2f s (port rate %d)" % (len(n) / N64_RATE, len(p) / prate, prate))
    if not a.len:
        a.len = min(len(n) / N64_RATE, len(p) / prate) - a.maxlag
    L = int(a.len * rate)
    n = n[:L]
    if not a.no_align:
        # the N64 segment [0, len) searched in the port's [-maxlag, len + maxlag) around the same start
        ml = int(a.maxlag * rate)
        pre = min(ml, int(a.port_from * prate))
        p0 = load_wav(a.port)[0][int(a.port_from * prate) - pre:]
        en, h = env(n, rate)
        ep, _ = env(p0, rate)
        en, ep = np.sqrt(en), np.sqrt(ep)
        cl, cc = slide(ep, en, 2 * (ml // h))
        coarse = cl * h - pre  # the port is late by coarse samples
        w = int(rate * 0.25)
        lo = max(0, coarse + pre - w)
        seg = int(min(len(n), rate * 30))
        fl, fc = slide(p0[lo: lo + seg + 2 * w].mean(axis=1), n[:seg].mean(axis=1), 2 * w)
        lag = lo + fl - pre if fc > 0.3 else coarse
        print("align: envelope lag %+.3f s (c %.2f), waveform lag %+.4f s (c %.2f): port late by %+.4f s" % (
            coarse / rate, cc, (lo + fl - pre) / rate, fc, lag / rate))
        p = p0[pre + lag:]
    k = min(len(n), len(p))
    n, p = n[:k], p[:k]
    print("overlap %.2f s" % (k / rate))
    compare(n, p, rate, "all")
    if a.windows:
        w = int(a.windows * rate)
        for i in range(0, k - w + 1, w):
            compare(n[i: i + w], p[i: i + w], rate, "t%6.1f" % (i / rate))


if __name__ == "__main__":
    main()
