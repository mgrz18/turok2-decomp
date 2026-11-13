#!/usr/bin/env python3
"""The optional areas of the hub levels on their own (docs/port/PLAYTHROUGH.md section 12, round 13 b3).

    python3 tools/port/extras.py OUTDIR --talisman 73..77     # a talisman chamber
    python3 tools/port/extras.py OUTDIR --nuke 78..83         # a flesh-eater level's nuke part
        # OUTDIR/playthrough.txt, OUTDIR/teleport.env (run as playthrough.py's, --pak-blank, 18,000 frames)

A new game, then T2_WARP_TO the level's portal warp (N001 for the chamber of level N, N003 for its
flesh-eater level), as walking into the portal does:

- **talisman chamber** (73 Leap of Faith, 74 Breath of Life, 75 Heart of Fire, 76 Whispers, 77 Eye
  of Truth): the pedestal's action needs the level's talisman piece (items 4400-4404, given with
  T2_GIVE_ITEMS: DEBUG-ASSISTED, in a real run it is picked up in the level, fullrun.TALISMAN);
  then fullrun.CHAMBER's walk: the piece placed (a cinema), the door, the talisman, the exit N002
  back to the portal's level. No teleport.
- **flesh-eater level** (78-83, one nuke part 4500 each, m_NukePartLevelFlags[level - 1]): a
  teleport next to the part and a short walk (DEBUG-ASSISTED: the level's way to it, through its
  flesh eaters, is not routed).
"""
import argparse, os, sys

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import fullrun  # noqa: E402

NEW_GAME = "START@4400 A@4500 A@4700 A@4800"
WARP_AT = 12963         # T2_WARP_TO's default: 60 frames into level 51's gameplay
CHAMBER_AT = 13200      # the chamber is loaded (fullrun.CHAMBER counts from it)
# chamber level: (portal warp, the piece item, the talisman item)
TALISMANS = {73: (2001, 4402, 4382), 74: (3001, 4400, 4380), 75: (4001, 4404, 4384),
             76: (5001, 4403, 4383), 77: (6001, 4401, 4381)}
# flesh-eater level: (portal warp, the nuke part (x, y, z): a level actor of object 611 = item 4500)
NUKES = {78: (1003, (-1075.2, 0.0, -358.4)), 79: (2003, (2751.9, 307.2, -1244.7)),
         80: (3003, (692.8, 0.0, -2317.3)), 81: (4003, (-2747.0, 0.0, -587.5)),
         82: (5003, (-2088.7, 307.2, -1733.2)), 83: (6003, (102.4, 0.0, -3123.2))}


def write(out, inputs, env):
    os.makedirs(out, exist_ok=True)
    with open(os.path.join(out, "playthrough.txt"), "w") as f:
        f.write("\n".join(inputs) + "\n")
    with open(os.path.join(out, "teleport.env"), "w") as f:
        for k, v in env:
            f.write("%s=\"%s\"\n" % (k, v))
        f.write("export %s\n" % " ".join(k for k, _ in env))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("out")
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument("--talisman", type=int, choices=sorted(TALISMANS))
    g.add_argument("--nuke", type=int, choices=sorted(NUKES))
    a = ap.parse_args()
    inputs = [NEW_GAME]
    env = [("T2_CHEATS", "1")]
    if a.talisman:
        warp, piece, _ = TALISMANS[a.talisman]
        inputs += ["%s@%d..%d" % (b, CHAMBER_AT + f0, CHAMBER_AT + f1) for b, f0, f1 in fullrun.CHAMBER]
        env += [("T2_WARP_TO", str(warp)), ("T2_GIVE_ITEMS", "%d:1@13100" % piece)]
    else:
        warp, (x, y, z) = NUKES[a.nuke]
        inputs.append("CU@16010..16060")
        env += [("T2_WARP_TO", str(warp)), ("T2_TELEPORT", "16000:%.1f,%.1f,%.1f,0.0" % (x, y, z + 90.0))]
    write(a.out, inputs, env)


if __name__ == "__main__":
    main()
