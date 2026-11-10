# Calling-convention mismatches (ABI) on the host

Matched C does not have to agree with itself across files. On MIPS o32 a caller and a callee that
disagree on the types still meet in the same argument words, so the N64 game works: a `CVector3`
by value is three words in a1..a3 or on the stack, and so are three `f32` after an integer first
argument, or three `s32` holding the floats' bits; a float after an integer first argument travels
in an integer register as its bits; a struct returned by value comes back through a hidden result
pointer in a0; a "missing" argument is whatever the caller still had in its own a0..a3. x86-64
SysV and AArch64 (AAPCS64, Apple's variant) pass these in different registers (SSE/FP vs general,
a small struct in registers, no hidden pointer for a 16-byte float struct), so the callee reads
garbage. Round 4 met three of these one crash at a time (STATUS.md M2, M6, M9). This page is the
static sweep: `tools/port/abicheck.py` finds them, and the fixes below remove the harmful classes
except the 64-bit width class.

## 1. The checker

```sh
python3 tools/port/abicheck.py --build OUT/build -j 4 [--raw raw.json] [--json findings.json] \
        [--detail [CLASS ...]] [--callee NAME ...]
```

It reads the port build's `compile_commands.json`, parses every C file of the linked targets
(`turok2_game`, `t2runtime`, `t2audio`, `t2placeholders`: 3,099 files) with libclang (the
`clang==19.1.7` bindings gendata uses; about 15 s with `-j 4`) and records:

- every function **definition**: the ABI signature of the return and each parameter (a K&R or
  `()` definition's parameters after the default promotions), variadic or not, and whether it can
  fall off its end (`-Wreturn-type`);
- every direct **call**: the converted argument types (for an unprototyped or variadic call, after
  the promotions), the return type of the declaration the caller sees, and whether the result is
  used.

It then compares each call with the callee's definition (static functions within their file). An
ABI signature is: integer (size, signedness, pointer), float (4/8), or a struct: its size, its
x86-64 eightbyte classes (INTEGER/SSE, MEMORY when larger than 16 bytes) and its AArch64 class
(HFA of 1..4 equal floats, 1-2 general registers, indirect). `--raw` caches the parsed files
(rerun the comparison in a second), `--detail` lists the sites, `--json` writes them.

| class | meaning | x86-64 / arm64 effect |
|---|---|---|
| `struct_class` | a struct by value whose classes or size differ (`Triple` vs `CVector3`, `Shared_Quad` vs `CQuaternion`) | integer vs SSE registers: this argument and every later one wrong |
| `struct_scalar` | a struct on one side, scalars or a result pointer on the other (three floats for a `CVector3`; a by-value return called as `void f(T *out, ...)`) | shifted arguments, result lost |
| `float_int` | float vs integer/pointer | rax vs xmm0, x0 vs s0 |
| `float_double` | f32 vs f64 (an f32 through an unprototyped call arrives as a double) | the low half of a double |
| `ptr_from_int` | the callee takes a pointer/64-bit value, the caller a 32-bit int (or the reverse for a return) | truncated pointer: works in Linux low memory, not on macOS (64BIT.md, MACOS.md) |
| `void_used` | the definition returns void, a caller uses the result (MIPS: v0 of its last call) | garbage |
| `noreturn_used` | a non-void definition falls off its end and a caller uses the result | garbage |
| `missing_args` | fewer arguments than the definition's parameters (MIPS pass-through) | garbage |
| `variadic` | a variadic declaration vs a non-variadic definition, or the reverse | Apple arm64 passes variadic arguments on the stack |
| `int_from_ptr`, `extra_args`, `narrow_ext`, `struct_layout` | information: truncation of a pointer the callee uses as an int, surplus arguments, s8/s16 extension, same classes with another member layout | harmless or low risk |

Not checked before round 6 (section 5 adds them): calls through function pointers (184 sites; the AI/state/impact tables: a handler's
own parameters vs what the dispatcher passes) and calls to functions without a C definition (asm,
the host runtime's C++). The impact handlers below were fixed from the definition side for that
reason.

## 2. Counts

On 98574f3c (Linux build, 3,099 files, 10,961 calls, 10,653 checked against a definition):

| class | sites (callees) before | after this round |
|---|---|---|
| struct_class | 26 (10) | **0** |
| struct_scalar | 62 (18) | **0** |
| float_int | 43 (20) | **0** |
| float_double | 0 | **0** |
| void_used | 85 (43) | **4 (3)** |
| noreturn_used | 0 | 0 |
| missing_args | 67 (41) | **7 (7)** |
| variadic | 0 | 0 |
| ptr_from_int | 496 (205) | 498 (205) |
| harmful total | 779 (294) | **510 (214)**, of which 498 are the 64-bit width class |
| info: int_from_ptr / extra_args / narrow_ext / struct_layout | 246 / 240 / 322 / 25 | 242 / 174 / 331 / 25 |

The remaining non-width sites (section 4) need a reading of the MIPS code. `ptr_from_int` is the
64-bit typing class (`s32` declarations of pointer-carrying functions): harmless on Linux while
the game lives below 2 GB, the next macOS work (genprotos.py `apply` per group, MACOS.md).

## 3. How the fixes are made

All fixes are host-only: `#ifdef TARGET_PORT` in src/ or port-only files; `tools/asm_diff.py`
reports 0 differences over all 3,938 files.

- **Caller-side adapters** (`port/game/t2abi.c`): the caller keeps its own declaration and, under
  `TARGET_PORT`, `#define`s the callee to an adapter that takes the caller's view and calls the
  definition with what the MIPS argument words meant. Words are reinterpreted bit for bit
  (`memcpy`), never converted numerically: three `f32` or three `s32` words become a `CVector3`
  (`Sound__Play_f3`/`_i3`, `AI_AngleToPoint_*`, `GameObject_DropPickupAt_i3`,
  `Collision_MoveInstance_f3`, `Quad_ContainsPoint_f3`, `Rumble_GetLevelAt_i3`,
  `GameObject_TraceMove_f3`/`_fif`), an `s32` becomes the f32 with those bits (`CMtxF__Rotate*_i`,
  `CQuaternion__Slerp_i`, `CVector3__Lerp_i`, `AI_TurnClamped_i`, `AI_TurnHeadToTarget_i`,
  `Sweep_HitCylinder_i`, `CInstancePool__SpawnAnimInstance_i`, `CLevelActorPool__Spawn_i`) or the
  reverse (`AI_SetTargetOverride_f`, `Automap_SetPlayer_f`, `Actors_GetLightVolumeFalloff_*`,
  `DLights_GetAtPoint_f3`), scalars become a struct (`Bits_ReadFloat_3`,
  `MotherBoss_AnimEvent_ev`, `Primagen_HandleAnimEvent_ev`), and a result pointer receives a
  struct returned by value (`CGameObjectInstance__GetRotation_sret`,
  `GameObject_GetGroundRotation_sret`, `GameObject_GetAimRotation_sret`). The redirect line is
  `#ifdef TARGET_PORT /* ABI ... */ #define X X_adapter #endif`, before the file's declaration.
- **Definition-side result pointers**: when (almost) every caller uses the hidden-pointer form,
  the definition is renamed `X_byval` on the host and `void X(T *out, ...)` wraps it
  (`CQuaternion__FromDirection`: 23 of 25 callers, `CGameRegion__GetNormalRaw`,
  `CVector3__ProjectKeepLength`); the by-value callers redirect to `X_byval` (func_0022A1A4).
- **The Rage Wars `Triple`** in the impact-event handlers (func_0023870C..func_00238FD4,
  func_00238134, func_00238888, func_00238958, func_002389EC, func_00238D3C, func_002638A0,
  func_00265D14): the dispatcher passes a `CVector3`, the handlers take a `Triple` and forward it
  to `CVector3` definitions. Under `TARGET_PORT` these files `#define Triple T2HostTriple` (three
  floats) after their own struct definition, so both the handler's parameter and the forwarded
  argument are float structs. `Automap_SetPlayer` and `CGameObjectInstance__LocalToWorld` (whose
  MIPS view took the vector as an `IntPair` and left the actor in a1) and `func_002156D4` (a
  struct-returning wrapper) have host bodies.
- **`void_used`**: the definitions return their last call's value on the host (tailret: a copy of
  the function under `TARGET_PORT` with `return` in front of the last call): the 25
  `EnemyAnim_*` animation pickers (the AI uses the picked animation), `CVector3__Distance`,
  `CGameObjectInstance__GetAverageScale`, `CGameObjectInstance__LoadGeometry`,
  `CPathTrack__GetCurrentPoint`, `Texture_Request(Sync)`, `CCache__FreeOldestMemEntryWithFlags`,
  `Pak_RefreshNotes`, `func_00226134`, `CScene__GetSwoosh`, `func_0021852C`, the host
  `__osPfsSelectBank` and `CAudioWorld__PlaySoundPitched`. `include/protos/cache.h` and
  `scene.h` declare the host return types under `TARGET_PORT`.
- **`missing_args`** (pass-through): a wrapper `W` whose call passes none or only its own first
  parameters, with no call before it in `W` (so a0..a3 still hold W's arguments), gets a host copy
  whose parameters extend to the callee's and whose call passes them: the 13 `PlayerDeath*_Update`,
  the 8 `EngineMode*_Draw` (`CEngineApp__DrawWorld`), `PlayerRide*_Update`, `Turret_*Update`,
  `EnemyMode_TakeCover_Update`, `WeaponActor_SetModeWrapper` (`CAI__SetMode`), `CPlayer__Warp`,
  `CScene__Construct`, `CScene__LoadLevel` (func_0042C96C), `CPlayer__LevelStart`,
  `__alSeqNextDelta` (`__alSeqReadVarLen(seq)`, libultra's code) and the rest of the FIX list (`abicheck --detail missing_args` before/after). A `(void)`
  local declaration of the callee becomes `()` under `TARGET_PORT`; a callee with float
  parameters keeps a prototype (the checker caught a `Random_Float` f32 that an unprototyped call
  would have passed as a double).

## 4. What is left

- `void_used` 4: `DLight_Spawn` (its last call, `CDLightSet__Alloc`, is void too: v0 is whatever
  that left), `Cinema_DrawOverlay` (same, `Cinema_DrawFade`), `CGameRegion__FloodSetHeight`
  (the caller reads f0). Each needs the MIPS code read to know what the value is.
- `missing_args` 7: the pass-through register is not one of the caller's own parameters, or a call
  precedes it: `CScene__GetRegionInfo` from `Sweep_GetGroundBehavior` (a1),
  `CScene__GetLight` from `DLight_Spawn`, `CGameRegion__GetCeilingNormal`, `func_002261A8`
  from `CPathTrack__StartAtNearestPoint` (a2), `CISet__GetBlock` from `func_002261A8`, and two
  calls next to an `__asm__` register pin (`GameObject_IsOnGround`, `func_00268160`).
- Function-pointer calls (184 sites): handler tables whose entries disagree with the dispatcher's
  call (like the impact handlers did). A next step for the checker: collect the address-taken
  functions per table (the gendata pointer slots know them) and compare with the call through the
  table's element type.
- `ptr_from_int` 498: the macOS truncation sweep (`genprotos.py apply`), not an x86-64 Linux
  problem today.

## 5. Round 6 (u5): indirect calls, the width class, asm labels

### Indirect calls

`abicheck.py` now follows calls through function pointers (184 sites). For each call it records
the pointer's type and where the pointer is loaded from (`var:` a global table, `rec:S.f` a struct
field, a local or parameter followed back to its load in the same function), and for every place
a function's address is taken (2,039, the gendata initializers of `turok2_data` included) the
pointer type it is converted to and the table/field it goes into. Two views, printed after the
direct-call table:

- **store**: each stored function against the prototyped pointer type it is converted to;
- **call**: each indirect call against the definitions stored where its pointer comes from
  (an unprototyped `()` pointer type with no table is not matched by type: too broad).

109 of the 184 calls find no stored function (tables reached through a struct whose stores use
another struct name, the audio/OS hooks set by the host runtime, the menu descriptors): they are
reported as a count only. Two new info classes: `struct_word` (an integer struct of <= 8 bytes
vs a 32-bit int: the same register on x86-64 and AArch64, the int is the struct's first four
bytes, what MIPS read as the first word) and `struct_layout` also for two one-register integer
structs of different sizes (`SevenBytes` vs the 8-byte event value).

Indirect harmful findings: **210 call + 4 store -> 0 call + 4 store**. Fixed (host-only):

| site | MIPS | host fix |
|---|---|---|
| `Turret/Throwable/Door/Destructible/Action/InteractiveAnim_Step` | `cb()`: the mode callback gets the step's a0/a1 | `cb(actor, ai)`, actor `void *` (was s32/sptr) |
| `CAI__CallHurt(s32, CAI *)` | `m_pfHurt()` gets (actor, ai, damage) | `(void *, CAI *, void *)`, passes the three |
| `BlindOne_UpdateFight` | phase `update()` gets the state | passes it |
| `ImpactEvent_PlaySound` (`Quad`), `func_00239350` (`IntPair, fifth, sixth`), `func_00239398` | the CVector3 + event value fill a3 and the stack words | host signature `(.., CVector3 pos, s32 value)`: the sound id came from a garbage register |
| Destructible `ActionState` view (func_00265A30/00265954) | N64 offsets 0x2C/0xF0/0xF8 into CAI | `hostview.py --apply` (host 0x30/0x130/0x140): the step and hurt callbacks were never installed |

The 4 store findings are declarations only: `Destructible_Hurt` stored into a 2-parameter
`onEvent` type (called through `CAI__CallHurt` with 3), and `TotemDefense_InitFight`
(func_00408B20) storing `TotemDefense_DrawTotem` through `s32 (**)(s32, s32, s32)` at the
raw N64 offset `arg1 + 0x234` (the whole function is N64-offset code: a hostview job).

The impact handlers that read the event value as one `s32` (`ImpactEvent_SetHotPoint`,
`ClearHotPoint`, `SetWeaponFirePos`, `func_00238134`, the new host bodies) get the struct's
first four bytes. That is the MIPS first word when the sender filled it as a word (the animation
events' `IntPair`), but a `CEventValue`/`EffectParams` built as two `s16` halves gives
`lower << 16 | upper` on a little-endian host where MIPS saw `upper << 16 | lower`: a data/endian
question for the particle and collision senders, not an ABI one (not changed).

### The width class

`tools/port/abiptrfix.py FINDINGS.json [--apply]` (FINDINGS from `abicheck --json`): for each
`ptr_from_int` argument site it retypes the parameter of the declaration the caller sees from
`s32/int/u32` to `sptr/uptr` (the same types on the N64); for a return site, the definition's
return. 115 declarations by the tool, 9 by hand (renamed libc/libultra names: `osSendMesg`,
`memset`, `strlen`; `M2C_UNK` parameters). `asm_diff` 0.

`abicheck` also resolves **asm labels** now (`s32 f_alias(s32, s32) __asm__("Menu_Update")`:
the call goes to `Menu_Update`): 60 more calls checked, which found the menu file's aliases
(fixed, see 64BIT.md part 4).

### Counts (Linux build, 3,072 TUs with turok2_data; 10,823 direct calls checked, 186 indirect)

| class | 6d04fabb | now |
|---|---:|---:|
| ptr_from_int | 185 | **4** (CCache__Deallocate(0, ...) through `void CCache__Deallocate();` in func_0041F32C: a literal 0, harmless) |
| void_used | 2 | 2 |
| missing_args | 6 | 6 (the asm-label pass shows 2 more, `MenuItem_FormatBinding` through its aliases: fixed) |
| struct_class / struct_scalar / float_int / float_double / variadic | 0 | 0 |
| harmful total | 193 | **12** (+ 4 store-side indirect, declaration-only) |
| indirect harmful (call / store) | not checked | 0 / 4 |

The `void_used` and `missing_args` sites left are the section 4 list (they need the MIPS code
read: what v0/a1/a2 hold at the call).
