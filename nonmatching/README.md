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
| `engine/func_0021375C.c` | vertex file chunk unpack (Rage Wars func_002782E4) | 27 of 197 words (registers in the inner vertex loops) |
| `engine/func_00213A70.c` | box filter and vertex count (Rage Wars func_00278530) | 19 of 264 words (one register swap) |
| `engine/func_0021D39C.c` | draws an actor for one view | 35 of 264 words (registers only) |
| `engine/func_00220D00.c` | walks the 0xC0-byte level records | 12 of 324 words (registers only) |
| `engine/func_00221D3C.c` | builds the cached geometry display list behind a patched branch | 182 of 362 words (registers: a do { } while (0) and a later r = 0 fix the start) |
| `engine/func_002222E0.c` | per-region object pass over the world's object table | same size, 206 of 385 words |
| `engine/func_00226490.c` | draws the type-2 actors at the head of the list, then the mode's extras | 51 of 87 words (the actor goes straight to $s0, not through $a0) |
| `engine/func_0022D2FC.c` | particle matrix build (Rage Wars Particle_BuildMatrix) | 13 of 624 words (registers in the depth scale); its file, src/us/code/func_0022D2FC.c, pulls its asm in with INCLUDE_ASM |
| `engine/func_002429BC.c` | AI actor setup (Rage Wars ACTOR_SETUP) | 9 of 145 words (one load scheduled after the -1 stores) |
| `engine/func_00246BC0.c` | player input from the controller | 6 of 330 words (a dead reload; needs a pinned register) |
| `engine/func_002478C8.c` | multiplayer kill and tag messages | 205 of 210 words (two branches cross-jumped; the ROM keeps 1.5f in $f20) |
| `engine/func_00248720.c` | fires the player's weapon: ammo, animation, projectile | 215 of 233 words (draft from the asm, not tuned) |
| `engine/func_00248A8C.c` | strips the weapons and gives the starting kit | 12 of 71 words (the clearing loops' addressing) |
| `engine/func_00248BA8.c` | player warp / spawn | 68 of 348 words (registers only) |
| `engine/func_00249358.c` | hurts a player: damage, protection, red flash | 153 of 245 words (the health update's copies; the rest shifts) |
| `engine/func_0024B88C.c` | the weapon wheel direction the stick points at | 14 of 146 words (the final ABS test) |
| `engine/func_0024EAF8.c` | mounts the creature in front | 5 of 151 words (the kind == 1 compare) |
| `engine/func_0024ED54.c` | rams the creature in front while riding | 1 of 135 words (bne in the ROM, bnel here) |
| `engine/func_0024EF70.c` | turns the body and tilts the view from the stick | 39 of 129 words (the two rates scheduled in the other order) |
| `engine/func_00255D70.c` | clears flag 0x800000; in multiplayer opens the player's screen | 3 of 53 words (prologue schedule) |
| `engine/func_0025CAC8.c` | AI hit callback | 7 of 346 words (FP registers in one shared formula) |
| `engine/func_0025F29C.c` | AI callback: 0x120 from the 0xC8 level | 8 of 23 words (FP registers) |
| `engine/func_0025F2F8.c` | AI callback: 0x64 from the 0xC8 level | 8 of 24 words (FP registers) |
| `engine/func_0025F928.c` | AI callback: raises the level by 10, sets the timer | 8 of 20 words (FP registers) |
| `engine/func_0025F9FC.c` | AI callback: raises the level by 10, adds to the timer | 3 of 22 words (FP registers) |
| `engine/func_0025FBB8.c` | AI callback: raises the level by 20, sets the timer | 8 of 20 words (FP registers) |
| `engine/func_0025FCB8.c` | AI callback: raises the level by 20, sets the timer | 8 of 20 words (FP registers) |
| `engine/func_002626A8.c` | AI init: installs an actor kind's table and callbacks | 4 of 16 words (one temporary where the ROM has two) |
| `engine/func_0026B4A4.c` | the level triangle nearest below a point | 3 of 237 words (one `li` scheduled; uses a duplicated branch) |
| `engine/func_0026C148.c` | does a segment cross a region's surface height | 1 of 62 words (a j the ROM threads to the epilogue; match_func masks it) |
| `camera/func_0027D160.c` | CCamera Advance: position, orientation, frustum and clip planes per frame | 147 of 936 words (FP registers, frustum store order) |
| `camera/func_0027E000.c` | CCamera DisplayListSetup: projection, viewport, scissor, look-at, z-buffer clear, fog | 442 of 476 words differ (much of it one word's shift) |

