# Not matching yet

C that does what the original does, written and checked against the
disassembly, but that does not yet compile to the same bytes: register
allocation and instruction order still differ. These files are not built;
the asm is. They are here so the reading of each function is not lost while
the permuter (`tools/permuter/`) or a person finishes it. When one matches,
it moves to `src/` through `tools/integrate.py` and leaves this folder.

| File | What it is | Distance when added |
|---|---|---|
| `camera/func_0027E000.c` | CCamera DisplayListSetup: projection, viewport, scissor, look-at, z-buffer clear, fog | 442 of 476 words differ (much of it one word's shift) |
| `engine/func_00204778.c` | heap debug view: the heap's blocks as coloured bands | 0x5B8 vs 0x618 bytes, 245 of 390 words |
| `engine/func_00209AF4.c` | expands a packed model into an F3DEX2 display-list resource | 414 of 530 words |
| `engine/func_00213A70.c` | box filter and vertex count (Rage Wars func_00278530) | 19 of 264 words (one register swap) |
| `engine/func_0021D39C.c` | draws an actor for one view | 35 of 264 words (registers only) |
| `engine/func_00220D00.c` | walks the 0xC0-byte level records | 12 of 324 words (registers only) |
| `engine/func_00221D3C.c` | builds the cached geometry display list behind a patched branch | 190 of 362 words, one word longer |
| `engine/func_002222E0.c` | per-region object pass over the world's object table | same size, 206 of 385 words |
| `engine/func_0022D2FC.c` | particle matrix build (Rage Wars Particle_BuildMatrix) | 13 of 624 words (registers in the depth scale) |
| `engine/func_00246BC0.c` | player input from the controller | 6 of 330 words (a dead reload; needs a pinned register) |
| `engine/func_00248BA8.c` | player warp / spawn | 68 of 348 words (registers only) |
| `camera/func_0027D160.c` | CCamera Advance: position, orientation, frustum and clip planes per frame | 147 of 936 words (FP registers, frustum store order) |
| `engine/func_0025CAC8.c` | AI hit callback | 7 of 346 words (FP registers in one shared formula) |
