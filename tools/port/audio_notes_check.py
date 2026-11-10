"""Checks the port's music sequencing against the ROM's sequence data (docs/port/AUDIO.md §9).

Run the game with T2_AUDIO_LOG=1 (and --audio --wav ...: without an output the mixer does not
run): the compressed sequence player logs every alCSeqNew (division, track mask, first three
track offsets) and every note on (the player's clock in microseconds, key, velocity, channel).
This script finds the song whose track offsets match in tools/assets/audio.py's sound tree, reads
its note-ons with the same track reader (CSeqTrack: running status, backup blocks), converts
ticks to microseconds the way the player does (uspt = (s32)(tempo * 1/division)), and compares
them with the logged ones from that alCSeqNew on: in order and as (time, key, channel) sets.
First pass only (no loop following), so compare runs shorter than the song's loop.

usage: python3 tools/port/audio_notes_check.py REPO LOG OFF0 OFF1 OFF2
   e.g. ... . run.log 44 CF 377      (the title song: "alCSeqNew: ... offsets 44 CF 377")"""
import re, sys

repo, logp = sys.argv[1], sys.argv[2]
want = [int(v, 16) for v in sys.argv[3:6]]
sys.path.insert(0, repo + "/tools/assets")
import audio  # noqa: E402

rom = open(repo + "/baserom.us.z64", "rb").read()
for s in audio.parse_tree(rom)["songs"]:
    if s["kind"] == "cseq":
        d = rom[s["rom"]:s["rom"] + s["size"]]
        if [audio.u32(d, 4 * i) for i in range(3)] == want:
            song, data = s, d
            break
division = audio.u32(data, 64)
ref, tempos = [], [(0, 500000)]
for t in range(16):
    off = audio.u32(data, 4 * t)
    if not off:
        continue
    tr = audio.CSeqTrack(data, off)
    tick, last = tr.varlen(), 0
    for _ in range(100000):
        st = tr.byte()
        if st == 0xFF:
            typ = tr.byte()
            if typ == 0x51:
                b = [tr.byte() for _ in range(3)]
                tempos.append((tick, (b[0] << 16) | (b[1] << 8) | b[2]))
            elif typ == 0x2F:
                break
            elif typ == 0x2E:
                tr.byte(), tr.byte()
            elif typ == 0x2D:
                tr.pos += 6
            last = 0
        else:
            if st & 0x80:
                status, b1 = st, tr.byte()
                last = st
            else:
                status, b1 = last, st
            hi = status & 0xF0
            if hi not in (0xC0, 0xD0):
                b2 = tr.byte()
                if hi == 0x90:
                    tr.varlen()
                    ref.append((tick, b1, b2, status & 0xF))
        tick += tr.varlen()
tempos = sorted(set(tempos[1:])) if any(t == 0 for t, _ in tempos[1:]) else sorted(tempos)
# the player keeps uspt as an integer: tempo * (1 / division), truncated (__CSPSetUsptFromTempo)
print('tempo events', tempos[:4])


def us(tk):
    s, pt, u = 0.0, 0, 500000
    for t0, uu in tempos:
        if t0 >= tk:
            break
        s += (t0 - pt) * int(u * (1.0 / division))
        pt, u = t0, uu
    return s + (tk - pt) * int(u * (1.0 / division))


ref = sorted((us(t), k, v, c) for t, k, v, c in ref)
game, on = [], False
pat = re.compile(r"note on t=(-?\d+) us key (\d+) vel (\d+) chan (\d+)")
want_s = "offsets %X %X %X" % tuple(want)
for line in open(logp):
    if "alCSeqNew" in line:
        on = want_s in line
        continue
    m = pat.search(line)
    if on and m:
        game.append(tuple(int(g) for g in m.groups()))
print(f"song {song['index']} (key {song['key']}): reference {len(ref)} note-ons, game {len(game)} logged")
t0g, t0r = game[0][0], ref[0][0]
n = min(len(game), len(ref), 400)
match = sum(1 for i in range(n) if (game[i][1], game[i][3]) == (ref[i][1], ref[i][3]))
# order inside one tick may differ: compare as multisets per 5 ms bucket
errs = []
for i in range(n):
    errs.append((game[i][0] - t0g) - (ref[i][0] - t0r))
print(f"first {n}: same key/chan in sequence {match}; time error vs reference (us): "
      f"max |e| {max(abs(e) for e in errs):.0f}, last {errs[-1]:.0f}")
for i in range(0, n, max(1, n // 8)):
    print(f"  #{i}: game t={game[i][0] - t0g:9d} key {game[i][1]:3d} ch {game[i][3]:2d} | "
          f"ref t={ref[i][0] - t0r:11.0f} key {ref[i][1]:3d} ch {ref[i][3]:2d}")
# order-free: the (time, key, channel) multisets up to the last logged game note
tl = game[-1][0] - t0g
gs = sorted((g[0] - t0g, g[1], g[3]) for g in game)
rs = sorted((round(r[0] - t0r), r[1], r[3]) for r in ref if r[0] - t0r <= tl)
print(f"order-free over {tl / 1e6:.1f} s: game {len(gs)} notes, reference {len(rs)}, identical: {gs == rs}")
