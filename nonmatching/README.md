# Not matching yet

C that does what the original does, written and checked against the
disassembly, but that does not yet compile to the same bytes: register
allocation and instruction order still differ. These files are not built;
the asm is. They are here so the reading of each function is not lost while
the permuter (`tools/permuter/`) or a person finishes it. When one matches,
it moves to `src/` through `tools/integrate.py` and leaves this folder.

| File | What it is | Distance when added |
|---|---|---|
| `engine/func_00204778.c` | heap debug view: the heap's blocks as coloured bands | 0x5B8 vs 0x618 bytes, 245 of 390 words |
| `engine/func_00209AF4.c` | expands a packed model into an F3DEX2 display-list resource | 414 of 530 words |
| `engine/func_00221D3C.c` | builds the cached geometry display list behind a patched branch | 182 of 362 words (registers: a do { } while (0) and a later r = 0 fix the start) |
| `engine/func_002222E0.c` | per-region object pass over the world's object table | same size, 206 of 385 words |
| `engine/func_0022D2FC.c` | particle matrix build (Rage Wars Particle_BuildMatrix) | 13 of 624 words (registers in the depth scale: local-alloc gives distanceRange's min/max $f0/$f1); its file, src/us/code/func_0022D2FC.c, pulls its asm in with INCLUDE_ASM |
| `engine/func_002429BC.c` | AI actor setup (Rage Wars ACTOR_SETUP) | 9 of 145 words (the shared -1 in $v0, not $v1, so sched2 cannot hoist the flags load) |
| `engine/func_00246BC0.c` | player input from the controller | 6 of 330 words (a dead reload; needs a pinned register) |
| `engine/func_002478C8.c` | multiplayer kill and tag messages | 205 of 210 words (two branches cross-jumped; the ROM keeps 1.5f in $f20) |
| `engine/func_00248720.c` | fires the player's weapon: ammo, animation, projectile | 35 of 219 words (only the projectile loop: loop.c's find_and_verify_loops moves the found block out of the loop) |
| `engine/func_0024ED54.c` | rams the creature in front while riding | 1 of 135 words (bne in the ROM, bnel here: reorg annuls the slot because $v0 is live on the fall-through) |
| `engine/func_00255D70.c` | clears flag 0x800000; in multiplayer opens the player's screen | 3 of 53 words (prologue schedule) |
| `engine/func_0026C148.c` | does a segment cross a region's surface height | 1 of 62 words (a j the ROM threads to the epilogue; match_func masks it) |
| `camera/func_0027D160.c` | CCamera Advance: position, orientation, frustum and clip planes per frame | 147 of 936 words (FP registers, frustum store order) |
| `camera/func_0027E000.c` | CCamera DisplayListSetup: projection, viewport, scissor, look-at, z-buffer clear, fog | 442 of 476 words differ (much of it one word's shift) |

## The VM modules (`vm/`)

Drafts from the hand-matching pass over the paged VM modules (#46,
docs/VMASM.md). Each file's first comment says how far it is and why; several
also say where splat's cut is wrong (`WRONG CUT`), and those were checked over
the function's true extent, which `versions/cuts.us.txt` does not give yet for
all of them. The second module's functions (0x43CC14 and up) are linked
0x39000 below their names, as the module is.

| File | What it is | Distance when added |
|---|---|---|
| `vm/func_00424008.c` | resizes the selected VI mode around its horizontal and field centres (Rage Wars func_8040BDB8_de) | 131 of 185 words (GCC drops the first of each pair of halfword stores the ROM keeps) |
| `vm/func_0042BA40.c` | Turok 2's variant of Rage Wars func_8044C7B8_de (0x508) | 340 of 344 words (raw m2c draft) |
| `vm/func_00464960.c` | raw m2c draft (W batch), not worked on | 340 of 344 words |
