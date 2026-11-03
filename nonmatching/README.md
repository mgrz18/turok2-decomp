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
| `engine/func_00221D3C.c` | builds the cached geometry display list behind a patched branch | 182 of 362 words (registers: a do { } while (0) and a later r = 0 fix the start) |
| `engine/func_002222E0.c` | per-region object pass over the world's object table | same size, 206 of 385 words |
| `engine/func_00226490.c` | draws the type-2 actors at the head of the list, then the mode's extras | 51 of 87 words (the actor goes straight to $s0, not through $a0) |
| `engine/func_0022D2FC.c` | particle matrix build (Rage Wars Particle_BuildMatrix) | 13 of 624 words (registers in the depth scale); its file, src/us/code/func_0022D2FC.c, pulls its asm in with INCLUDE_ASM |
| `engine/func_002429BC.c` | AI actor setup (Rage Wars ACTOR_SETUP) | 9 of 145 words (one load scheduled after the -1 stores) |
| `engine/func_00246BC0.c` | player input from the controller | 6 of 330 words (a dead reload; needs a pinned register) |
| `engine/func_002478C8.c` | multiplayer kill and tag messages | 205 of 210 words (two branches cross-jumped; the ROM keeps 1.5f in $f20) |
| `engine/func_00248720.c` | fires the player's weapon: ammo, animation, projectile | 35 of 219 words (E batch: only the projectile loop; cse threads the found path past the caller test) |
| `engine/func_00248A8C.c` | strips the weapons and gives the starting kit | 6 of 71 words (E batch: the clearing loops match; one shared `li 1` in a delay slot) |
| `engine/func_00249358.c` | hurts a player: damage, protection, red flash | 5 of 245 words (E batch: $v0/$v1 swapped around the handicap and the surface check) |
| `engine/func_0024EAF8.c` | mounts the creature in front | 5 of 151 words (the kind == 1 compare) |
| `engine/func_0024ED54.c` | rams the creature in front while riding | 1 of 135 words (bne in the ROM, bnel here) |
| `engine/func_00255D70.c` | clears flag 0x800000; in multiplayer opens the player's screen | 3 of 53 words (prologue schedule) |
| `engine/func_0025F2F8.c` | AI callback: 0x64 from the 0xC8 level | 2 of 24 words (E batch: FP register of the final add) |
| `engine/func_0025FBB8.c` | AI callback: raises the level by 20, sets the timer | 2 of 20 words (E batch: FP register of the final add) |
| `engine/func_002626A8.c` | AI init: installs an actor kind's table and callbacks | 4 of 16 words (one temporary where the ROM has two) |
| `engine/func_0026B4A4.c` | the level triangle nearest below a point | 3 of 237 words (one `li` scheduled; uses a duplicated branch) |
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
| `vm/func_004004D8.c` | display-list renderer (0x1658 bytes, two jump tables) | 44 of 1430 words (X batch: register allocation only) |
| `vm/func_0041F0C8.c` | Controller Pak menu code | 3 of 153 words (X batch: CSE threads the first r test past the second) |
| `vm/func_00421528.c` | copies a block from D_800C201C | 6 of 75 words (X batch: one store to D_80110060 scheduled early) |
| `vm/func_00428A18.c` | osPfsIsPlug (libultra pfsisplug.c); starts at 0x428A20, 0x17C | 35 of 95 words (__osMaxControllers reused, not reloaded) |
| `vm/func_0042BA40.c` | Turok 2's variant of Rage Wars func_8044C7B8_de (0x508) | 340 of 344 words (raw m2c draft) |
| `vm/func_004567AC.c` | builds the save block; starts at 0x4567B0 (0x198) | 24 of 102 words (D_800F70A8 read directly, then through a CSE'd address) |
| `vm/func_0045BDF8.c` | 1 when two checks pass | 8 of 15 words (the ROM returns by a branch; GCC folds it into sltiu) |
| `vm/func_00467B4C.c` | bit stream: reads a length code; starts at 0x467B50 (0xC0) | 47 of 48 words (the loop keeps the old count) |
| `vm/func_004691B8.c` | initialises a unit quad; starts at 0x4691C0 (0x90) | 31 of 36 words (registers only) |
