# What the Rage Wars PC port teaches the Turok 2 native build

Subject: `references/rw_junk` = thejunkman9000's *Turok: Rage Wars* decomp + Windows port
(GitHub `thejunkman9000/Turok-Rage-Wars-Decomp`, now redirected to
`JohnSams165/Turok-Rage-Wars-Decomp`; local clone = single commit `4a29f8f`, 2026-10-20).
Sources read: `port/` (all C/C++/headers, the key Python passes), `docs/PORT_PLAN.md`,
`docs/PORT_STATUS.md` (388 lines of dated bring-up notes), `port/AUDIO_HLE.md`,
`docs/hand_asm_c/`. Same engine family as Turok 2 (Acclaim, libultra + libaudio,
F3DEX2-family GBI, RNC ProPack, SN64 TLB-mapped code, Controller Pak saves).

## 0. Licence: the CC0 assumption is wrong (action needed)

| What | Finding |
|---|---|
| rw_junk repo | **No licence at all.** No `LICENSE`/`COPYING`/`UNLICENSE` file in the clone or on GitHub (raw URLs 404), GitHub API `license: null`, README has no licence section. Default copyright applies: *all rights reserved*. |
| Where "CC0" came from | rw_junk's README says the **other** decomp, melalawi/ragewars-decomp (now `AbuN64/RageWarsDecomp`), is CC0, and that rw_junk's `include/their/` + `their_syms.ld` (copied from it) are CC0. That statement does not cover rw_junk's own code. (Even AbuN64's README says "License: CC0 1.0 (LICENSE)" while the repo has no LICENSE file and API `license: null`; the README statement is the only grant.) |
| `docs/hand_asm_c/rnc.c` (-> our `port/third_party/rnc/rnc.c`) | Written by the rw_junk author ("readable C reconstruction ... verified by differential testing"), no licence header. **Not CC0; no licence.** Also derived from it: `tools/assets/propack.py` (Python port of the same decoder). |
| `port/m2c_macros.h` | Copied from m2c (`matt-kempster/m2c`, **GPL-3.0**) with edits. Do not copy it; write our own macro header (p1). |
| `RESAMPLE_LUT` in `audio_hle.c` | The aspMain microcode's resampler table (Nintendo data, also in GPL mupen64plus-rsp-hle and in sm64-port). Facts/tables, but note the provenance. |

Consequences / recommendation (decide before the port is published):
1. `docs/assets/PORT.md` §8 and `port/third_party/rnc/README.md` claimed CC0; both now say
   "no licence, unresolved" (this round).
2. Either get an explicit grant from the author (an issue on the GitHub repo asking for a
   LICENSE file; CC0 or MIT would match what we assumed), or **replace `rnc.c` with our own
   decoder** written from Turok 2's own disassembly (`func_00200C40..func_002010C0`,
   docs/ASM_ORIGIN.md) + the public RNC format. It is ~400 lines; our differential test vs
   the ROM blobs (CRC16 in `t2asset.c`) already exists to validate a rewrite. Do the same for
   `propack.py`. Until then: *nothing else from rw_junk is copied*; everything below marked
   "reusable" means "reusable once licensed, or re-implementable from the documented
   behaviour".

## 1. Architecture in one page

| Aspect | Rage Wars port | Turok 2 plan (LUS, 64-bit) |
|---|---|---|
| Code origin | Matched C for all game code (README 2026-10-11) **plus** ~800 functions statically recompiled from MIPS (`mips2c.py`, `gen_recomp.py`: `r[32]` register array, o32 ABI, FPU in `g_fpr`), 777 behaviour drafts (`port/nonmatching`), 142 hand overrides (`port/overrides`). The port predates full matching and still runs "twins" (recompiled copies) for functions whose matched C misbehaves (`twin_prefer.txt`). | 100 % matched C; no recompiler needed. The twin trick is not available, so host-correctness of matched C must be fixed in the C (see §3). |
| Target | **32-bit x86 Windows only**, clang -> LLVM IR -> Python passes -> `zig cc` (x86-windows-msvc), MSVC link. | arm64 macOS first, x86_64 Linux/Windows. |
| Pointers | **Guest address == host address.** 8 MB of RDRAM reserved at 0x80000000, globals emitted as absolute symbols at their N64 addresses (`gen/syms.asm`), low guest space 0x200000-0x2000000 moved to +0x30000000 (`XL()`, `port_xl`). If Windows already mapped one of those ranges the exe relaunches itself (ASLR). Structs keep 32-bit pointers unchanged. | Impossible on 64-bit hosts (no 32-bit address space for the data; macOS forbids low mappings). Turok 2 needs real 64-bit pointers: widen pointer fields / separate host structs (p2's audit). |
| Byte order | **Data stays big-endian.** `port/be_ir.py` rewrites every 16/32/64-bit load/store (ints, pointers, floats, doubles) in the IR into a bswap'd access; module-private constant arrays are pre-swapped; `byval_ir.py` normalises struct-by-value args; `va_arg` hacked in `compat_clang.h`; the OS shim uses `LD32/ST32/LDP/STP` for every guest field. | Data converted to host order at load (swap_spec.json, docs/assets/BYTEORDER.md). No IR rewriting. |
| Code pointers in data | `relocate_fnptrs()` (game_glue.c) scans every loaded data word in [0x200400, 0x4FFFFF] and replaces it with the host function if the value is a known function start (heuristic over the whole data image) + `port_resolve()` at run time + alias stubs for entries *inside* functions (`forced_entries.txt`). | Native C function pointers: tables in C already hold real pointers when the data is compiled C. Only pointers that live in ROM *asset* data need a table (p2). |
| TLB / VM paging | Kept: an access-violation handler (`page_in`, game_glue.c) walks the game's own region list at 0x80103F88 and commits the faulting 4 KB page from the ROM. Asset space moved to 0x30000000+ by overriding the two pager functions. | Link one copy of each VM module natively; no paging (docs/VMASM.md). |
| libultra | `os_shim.c` replaces threads, queues, timers, events, PI DMA (memcpy from ROM), VI, SP task capture, AI, SI; cache/TLB/interrupt calls are no-ops. The game's own pfs/cont code runs above a PIF emulation (`pak.c`). | Same split: game-side libultra C (pfs, cont, motor, libaudio) compiled natively; the OS layer (threads/queues/events/DMA/VI/SP/AI/SI) replaced. LUS provides parts (p3/p4). |
| Renderer | Own D3D11 **F3DEX2** display-list interpreter (`renderer.cpp`, 872 long lines): CPU-side RSP state (matrix stack, 64 vertices, lights, fog), RDP state (tiles, 4 KB TMEM emulated, TLUT, combiner -> HLSL with both cycles, othermode, scissor, fill/texrects), internal 1920x1440 target, letterboxed blit. Runs on the platform thread from an 8 MB RDRAM snapshot taken at `osSpTaskStartGo`. | LUS Fast3D (`gfx_run`), F3DEX2 GBI (Turok 2 is built with `-DF3DEX_GBI_2`; its ucode is the "2.05" NoN fifo F3DEX2-family build). renderer.cpp is a checklist of corner cases, not code to reuse (§4). |
| Audio | The **real libaudio** (alInit/alSynNew/alAudioFrame, CSP + sound player, reverb) produces ABI1 command lists; `audio_hle.c` executes them on the CPU (DMEM as a BE byte array); `audio_out.c` plays the AI buffers through waveOut. | Same idea; see §5 for the 64-bit command-word problem. Turok 2 adds adec streams (MDCT, CPU-side already). |
| Input | Raw Input + XInput + DirectInput, platform-free mapping engine `input_core.c` + `controls.ini` + a GUI mapper; "native" mouse look by overriding two game functions. | LUS ControlDeck (SDL) replaces the device + mapping layers. The mouse-look technique is the transferable part. |
| Saves | Controller Pak emulated at the SI/Joybus level (`pak.c`, 903 lines): the game's own `osPfs*` runs unchanged; 32 KiB `.mpk` files, Project64-compatible, atomic write-back. | Turok 2 calls `osPfs*` from 87 C files and `osMotor*` from 18: the same approach fits (§6). |
| Boot | `win32_main.cpp` creates window + D3D, `port_game_init` maps guest memory, copies ROM data to RAM (`load_guest_data`), relocates code pointers, starts the game's boot thread. | romMain/SNMAIN are asm (docs/ASM_ORIGIN.md); native `main` replaces them and must reproduce their side effects (§3.5). |

Verdict: the RW port is a **binary-compatible emulation of the N64 memory model on a 32-bit
host**. Almost none of its *mechanism* transfers to a 64-bit LUS port (no guest==host,
no IR byte-swapping, no recompiler, no fault-driven paging). What transfers is (a) a set of
portable subsystems (audio HLE, PIF/pak model, input mapping core, ROM check logic),
(b) a very detailed list of **bugs that matched C has on a host CPU** and of
**timing/threading traps**, and (c) its debugging methodology (oracles).

## 2. What worked (their measured results)

- Boot -> title -> all menus -> 1-4 player split-screen matches, attract mode indefinitely,
  results/next-arena loops; 5-match 4-player soaks with no crash (PORT_STATUS 2026-10-09..12).
- Audio at real time: 10 s of audio per 10 s of frames, 1 underrun (initial prime).
- Pak: 173/173 scripted pak tests, 215-check host self-test, Project64 file compatibility.
- Recompiler validated by differential tests: 1221/1226 functions agree with the matched C.
- 60 fps at 1x on the dev host; fast-forward (`TUROK_SPEED=n`) with a frame-based clock.

## 3. What was hard (and applies to Turok 2 unchanged)

These are properties of *matched C running on a non-MIPS CPU*, not of their 32-bit design.
Turok 2 will hit every one of them.

1. **Dropped argument registers** (the largest class: 72 generated + 28 hand-written
   overrides). Matching C can declare/call with fewer arguments than the callee reads,
   because on MIPS a caller's own a0..a3 are still in the registers ("pass-through"). Ex.:
   `func_002171FC` calls `state->fn110()` with no args, the handler reads a2; trampolines
   `f(void)` that forward a0..a3/v0. On x86/arm64 the callee reads garbage.
   Turok 2 exposure: **370 unprototyped declarations** (`T f();`) in `src/us/code` +
   `include`, plus every function-pointer call. Tools to port (re-implement): `argscan.py`
   (call sites passing fewer register args than the callee reads), `passthru.py` (proves from
   the caller's MIPS code that the register was never written, i.e. the original source
   forwarded it), `jalrscan.py` (same for function-pointer calls). For Turok 2 a cheaper first
   pass: compile with `-Werror=deprecated-non-prototype -Wincompatible-function-pointer-types`
   and fix every hit under `TARGET_PORT` or with full prototypes (prototypes do not change
   GCC 2.8.1 output for most calls, but verify with asm_diff).
2. **`void` functions whose v0 the caller uses** (`func_0028FDD8` returned the model size in
   v0 -> models allocated 0x70 instead of 0xA40 bytes) and **non-void functions without a
   `return`** (`func_0020BC50`: AI path edge = garbage, every bot frozen). Fix: `return x;`
   often still matches. Turok 2: build with `-Werror=return-type`; for the void-but-used
   case, scan callers that read v0 after `jal` to a void-declared callee (needs the asm,
   which we have in `asm/`).
3. **Float/integer prototype mismatches**: calls through a prototype that says `void`/`int`
   for a function returning `float` (x87 stack leak on 32-bit x86 -> indefinite NaN
   everywhere). On x86_64/arm64 there is no x87 stack, but the value is read from the wrong
   register class (rax vs xmm0, x0 vs s0): silent garbage. Their `scan_fpret.py` compares
   definition vs every prototype. Same for **float arguments** passed to an
   `int`-prototyped/unprototyped callee (o32 passes the first floats in f12/f14 only when the
   callee is known to take floats).
4. **Struct-by-value with different declarations on the two sides** (`{s32 a,b}` vs
   `{s16 a,b; s32 c}`: fire event key 0x655 arrived as sound id 0). On MIPS both are "two
   words in a0/a1"; on a host ABI they are passed differently. Turok 2: grep struct-by-value
   parameters and unify the types.
5. **Boot-path side effects skipped**: starting at the boot thread missed the cold-boot flag
   set by the asm entry (`D_8014D330 = 1`), so the whole settings block stayed zero (game
   speed 0, no items, bots dead). Turok 2: `boot_main`/SNMAIN and the C boot path
   (`osInitialize`, BSS clear, osResetType/osTvType/osMemSize) must be replicated in the
   native `main`. Make a list of every global the boot asm writes.
6. **Data inside code ranges**: two template blobs lived inside overlay A's *code* ROM range
   (`D_44B4A0`); the native build only loaded the data tail, so all collision profiles were
   zero (players walked through walls). Turok 2: check `.code` tails / VM modules for data
   that C reads by address (docs/VMASM.md's stale copy, `asm_origin` fragments) and make sure
   it becomes C data or an archive unit.
7. **UB that MIPS tolerates**: NaN -> int conversions (`cvt.w.s` vs x86 INT_MIN, arm64
   saturates to 0/limits: a third behaviour), `1 << 31`, NULL-page reads the N64 survives
   (their `softfault.c` decodes x86 instructions and skips the fault: not portable, and macOS
   has no page 0 to map; fix the sites instead, run with UBSan + ASan from day one),
   permuter-hacked sources that read uninitialised/out-of-frame data (`func_00427690`: had to
   run as a recompiled twin; Turok 2 has no twin fallback, so such sources need a
   `TARGET_PORT` readable variant).
8. **Optimiser vs memory layout**: their `-O1` folded union halves using the host's layout
   before the byte swap pass. For Turok 2 the same hazard appears as type-punning of asset
   bytes that swap_spec swapped per field (a u32 read as two u16, a word read as bytes):
   p2's endian list (d) is the place to catch them.
9. **Threading/timing** (os_shim.c, all hard-won):
   - Run game threads **one at a time** with libultra's priority semantics (their "baton");
     free-running OS threads produced races (renderer reading lists the loaders overwrite ->
     they snapshot 8 MB RDRAM per graphics task).
   - Audio tasks must complete immediately (at the scheduler's next `osRecvMesg`), not one per
     frame tick, or audio runs at 60 % speed; `osAiGetStatus` must model the AI FIFO on the
     clock (FULL when > 2 frames + host lead are queued); post the AI event every tick while
     audio is queued.
   - Completion posted from inside `osSpTaskStartGo` (before the scheduler's own bookkeeping)
     lost completions and hung the audio thread.
   - SI: re-created controller queue lost an in-flight completion (menu freeze); a blocking
     wait on the controller queue with nothing in flight is completed on the spot.
   - Report absent pads with `CONT_NO_RESPONSE_ERROR` (0x08), the bit the game tests.
   - A frame-based virtual clock (`osGetTime`/`osGetCount` advance per game frame) makes
     fast-forward and scripted tests deterministic.
10. **Splat boundary fall-through** (a function that ends in a `jal` whose return lands in
    the next symbol). Only bit their recompiler; for matched C it would already be a
    non-match. Not applicable.

## 4. Renderer: corner cases to verify in Fast3D

Each of these was a visible bug in RW and is plausible in Turok 2 (same GBI usage):
- **Colour image width changes per screen** (`G_SETCIMG` 284 in menus, 480 in matches):
  the viewport/scale must follow SETCIMG.
- **Partial z-buffer clears**: 4-way split clears each quarter with a `G_FILLRECT` to the
  z image (`F63C02C8`, colour 0xFFFCFFFC); a renderer that only handles full-screen z clears
  loses geometry. Fast3D's handling of fill-to-zimage must be checked.
- **Scissor left set** at the final blit -> only a column visible (black screens).
- **`G_MODIFYVTX` ST**: overwrites *final* S/T; the `G_TEXTURE` scale must be applied at
  vertex load, not at draw time (otherwise doubled scale: streaky pillars, black weapons).
  Turok 2 emits 51,827 modify-vertex commands (docs/assets/MODELS.md), so this matters.
- **DL nesting**: `G_DL` branch (no push) chains; RW needed depth 18 (F3DEX2 stack depth)
  and treats branches without consuming a level.
- `G_LOADTILE` with lrs < uls (negative size) at teardown -> must be a no-op; clamp
  `G_LOADBLOCK` to 4 KB TMEM.
- Self-referencing/stale DLs at state transitions -> per-frame command budget.
- Fog: `G_MW_FOG` (fm, fo) + `G_SETFOGCOLOR` live across frames; `G_FOG` replaces vertex
  alpha with `clamp(fm*z/w + fo)`; blender `G_RM_FOG_SHADE_A`/`G_RM_FOG_PRIM_A`.
- 1:1 texrects need point sampling (bilinear bleeds padding columns into edges).
- Tile `shiftS/shiftT` must be applied.
- Translucent z mode wants a small slope-scaled depth bias.

## 5. Audio

`audio_hle.c` (406 lines, libc only) is a complete ABI1 HLE whose semantics were fixed
against the game's real command streams (WAV dumps of the output): ADPCM writes a 16-sample
history prefix in front of the decoded frames (RESAMPLE reads at out+32), RESAMPLE uses the
microcode's 64-phase table on the 4 samples before the position, INTERLEAVE count = one
channel's bytes, ENVMIXER keeps an 80-byte state with exponential ramps, POLEF implemented
(reverb). Two caveats found here:
- **`port/AUDIO_HLE.md` and `port/tests/audio_hle_test.c` are stale.** The doc still says
  POLEF unimplemented and Catmull-Rom resampling; the self-test (written before the fixes)
  **fails 9 of 16 checks** when built on the box (clang 19, x86_64 Linux) because it
  expects the old semantics (no history prefix, linear ramps). The code, not the test, is
  the verified artefact.
- **32-bit addresses**: commands carry 24-bit physical addresses resolved through
  `translate(u32)`. A native 64-bit libaudio emits `Acmd` words that are host pointers, so
  either (a) widen `Acmd` to two `uintptr_t` (as LUS does for `Gfx`) and make the HLE take
  64-bit `w1`, or (b) do what sm64-port/SpaghettiKart do: turn the `abi.h` macros into direct
  calls of mixer functions (no command list at all). (b) is simpler for a native build;
  the RW HLE's *semantics* (history prefix, envmixer state, POLEF, resample table) are what to
  carry over, and they double as a reference when checking an existing mixer.c.
- DMEM is a BE byte array there; in Turok 2 the codebooks/state may be host-order after
  swap_spec, so the HLE's `be16`/`put16` would flip. Decide one convention per buffer.
- Output: 100 ms prime with hysteresis, 16 ms device blocks, 300 ms latency cap removed
  their underruns (LUS has its own audio backend; same tuning applies).

## 6. Controller Pak and rumble

`pak.c` models the SI + PIF Joybus engine: 64-byte PIF RAM transfers from
`__osSiRawStartDma` are parsed (skip/no-op/end/reset/handshake, channel 4 EEPROM empty),
controller commands identify/read/write with the 5-bit address CRC and CRC-8 (0x85),
status bits for new/removed accessory (so libultra's own `PFS_ERR_NEW_PACK` path works),
memory pak (32 KiB) vs rumble pak (0x8000 id, 0xC000 motor). Above it the game's own
`osPfsInitPak/Checker/AllocateFile/ReadWriteFile/DeleteFile` and `osMotor*` run unchanged.
Files: Project64 `.mpk` naming (`<internal title>_Cont_<n>.mpk`), DexDrive/4-in-1/`.srm`
containers, formatted image on first run, atomic temp+rename write-back 300 ms after the
last change, flush at exit and on crash.
For Turok 2 this is the right design (87 files use `osPfs*`): the game-side libultra pfs
code is matched C and compiles natively; only `__osSiRawStartDma` (+ `osContInit`/`Query`
status) must call the PIF model. Porting effort: the Joybus/CRC/format core is portable C;
replace ~100 Win32 call sites (30 critical-section, ~25 file API with wide paths, `MoveFileEx`, a writer thread, `GetTickCount`) with
std C / SDL / LUS file helpers; drop the hot-swap hotkeys or bind them to LUS menu items.
Traps they hit: a by-value `__OSInodeUnit` argument (`corrupted()`) passed differently by
two code paths (see §3.4); this SDK's `osPfsAllocateFile` leaves the status byte
uninitialised (hardware too). Does LUS already emulate the pak? Verify in p3; if it only
offers save-file helpers, keep this SI-level model.

## 7. Methodology worth copying

- **Ground-truth oracle in an emulator**: Project64 4.0 with its JS debugger API
  (`events.onwrite(AddressRange)`, `onpidma`, `onexec`, `mem.getblock`, `onpifread` to feed
  input), driven by the same scripted input as the port. Used for every "this RAM should not
  be zero" question (collision profiles, settings block, spawn key). For Turok 2 the same
  works, and with matched C we can also diff RAM regions between emulator and port at the
  same frame.
- **Deterministic oracle mode**: wait until every game thread blocks before ending a frame
  (`port_wait_idle`), frame-based clock; scripted input files; screen-id/state hooks in the
  state dispatcher to synchronise tests on game events instead of wall time.
- **Write watchpoints on the host** (x86 debug registers, `TUROK_HWWATCH`) logging NaN
  stores with backtraces; `-finstrument-functions` ring buffer of recent calls printed on a
  crash; stall watchdog that dumps every thread's queue/state.
- **Fuzzers**: random menu/level input chosen from the current screen id, with crash/stall
  classification.
- **Delta debugging** of "which sources must run as twins" (`bisect_auto.py`): for Turok 2
  the analogue is bisecting `TARGET_PORT` fixes.

## 8. Reuse table

Legend: **as-is** = drop-in once licensed; **adapt** = keep the logic, rewrite the platform
or 32-bit parts; **idea** = re-implement from the documented behaviour; **n/a** = tied to
the 32-bit guest-memory/recompiler design or replaced by LUS. Licence status of every row:
*none granted* (§0).

| File | What it does | Turok 2 |
|---|---|---|
| `docs/hand_asm_c/rnc.c` (+ `port/native/rnc*.c`) | RNC ProPack method 1/2 decoders, bounds-checked | **already copied** to `port/third_party/rnc/`; licence must be fixed or rewritten (§0) |
| `port/audio_hle.c/.h` | ABI1 aspMain HLE (ADPCM, RESAMPLE, ENVMIXER, POLEF, MIXER, INTERLEAVE, ...) | **adapt**: 64-bit command words / host-order DMEM conventions, or use as the reference for a mixer.c (§5) |
| `port/tests/audio_hle_test.c` | self-test | **stale** (9/16 fail vs current code); rewrite against verified semantics |
| `port/audio_out.c` | waveOut ring buffer, prime/hysteresis | n/a (LUS audio backend); copy the tuning numbers |
| `port/pak.c` | SI/PIF Joybus engine, memory/rumble pak, .mpk files, write-back | **adapt** (replace Win32 file/thread API) (§6) |
| `port/pak_os.c` | transliterated `__osContRamRead/Write`, `__osContGetInitData`, `osMotorInit/Start/Stop`, `osPfsIsPlug` (keeps buffers at guest addresses) | n/a: Turok 2 has these as matched C; only the hook into `__osSiRawStartDma` matters |
| `tools/pak_tool.py` | independent .mpk format tool (CRCs, notes, repair checks, containers) | **as-is** for testing saves |
| `port/tests/pak_selftest.c` | 215 host checks of CRCs/PIF/devices | **adapt** with pak.c |
| `port/os_shim.c` | libultra OS layer: baton threads, message queues on guest structs, timers, VI/SP/DP/AI/SI events, PI DMA, task capture | **idea**: the scheduling model and every timing rule in §3.9; code is Win32 + BE-guest-struct specific |
| `port/game_glue.c` | guest memory reservation, ROM->RAM data load, code-pointer relocation, TLB page-in, crash/stall/x87/watchpoint diagnostics, debug env switches | n/a (32-bit guest model); **idea**: diagnostics, cold-boot list (§3.5) |
| `port/renderer.cpp` | D3D11 F3DEX2 interpreter with TMEM/combiner/fog | n/a (Fast3D); **checklist** §4 |
| `port/win32_main.cpp`, `settings.c` | window, swap chain, fullscreen toggle, settings.ini | n/a (LUS window/config) |
| `port/input_core.c/.h` | platform-free N64 controller model, binding sources, controls.ini parser/writer, profiles, mapping engine (unit-tested) | n/a with LUS ControlDeck; **adapt** only if a non-LUS input path is wanted |
| `port/input.c` | game integration: port assignment, mouse capture, **native mouse look** via overrides of the pad->player function and the view update | **idea**: Turok 2 equivalents of `func_002631D0`/`func_002231B0` behind `TARGET_PORT`; restrict to human controller slots (their bot bug) and drop stale mouse motion (max last 4 polls) |
| `port/input_dev.c`, `config/*` | Raw Input/XInput/DInput, GUI mapper | n/a |
| `port/rom_find.c` | ROM search, .v64/.n64 normalisation, SHA-1 with named wrong releases, file dialog, copy to data dir | **idea** for the archive-builder front end (LUS ports extract on first run); Win32 UI code n/a |
| `port/softfault.c` | x86 instruction decoder that skips NULL-page faults | n/a (x86-32 only; fix sites, use sanitizers) |
| `port/oracle.c` + Project64 scripts | deterministic frames, emulator ground truth | **idea** (§7) |
| `port/be_ir.py`, `byval_ir.py`, `compat_clang.h`, `patch_*.py` | big-endian IR rewrite, by-value normalisation | n/a |
| `port/mips2c.py`, `gen_recomp.py`, `recomp_*`, `asm_*`, `rab_dis.py`, `fpsig.py`, `patch_jtbl.py` | static recompiler pipeline | n/a (Turok 2 is fully C) |
| `port/argscan.py`, `passthru.py`, `passthru_fix.py`, `jalrscan.py`, `wip/scan_fpret.py`, `wip/scan_fallthrough.py` | scanners for host-incorrect matched C | **idea, high value**: re-implement against our asm/ + prototypes (§3.1-3.3) |
| `port/m2c_macros.h` | m2c valid-syntax macros | n/a (GPL-3 origin; write our own) |
| `port/fuzz.py`, `pressfile.ps1`, `soak.ps1`, `demotest.ps1`, `lvltest.ps1`, `imgstat.py` | scripted input, fuzzing, image-share oracles | **idea** (PowerShell; rewrite in Python for the box) |
| `docs/hand_asm_c/thread.c` | readable `__osEnqueueThread` etc. | n/a (threads replaced) |
| `port/ubsan.c`, `ssp.c`, `libc_shim.c` | runtime stubs for zig/MSVC | n/a |

## 9. Decisions recommended for Turok 2

1. Resolve the rnc.c/propack.py licence now (ask the author or rewrite; §0). Record
   provenance of every third-party file in `port/third_party/*/README.md`.
2. Do not try the RW memory model (guest==host, BE in memory) on 64-bit: it needs a 32-bit
   address space. Keep the current plan (host-order data, 64-bit pointers).
3. Before the first native run, run the §3 scans on the C (unprototyped calls: 370
   declarations; void-but-used; missing returns; float prototype mismatches; by-value
   structs). They are the bulk of RW's bring-up bugs and are cheaper to find statically.
4. Port layer: a single-runnable-thread libultra scheduler (or LUS-style restructuring into
   one loop) with the audio/SI timing rules of §3.9, the game's own libaudio + a CPU audio
   mixer, the SI-level Controller Pak model.
5. Build the emulator-oracle harness (Project64 JS or mupen64plus debugger) early: matched C
   makes RAM diffs at equal frames meaningful.

## 10. What the next round needs

- A decision/answer on the licence (§0).
- p1's compile log to feed the §3.1-3.4 scanners (a clang warning pass is the cheapest form).
- An inventory of what the Turok 2 boot asm (`boot_main`, `STVM_MapTLB`) and the C boot path
  write before `main` runs (§3.5).
- Fast3D checks for §4 items once a frame renders (z-image fill rects, G_MODIFYVTX ST, SETCIMG width).
- Choice between widened-Acmd HLE and direct-call mixer for audio (§5).
