# Not matching yet

C that does what the original does, written and checked against the
disassembly, but that does not yet compile to the same bytes: register
allocation and instruction order still differ. These files are not built;
the asm is. They are here so the reading of each function is not lost while
the permuter (`tools/permuter/`) or a person finishes it. When one matches,
it moves to `src/` through `tools/integrate.py` and leaves this folder.

| File | What it is | Distance when added |
|---|---|---|
| `engine/func_00204778.c` | heap debug view: the heap's blocks as coloured bands | 0x5B8 vs 0x618 bytes, 245 of 390 words (CSE skips the `if (hi < lo)` block and folds the highlight; without -fcse-skip-blocks it is 181 words, so the source gives that join more uses) |
| `engine/func_0022D2FC.c` | particle matrix build (Rage Wars Particle_BuildMatrix) | 13 of 624 words (registers in the inlined depth scale: local-alloc puts distanceRange's max/min in $f0/$f1, so farness gets $f2 and the rest shift; in the ROM $f0/$f1 are busy there); its file, src/us/code/func_0022D2FC.c, pulls its asm in with INCLUDE_ASM |
| `engine/func_002429BC.c` | AI actor setup (Rage Wars ACTOR_SETUP) | 9 of 145 words (the shared -1 in $v0, not $v1, so sched2 cannot hoist the flags load) |
| `engine/func_00246BC0.c` | player input from the controller | 6 of 330 words (a dead reload; needs a pinned register) |
| `engine/func_002478C8.c` | multiplayer kill and tag messages | 134 of 210 words, 0x2FC vs 0x348 (everything matches up to the tag branch's killer message, which jump2 cross-jumps with the "you killed" tail: both a0 moves carry the same REG_EQUAL note) |
| `engine/func_00248720.c` | fires the player's weapon: ammo, animation, projectile | 35 of 219 words (only the projectile loop: loop.c's find_and_verify_loops moves the found block out of the loop) |
| `engine/func_0024ED54.c` | rams the creature in front while riding | 1 of 135 words (bne in the ROM, bnel here: reorg annuls the slot because $v0 is live on the fall-through) |
| `engine/func_00255D70.c` | clears flag 0x800000; in multiplayer opens the player's screen | 3 of 53 words (prologue schedule) |
| `engine/func_0026C148.c` | does a segment cross a region's surface height | 1 of 62 words (a j the ROM threads to the epilogue; match_func masks it) |
| `camera/func_0027E000.c` | CCamera DisplayListSetup: projection, viewport, scissor, look-at, z-buffer clear, fog | 54 of 476 words, same size (sched1's order in the head 0x84-0x17C: which m_WarpXScale load comes first, and so the mov.s and the x/y truncs) |

## The VM modules (`vm/`)

Drafts from the hand-matching pass over the paged VM modules (#46,
docs/VMASM.md). Each file's first comment says how far it is and why; several
also say where splat's cut is wrong (`WRONG CUT`), and those were checked over
the function's true extent, which `versions/cuts.us.txt` does not give yet for
all of them. The second module's functions (0x43CC14 and up) are linked
0x39000 below their names, as the module is.

| File | What it is | Distance when added |
|---|---|---|
| `vm/func_00424008.c` | resizes the selected VI mode around its horizontal and field centres (Rage Wars func_8040BDB8_de) | 32 of 185 words, same size and stack layout (scheduling and registers in the hStart part; flow kept from deleting the halfword stores) |
