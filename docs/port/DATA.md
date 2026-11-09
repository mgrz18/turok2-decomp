# The game's data as C (Phase 7, round 2, q2)

The game C files reference ~1,600 data symbols that the matching build defines in assembly
(`us/asm/data/*.s`, splat output) or by address in the linker script. Before this step,
`turok2_game` had **1,788 undefined symbols**. `tools/port/gendata.py` now defines all the data
ones in C. With module 2 dropped and a stub `main`, the link has **33 unresolved symbols**, and
all of them are on the function side (below).

```sh
# after `ninja -C <out> turok2_game` (data.cmake turns on compile_commands.json, which gendata reads)
pip install clang==19.1.7        # python bindings for the system libclang 19
python3 tools/port/gendata.py --build <out> [--cache <out>/decls.json] [-j 4]
cmake <out> && ninja -C <out> turok2_data          # port/cmake/data.cmake
python3 tools/port/linkcheck.py --build <out>      # link check + unresolved list
```

The generator takes about 1 min with 4 workers (libclang parses each of 1,5xx files twice). With
`--cache`, a rerun takes about 10 s. Output goes to `port/gen/`, which is **gitignored**: it is ROM
content (strings, tables, microcode data), like the asset archive. Every port build generates it
from its own matching ELF, `build/turok2.us.elf`.

| file | content |
|---|---|
| `port/gen/data_rodata.c` | `.code_rodata` and `.code_rodata_tail` (engine rodata and data) |
| `port/gen/data_libultra.c` | `.libultra`, `.libultra_tail`, `.ucode_copy2` data |
| `port/gen/data_virtual.c` | module-1 VM data (`.virtual`): menu descriptors, mode tables |
| `port/gen/data_misc.c` | the rest (`.code` data, `.lnk_debug`, `.pre_virtual`, microcode sections) |
| `port/gen/data_bss.c` | .bss: zero arrays, plus the aliases into them |
| `port/gen/data_abs.c` | 6 ROM offsets that the C takes the address of (`(u32)&D_14A000`), as absolute symbols |
| `port/gen/relocs.c` | `t2_data_syms[]` and `t2_n64_to_host(u32)`: N64 address to host address |
| `port/gen/data_report.txt` | counts, declaration conflicts, unresolved pointer slots |

## The approach: typed from the C declarations, host layout, real pointers

The task offered two options: (1) N64-shaped byte images with a 32-bit handle in each pointer
slot plus a handle table, or (2) typed definitions where the C type is known. I implemented (2)
for everything that has a declaration and kept part of (1) for the rest:

1. **The type comes from the game's own declarations.** libclang parses every module-1 file that
   references a missing symbol, once for `x86_64-linux-gnu` and once for `mips-linux-gnu`, with
   the same flags as the port build. The same declared type therefore has both layouts: the N64
   offsets (where to read in the ELF) and the host offsets (where to write). If declarations
   disagree, the most common one wins, and every conflict is listed in the report.
2. **Each leaf is converted.** A scalar is read big-endian and written little-endian at its host
   offset. A `long` is widened to 8 bytes with sign or zero extension. A `char` is copied as is,
   so strings stay strings. Bitfields are re-packed from MSB-first (MIPS BE) to LSB-first (host).
   Unions take their first member.
3. **A pointer leaf becomes a host pointer.** The N64 value is looked up in the ELF symbols:
   KSEG0 and the engine's `0x002xxxxx` window are folded to one physical address, and VM modules
   are separate. The slot is emitted as `(void *)((char *)&target + host_offset)`. The offset
   inside the target goes through the target's layout, so it stays correct past widened pointers.
   - A function target is emitted as `(void *)Func`: 388 functions, all defined by the game C.
   - A data target is generated too, even if the C never names it: 276 extra symbols. It is typed
     by the pointee type when the pointer points at its start.
4. **An integer leaf keeps its N64 value**, even when it holds an address (m2c `s32`/`u32`
   pointers, `0xFF01` descriptor words followed by `0x800Bxxxx`). That value is the **handle**.
   `relocs.c` maps it back: `t2_n64_to_host(0x800C2030) == gScreenWidth`. Code that q3 retypes
   from `s32` to a pointer gets a real pointer the next time the generator runs. Code that keeps
   the integer can call `t2_n64_to_host`.
5. **Symbols without a usable declaration** are copied as bytes in ROM order: 6 of the 1,602.
   Pointer targets reached through `void *` or `char *` are copied the same way.
6. **.bss**: a linker-script symbol (`SHN_ABS`, placed by address, no `.s` data behind it), or any
   symbol outside a loaded section, becomes a zero array. Its size is the declared size, or the
   distance to the next symbol for `[]`. That distance is capped at 1 MB inside RAM and 4 KB
   outside RAM: `D_8F128560` is not a RAM address.
   The bytes the ELF holds at those addresses belong to whatever splat put there (libultra code,
   strings), not to the variable. The first version read them and produced 11,800 garbage
   pointers.
7. **Symbols inside another symbol become aliases.** splat puts labels inside structs that the C
   declares whole: `gScene_m_nDrawActors` is `gScene + 0x8EC`, which is `gEngineApp + 0xD00` on
   the host, and the `gEngineApp_m_Settings_*` fields work the same way. Each becomes
   `.set alias, base + host_offset` in the base's file. The aliases are exact on ELF and on
   Mach-O (`.globl` + `.alt_entry` + `.set`, round 3: MACOS.md; `turok2_data` is built with
   `-fno-common` so the base is in a section). Only COFF still gets a 1-byte FIXME placeholder.

The generated files include no game header. Each typed symbol is a packed anonymous-layout struct
of `unsigned char bN[]` runs and `void *pN` members at the exact host offsets, aligned to the
type's host alignment. The linker does not see C types, so the game's `extern MenuDesc
gPauseMenuDesc;` binds to it. Each struct is declared before any definition, so a definition can
point anywhere in its file.

Checked on the box: a test program linked against `libturok2_data.a` reads `gScreenWidth == 320`.
`t2_n64_to_host(0x800C2030)` returns `gScreenWidth`. `gPauseMenuDesc.items` points at
`D_004394D8`, `count == 10`, and init/close/update point at `PauseMenu_Init/Close/Update`.
`&gScene_m_nDrawActors - &gScene == 0x8EC`.

### Trade-off against handles everywhere

Keeping every symbol N64-shaped, with 4-byte handles in pointer slots, would keep the bytes
identical to the N64. It would also work without libclang. But every pointer read in the game
would then need a `t2_n64_to_host()`, including the hundreds of `desc->init(...)` and
`table[i]->field` reads that the typed C already does correctly. The typed route gives those
reads real pointers today. It gives integer slots the handle, which is the N64 address, so
nothing is lost. The costs:

- **The data follows the C types.** A wrong m2c declaration produces wrong data:
  - `extern s32 D_x;` on what is really a string swaps it as words.
  - A struct declared bigger than its data runs into the next labels.

  There are 187 symbols with conflicting declarations, but only 10 differ only in signedness.
  Most of the rest are m2c `char[]` vs `s32` vs `f32` views of the same literal pool, and the
  most common declaration wins. Each typing fix in `src/` (q3) improves the data on the next run.
- The generator depends on libclang (python `clang` 19) and on a port build with
  `compile_commands.json`.
- An asset struct that the game reads in place must **not** go through this tool (64BIT.md:
  "never widen a struct that swap_spec.json describes"). That is not an issue here: these are
  statics, never asset bytes.

## Numbers (tree 55932a88 plus this commit, box x86_64)

| | count |
|---|---:|
| undefined symbols in `libturok2_game.a` (all objects) | 1,788 |
| module-2 objects left out (a `.virtual_1` function) | 629 of 3,917 |
| data symbols that module-1 C needs | 1,602 (1,596 typed, 6 as bytes) |
| extra symbols generated as pointer targets | 276 |
| .bss symbols (zero) | 807 |
| aliases inside another symbol | 216 |
| pointer slots | 2,638: 1,480 relocated, 756 null in the ROM, 402 unresolved (set to NULL) |
| functions referenced from data | 388, all defined by the game C |
| generated size | 1.23 MB host (1.16 MB N64), 7 C files, ~2 MB of source |
| **unresolved after linking module 1 + data + stub main** | **33** |

`linkcheck.py` extracts the library and drops the module-2 objects. It links with
`-no-pie -nostdlib --unresolved-symbols=ignore-all`, so the real linker still checks duplicate
definitions and relocations: **link OK**. It then classifies what remains:

| category | n | symbols | owner |
|---|---:|---|---|
| asm-only / excluded functions (`.code`) | 32 | `RNC_UnpackMethod1/2` (q1's rnc.c), `__osException __osDispatchThread __osEnqueueAndYield __osEnqueueThread __osPopThread __osCleanupThread` (t2os), `__osDisableInt __osRestoreInt osSetIntMask __osGetSR __osSetSR __osGetFpcCsr __osSetFpcCsr osGetCount __osSetCompare` (host no-ops or clock), `osInvalDCache osInvalICache osWritebackDCache osWritebackDCacheAll` (no-ops), `__osProbeTLB __osUnmapTLBAll func_0028EE40 func_0028EEC0` (TLB: no-ops), `func_002A3100 func_002A31E4 func_002A31F0 func_002A31FC` (crash handler, SR), `HostIo_Call sprintf func_00298470` (stub, libc, sqrtf) | q5 (host stubs), q1 |
| host libc | 1 | `memcpy` | libc |

That is exactly the "function-side remainder" (docs/port/BUILD.md). No data symbol is left.

## Known gaps (data_report.txt has the lists)

- **Round 3 (r3):** `RUNTIME_BSS` in gendata.py makes 27 of those runtime structs bss, and a pointer-wide integer leaf (`sptr`/`uptr`) holding a known address is now relocated, so the `*ModeTable`s come out as real pointers. Unresolved pointer slots went from 403 to 24 (extent overruns of small `.code_rodata` symbols) and address-holding integers from 334 to 125 (mostly `osViModeTable`, which holds no real pointers). The rest of this list describes the state before round 3.
- **402 pointer slots hold values that are not addresses**, in 38 symbols. Most are in large
  runtime structs that splat placed inside loaded sections: `gDemoPlayer_match_decode` (108),
  `gAudioWorld` (97), `gRumbleEntries` (48), `gDemoPlayer` (26), `sPathSearch` (24). Their
  declared size runs past the zero words into the next labels' bytes (strings, code). They are
  runtime state, probably .bss in Acclaim's layout. They are written as NULL. The right fix is
  to mark such symbols bss: they are zero at boot, or the game initializes them.
- **Integer slots that carry addresses**: the report lists 331 in 85 symbols whose value looks
  like an address (KSEG0 RAM, the engine window or module 1) and falls on a known symbol. These
  are kept as the N64 handle. The list is q3's worklist: `osViModeTable` (56: VI register words
  and framebuffer addresses; those are not real pointers), `gEnemyGroundModeTable` (37),
  `sWeaponActorModeTable` (31), `sPrimagenModeTable` (21), the other boss and platform
  `*ModeTable`s (function pointers declared as `s32`), `sCheatCodes` (12) and `gAudioWorld` (11).
  Typing them in `src/` and rerunning turns them into real pointers.
- **Aliases on COFF**: see point 7 above (Mach-O done in round 3).
- **Module 2** is skipped entirely, as the task asked: no `.virtual_1` data and no objects.
- The `void *` slots are host pointers, so the data cannot be written back as N64 bytes. Nothing
  needs to (saves go through their own code).
- `data_abs.c` (`D_14A000` and 5 others) uses absolute symbols. These need `-no-pie` on ELF, or
  the code using them should switch to archive keys (docs/assets/PORT.md).

## What the next round needs

- q5: link `turok2_data` into `turok2`, together with the host stubs for the 32 functions above
  (`RNC_*` from q1). Run to the first crash.
- (done in round 3: `RUNTIME_BSS`) Mark the runtime structs above as bss.
- q3: type the `*ModeTable` declarations (function pointers stored as `s32`), then rerun the
  generator.
- Replace the `.set` aliases with something COFF accepts before a Windows build (Mach-O: done, MACOS.md).

## Round 3 (r1): generator changes found by running the game

| change | why | count on this tree |
|---|---|---|
| offset translation through a **union** prefers the member that types the offset (not the last one) | `CGame`'s `union { CCameraPool m_CameraPool[1]; struct { pad; u8 m_MenuPool[]; }; }` and `CEngineApp`'s `m_Game` / `m_Settings` union: aliases inside the camera pool landed in the pad | aliases now on the fields (`gEngineApp_m_Game_m_CameraPool` = `&gEngineApp.m_Game.m_CameraPool`) |
| a union's data is laid out by a **scalar member as large as the union** when there is one (else the first member) | libultra's `du` (`{ struct { u32 hi, lo; } w; f64 d; }`): written by its words, a little-endian double had its halves swapped (`sinf`/`cosf` constants -> NaN) | all `du` constants |
| a **.bss symbol declared only as bytes** gets twice its N64 size, 8-aligned | `extern u8 gPoolBuffers[]` is a GfxPool of pointers (0x20 N64, 0x40 host): `GfxPool_Alloc` overwrote the next symbol | 409 |
| **static display lists**: in data typed `Gfx` (two `GfxWord`s), the w1 of G_VTX, G_MTX, G_MOVEMEM, G_DL, G_RDPHALF_1, G_SETTIMG/ZIMG/CIMG and G_MOVEWORD G_MW_SEGMENT with an RDRAM address becomes a host pointer | `gRspInitDL` loads the lights at 0x800AB0D8 | 10 lists, 4 addresses |
| a **pointer target** of a record type is typed as a **table**: as many elements as the integer stored right after the pointer says (`{ T *items; s16 count; }`, MenuDesc), else as many as fit before the next symbol; never over a symbol the C declares (it keeps its type) | menu item tables were one typed `MenuItemDesc` followed by N64 bytes | 88 tables; pointer slots 2,621 -> 4,074 (relocated 2,487) |

The report (`data_report.txt`) prints the three new counts.
