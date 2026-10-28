# Not matching yet

C that does what the original does, written and checked against the
disassembly, but that does not yet compile to the same bytes: register
allocation and instruction order still differ. These files are not built;
the asm is. They are here so the reading of each function is not lost while
the permuter (`tools/permuter/`) or a person finishes it. When one matches,
it moves to `src/` through `tools/integrate.py` and leaves this folder.

| File | What it is | Distance when added |
|---|---|---|
| `camera/func_0027E000.c` | CCamera DisplayListSetup: projection, viewport, scissor, look-at, z-buffer clear, fog | same size, 473 of 477 words |
