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

## The VM modules (`vm/`)

Drafts from the hand-matching pass over the paged VM modules (#46,
docs/VMASM.md). Each file's first comment says how far it is and why; several
also say where splat's cut is wrong (`WRONG CUT`), and those were checked over
the function's true extent, which `versions/cuts.us.txt` does not give yet for
all of them. The second module's functions (0x43CC14 and up) are linked
0x39000 below their names, as the module is.

| File | What it is | Distance when added |
|---|---|---|
| `vm/func_004004D8.c` | display-list renderer (0x1658 bytes, two jump tables) | 1408 of 1430 words (raw m2c draft) |
| `vm/func_00403C14.c` | fills a boss module's value table for a mode | 20 of 57 words (mode 1's constants scheduled early) |
| `vm/func_00403F3C.c` | resets a boss actor: its lights and start position | 304 of 325 words, mostly a shift (frame 0x50 vs 0x48: 0.0f, 3 and a timer's address kept in registers across the calls) |
| `vm/func_00404450.c` | sums the module's bonus and total over the slots in its mask | 7 of 75 words (the loop's base register) |
| `vm/func_0040457C.c` | spawns the next enemy wave near the player | 5 of 434 words (a do { } while (0) gets the $s registers but hoists a global's address) |
| `vm/func_00405AC8.c` | real extent 0x405AC8-0x405C68 (splat's func_00405BFC is its tail) | 5 of 104 words (`li a2, 0x13` in the jal's delay slot) |
| `vm/func_00406000.c` | one step of a boss attack script | 4 of 358 words (a copy based on &state instead of the word before it) |
| `vm/func_00406C4C.c` | spawns an effect from parameters passed by value | 52 of 112 words (GCC folds 10.0f * 256.0f) |
| `vm/func_00407AC8.c` | the same call shape as func_00405AC8 | 5 of 46 words (`li a2, 7` in the delay slot) |
| `vm/func_00407CB0.c` | checks whether the player holds all five pieces (message 0x172 or 0x173) | 9 of 47 words (the flags pointer's register) |
| `vm/func_004088B0.c` | sets a boss's timer from its definition's seconds | 63 of 66 words (the ROM rereads obj->def after two stores) |
| `vm/func_0040A6F0.c` | picks the next attack animation from a weighted table (true extent 0x20C) | 5 of 131 words (one register, one delay slot) |
| `vm/func_0040ABB4.c` | a boss module's phase 2 entry | 71 of 93 words (the ROM reaches the state fields through their own symbols) |
| `vm/func_0040D0F0.c` | second boss phase update: spawns minions around the boss | 152 of 174 words, a one-instruction shift (a mov.s copy of the first random value) |
| `vm/func_0040D5C4.c` | AI init of the second boss ("Eyeball") | 46 of 55 words (stores through their own symbols in the ROM) |
| `vm/func_00417490.c` | starts a match from the game setup and resets its slots | 27 of 95 words (the level read between the time store and its test) |
| `vm/func_0041760C.c` | the same code as func_00417490 | 86 of 97 words |
| `vm/func_0041778C.c` | the same code as func_00417490 | 25 of 97 words |
| `vm/func_0041D890.c` | snapshots the player and the multiplayer setup into a new buffer | 28 of 102 words (D_800F70A8 reloaded after each store) |
| `vm/func_0041DC5C.c` | picks the menu template for the game mode (true extent 0x134) | 1 of 77 words (GCC cross-jumps two stores of `kind`) |
| `vm/func_0041EA84.c` | Controller Pak menu code | 101 of 112 words (m2c draft) |
| `vm/func_0041F0C8.c` | Controller Pak menu code | 133 of 153 words (m2c draft) |
| `vm/func_0041FB50.c` | Controller Pak menu code | 62 of 105 words (m2c draft) |
| `vm/func_0041FCF4.c` | Controller Pak menu code | 111 of 116 words (m2c draft) |
| `vm/func_0042019C.c` | true extent 0x2B8 (its tail was func_004203B0) | 164 of 174 words (m2c draft) |
| `vm/func_0042085C.c` | true extent 0x3D4 (cut into three) | 235 of 245 words (m2c draft) |
| `vm/func_00421528.c` | copies a block from D_800C201C | 88 of 88 words (m2c draft) |
| `vm/func_00422258.c` | copies a name, padded with spaces | 5 of 47 words (the fill loop's index register) |
| `vm/func_00422FC0.c` | menu event: resets the menu state and selects the event's port | 13 of 32 words (an inlined helper's parameter reloaded from its stack slot) |
| `vm/func_004233E8.c` | menu event: sets up a player's (or the Pak's) port | 24 of 36 words (the ROM keeps an inlined parameter in an unused stack slot) |
| `vm/func_004242EC.c` | cycles a mode and takes two maxima (osGetMemSize); true extent 0x21C | 80 of 135 words (scheduling) |
| `vm/func_00424780.c` | menu code | 113 of 131 words (m2c draft) |
| `vm/func_00424978.c` | menu item: checks an entry against the player's inventory | 24 of 38 words (the branches invert) |
| `vm/func_00424A60.c` | whether a level is open, from two flags each | 74 of 78 words (GCC keeps &D_800F7078 in a register) |
| `vm/func_00425220.c` | menu code | 258 of 268 words (m2c draft) |
| `vm/func_00425BC0.c` | true extent 0x2B4 (its tail was func_00425D94) | 165 of 186 words (m2c draft) |
| `vm/func_00425E74.c` | true extent 0x1F0 (its tail was func_00425F84) | 92 of 132 words (m2c draft) |
| `vm/func_00428A18.c` | osPfsIsPlug (libultra pfsisplug.c); starts at 0x428A20, 0x17C | 35 of 95 words (__osMaxControllers reused, not reloaded) |
| `vm/func_00429FA0.c` | true extent 0xAE0 | 674 of 702 words (raw m2c draft) |
| `vm/func_0042AC0C.c` | true extent 0x7D8, two irregular switches | does not compile yet (m2c draft) |
| `vm/func_0042BA40.c` | Turok 2's variant of Rage Wars func_8044C7B8_de (0x508) | 340 of 344 words (raw m2c draft) |
| `vm/func_0042EF2C.c` | starts a match from the menu's configuration (0x298) | 159 of 166 words (the top's schedule, a struct copy, the loop's registers) |
| `vm/func_0042F344.c` | the module's per-player timer callbacks | 112 of 112 words (0x1C0 vs 0x1AC: the ROM does not strength-reduce the loops) |
| `vm/func_00430454.c` | resets the match and asks the session for mode 13 at a warp | 18 of 42 words ($s0/$s1 swapped, one store scheduled) |
| `vm/func_0045353C.c` | multiplayer menu item: sets the level from D_80130924 | 10 of 199 words (two byte stores through `lui $at`) |
| `vm/func_004549B0.c` | sets a player's options block to its defaults | 106 of 109 words (0x1B0 vs 0x1B4: the 0x30 loop walks a pointer in the ROM) |
| `vm/func_004567AC.c` | builds the save block; starts at 0x4567B0 (0x198) | 24 of 102 words (D_800F70A8 read directly, then through a CSE'd address) |
| `vm/func_0045BDF8.c` | 1 when two checks pass | 8 of 15 words (the ROM returns by a branch; GCC folds it into sltiu) |
| `vm/func_00467B4C.c` | bit stream: reads a length code; starts at 0x467B50 (0xC0) | 47 of 48 words (the loop keeps the old count) |
| `vm/func_004691B8.c` | initialises a unit quad; starts at 0x4691C0 (0x90) | 31 of 36 words (registers only) |
| `vm/func_00469374.c` | starts a level after checking its warp (0xA8; splat cut it at D_00469400) | 18 of 42 words ($s0/$s1 swapped) |
