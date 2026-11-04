# Structs to hoist

The readability pass (see docs/MATCHING.md, "The readability pass") named the
fields of 1,140 matched files, but where no header fit yet each file defined
its own local struct. The same object is now described many times over,
under different names and with different subsets of fields. This file lists
what to merge into `include/structs/` next; nothing here has been hoisted yet.

The tables are generated from `src/us/code/*.c` (every top-level
`typedef struct/union ... Name;`, grouped by name). "Layouts" counts the
different bodies a name has, so a name with many layouts is usually one
object seen through different subsets of fields (Player, Actor, World), but
can also be a generic name reused for unrelated things (State, Arg0, Entry,
Obj): check the offsets before merging. Pairs like func_00413230 /
func_0044C230 are the two copies of a VM module and will always repeat.

## include/structs/player.h (fixed and hoisted)

The header was 4 and then 8 bytes high between 0xA08 and the HUD. Fixed
against the code: CPlayerLoadSaveData is 0x4E8 (no m_dwLevelMissionObjFlags;
the nuke parts start at 0x4DC), CPlayer has no unk_0xA2C and no
m_bLevelStarting, and the 8 bytes were the alignment of libultra's Mtx (a
long long inside): CPlayerOverlay has its matrices at 0x10 and is 0xC0, sits
at 0xC10 after a pad, and CPlayer is 0x1328 (arrays of players step 0x1328).
m_WeaponList is s8, as every reader loads it.

player.h now also has CPlayerOptions (CPlayer+0x518; D_8011AB18 and the
four at D_8011AB58 inside the game options block), CBarrel (the weapon wheel
at 0xCD0, the fields the code reads), CPlayerList / CPlayerCList (the player
list D_80119870; D_80119890 is its active list). The local Player,
PlayerRaw, PlayerOptions, PlayerList, PlayerState/PlayerBlob (the save blob,
now CPlayerLoadSaveData) and the Weapon/Owner/Zoom/Bob/Camera... player
views were replaced in about 230 files. Still local, and why:

- the m_CharacterActor.m_CurrentAnimation.m_bCycleCompleted byte (0xE2) is
  read signed by its files: game_object.h has it u8, so they cast `(s8)`;
- func_00248340: stores integer 0 to the float fields from 0xA10 (sw $zero
  where a float store would keep 0.0f in a register);
- func_00229D10, func_00250430, func_002532B4, func_00455298,
  func_0024C47C (HudPlayer/Strokes/Player): typed differently from the
  header in ways that change the code;
- func_0021AE6C (its "Player" is the camera), func_00238914, func_0024B88C,
  func_00256738, func_00271DB4, func_0041CA00, func_00455960, func_00455D10
  (imported Rage Wars files, no common.h, or a forward `struct Player`);
- func_004085A0, func_0040BFF0 (their own CAI clashes with game_object.h);
- func_0024C47C, func_00248720, func_00420454, func_0042DE74, func_004567B0,
  func_00459374, func_00466D94 (struct copies between a local Vec/blob type
  and the header's), func_00247FB0, func_0024E9D0, func_00255B20,
  func_002584DC, func_00407CB0, func_00407F20, func_0042DC18, func_00466B38,
  func_004595D8 (an array spanning several header fields, e.g. 0x9D0[10]
  over the totem and boss flags), func_00451320, func_00451368;
- MenuData/MenuOptions (the menus' player at 0x518 points into the game
  options block D_8011AAD8, not at a CPlayerOptions): left to that family.

## The families to merge first

- **CIntelligence**: the type of `header.m_pIntelligence`
  (`include/structs/game_object.h` only forward-declares it). 116 files cast
  it; local views are CIntelligence, CIntelligenceSub (18 files),
  IntelligenceTable (15), Intelligence (4), and the AI/CAI structs (51 and
  69 files) for the per-actor AI state. The animation masks are 18 `u16`
  from 0x78 (`m_AnimMasks`, with halfword parameters from 0x76 handed to
  func_00245A98), the same in 18 files.
- ~~Player, PlayerRaw, PlayerOptions, PlayerList~~: done (above).
- **Actor** (156 files), ActorDef (94), LevelActor (63), ActorInfo (19),
  ActorDefinitionHeader (18): CGameObjectInstance and its definition.
  PARTLY DONE: 123 files now use CGameObjectInstance instead of a local
  Actor/LevelActor (m_nTypeFlag is u16: every reader uses lhu). Still local:
  ~30 files that also define a local `CAI` (wait for the CAI/CIntelligence
  merge), 11 files reading a field at 0x28C (a subclass past the 0x288
  instance), views of other objects named Actor (flags at 0x40, fields at
  0x52A...), and a few whose views need lvalue casts or did not match.
- **World** (50 files, 43 layouts), Shared_World (20), WorldSector (15),
  Level (24): `D_800F7078`, the level and its region graph; fields seen from
  0x18FA8 on (warp group, mode, pending, spawn key, warp ID).
- **MenuSettings** (38), GameOptions (12), GameSetup (18), GameFlags (16),
  MenuOptions (17): all `D_8011AAD8`, the game options block, declared under
  10 different local types in 108 files (table below).
- **MatchState** (17), GameState (21), PlayClock: `D_8011ACB0`, the match
  state, 8 local types in 22 files.
- **SndMgr** (9 files, 9 layouts), SndVoice (4), Voice (7), SndCtx (5),
  SndLock (4), SoundBank (5), SoundDef/SoundEffectInfo: the sound manager
  and its voices.
- **Node** (13 files), Region (30), RegionInfo (15), Triangle (18): the
  region graph and its collision triangles (`D_800B2608`).
- **MenuItem** (115 files, 26 layouts), MenuItemTmpl (36), MenuEvent (35),
  MenuCaller (32), MenuController (18), MenuPad (16): the menu system.
- **libultra copies**: OSThread (28), OSMesgQueue (32), __OSThreadContext,
  __OSfp, OSPfs (14), Gfx (23): these belong to the libultra/gbi headers.
- **Vectors**: Vec3f (166 files), Vec3 (104), V3 (22), Quat (18): all
  `CVector3`/`CQuaternion`, which `include/` already has.
  DONE: `include/structs/vector.h` (CVector3, CQuaternion, CMtxF; included
  by game_object.h). Vec3f/Vec3/V3/Point3/M04Vec3/B19Vec3/Quat/Quatf/Quat4
  are gone from src/us/code except in files that take Vec3 from
  `include/rw/abu_types.h` (imported Rage Wars code, left alone). V3 (s32
  fields) was only ever copied or passed by value, so CVector3 compiles the same.

## Globals declared under several local types

Each row is a global that 3 or more files declare with a struct type of
their own (top 40 of 63).

| Global | Files | Local types (files) |
|---|---|---|
| D_8011AAD8 | 108 | MenuSettings (37), GameSetup (16), GameFlags (14), GameOptions (11), MenuOptions (5), Shared_Game (5), Settings (3), MatchSettings (2), ... |
| D_800AF618 | 44 | WeaponDef (26), WeaponInfo (14), SlotDef (1), Entry (1), KeyInfo (1), EntrySet (1) |
| D_8012FB70 | 43 | State (43) |
| D_800D8E24 | 27 | Gfx (20), DlCmd (2), GfxWords (2), GfxWords_414938 (2), P4Gfx (1) |
| D_800C1BB0 | 24 | State (7), Unk800C1BB0 (5), CueContext (4), Arg (4), Cinema (3), State800C1BB0 (1) |
| D_8010FEB4 | 23 | Actor (9), LevelActor (6), Enemy (3), Enemy0040457C (2), Actor004035F8 (1), ActorNode (1), ListedObject (1) |
| D_800F7078 | 22 | World (9), Shared_World (6), Level (3), M5Grid (2), M04World (1), B18Mgr (1) |
| D_8011ACB0 | 22 | MatchState (7), GameState (4), PlayClock (2), Unk8011ACB0 (2), SessionState (1), State8011ACB0 (1), S10Game (1), Score8011ACB0 (1), ... |
| D_80119870 | 21 | Game (9), Game80119870 (4), Players (2), PlayerList (2), Game004001D0 (1), Game0040336C (1), World (1), Shared_World (1) |
| D_8012F608 | 20 | BossModule (7), ModeState00403CF8 (2), ModeState00403C14 (2), ModeState0040457C (2), BossState (1), ModeState00404450 (1), ModeState0043CF3C (1), ModeState00403F3C (1), ... |
| D_0043232C | 17 | Command (12), PhaseFuncs (5) |
| D_800C2224 | 17 | PakDirectory (10), Block (2), O01PadTab (1), Bank (1), PakCache (1), Bank426978 (1), SlotBank (1) |
| D_800F1CE0 | 17 | CollisionResult (6), Actor (4), PlayerState (2), CollisionState (1), BlastOverride (1), SpawnSource (1), Player (1), BlastActor (1) |
| D_0043240C | 16 | Command (11), PhaseFuncs (4), PhaseFuncs_40CCC0 (1) |
| D_800F56B0 | 16 | TimerState (7), State800F56B0 (3), Unk800F56B0 (2), MatchState (2), SeqState (1), State (1) |
| D_800F5EC0 | 16 | Controller (6), Ctl (3), PadState (3), ControllerProfile (2), SetupSlot (1), B14Ctl (1) |
| D_80119890 | 16 | Player (5), GameState (3), CameraPlayer (2), Actor (2), PlayerEntry (1), Player_40CFC4 (1), M04Node (1), ProjNode (1) |
| D_80130C80 | 15 | PfsPak (13), OSPfs (1), PfsHandle (1) |
| D_80130200 | 13 | State2 (8), Counters (2), Stats80130200 (1), TrailSlot (1), State2_40C850 (1) |
| D_800F1ADC | 12 | HitTarget (5), HitBuffer (2), CollisionOut (2), CollisionResult (1), HitInst (1), Listener (1) |
| D_800F7070 | 12 | GfxTask (8), FrameBuffers (2), Unk800F7070 (1), FrameRef (1) |
| D_8011AB58 | 12 | SetupSlot (11), PlayerSettings (1) |
| D_800C1FB0 | 10 | NameEntry (7), NameEdit (2), O06R (1) |
| D_800F1DB8 | 10 | Vec3f (6), Point3 (4) |
| D_800B2608 | 9 | Triangle (9) |
| D_800F2020 | 9 | Vec3f (4), Vec3 (4), CVector3 (1) |
| D_800C201C | 8 | O01Cfg (2), SaveBuffer (2), SaveGame (1), PlayTime (1), MenuOwner (1), SaveData (1) |
| D_8011AB18 | 8 | PlayerOptions (4), MenuOptions (3), Settings (1) |
| D_80130968 | 7 | O01Save (4), K7Blk (2), O01SaveSrc (1) |
| D_8011AAF1 | 6 | GameMode (5), Settings (1) |
| D_80128370 | 6 | OSMesgQueue (6) |
| D_80128540 | 6 | OSMesgQueue (6) |
| D_8012F3A0 | 6 | State004028F4 (1), Boss (1), State0040328C (1), State00403660 (1), State00402FA0 (1), State (1) |
| D_8012F9D8 | 6 | BossState (5), State (1) |
| D_800F5A10 | 5 | Bounds (4), CBox (1) |
| D_801198B0 | 5 | Game (3), MenuMatchGlobals (2) |
| D_8011AC88 | 5 | GameState (4), Cinema004001D0 (1) |
| D_801206A0 | 5 | Pager (5) |
| D_8012F9DC | 5 | BossMode (3), BossState (2) |
| D_800B8920 | 4 | OSThread (3), Unk0029B150 (1) |

## Every local struct name defined in 2 or more files

717 of the 1,699 local typedef names. "Fields" are the named offsets of the
most common layout (pads left out); a body without offset comments is shown
as written.

| Name | Files | Layouts | Fields | Files |
|---|---|---|---|---|
| Player | 182 | 145 | 0x518 *options | func_0021AE6C, func_0021EF40, func_002222E0, func_00229D10 +178 |
| Vec3f | 166 | 2 | `f32 x, y, z;` | func_0020F0C8, func_00210090, func_00211F64, func_0021285C +162 |
| Actor | 156 | 109 | 0xD4 flags | func_00219F74, func_0021AE6C, func_0021D7BC, func_0021D928 +152 |
| MenuItem | 115 | 26 | `u8 pad0[0x14]; char *text;` | func_0024AF68, func_0024B3E4, func_00254D5C, func_00417910 +111 |
| Vec3 | 104 | 2 | `f32 x, y, z;` | func_00212DA4, func_002130A8, func_00214564, func_00214B58 +100 |
| ActorDef | 94 | 48 | 0x6A animFlags[25] | func_002233A8, func_00223A94, func_00223F8C, func_00224ED0 +90 |
| CAI | 69 | 30 | 0xC7 animDone | func_0025AD90, func_0025BA80, func_0025BC70, func_0025BE88 +65 |
| State | 66 | 35 | 0x0 *obj, 0x4 count, 0x8 *actors[4], 0x18 slots[15], 0x65C phase, 0x660 activeMask, 0x664 unk664, 0x674 unk674, ... | func_002596B4, func_00259794, func_00259878, func_00259AE0 +62 |
| LevelActor | 63 | 24 | 0x234 *callback234, 0x248 *callback248, 0x250 unk250, 0x254 unk254 | func_00220260, func_00220408, func_00221210, func_00221A24 +59 |
| Targets | 56 | 7 | 0x0 a, 0x44 b | func_0025D030, func_0025D474, func_0025DBD8, func_0025DDBC +52 |
| AI | 51 | 47 | 0x36 unk36, 0xC7 unkC7 | func_00241960, func_00257EA0, func_00258000, func_0025B2D4 +47 |
| World | 50 | 43 | 0x18FA8 group, 0x18FAC warpMode, 0x18FB0 warpPending, 0x18FB4 unk18FB4, 0x18FB8 spawnsReady, 0x18FD0 spawnKey, 0x18FD4 warpID, 0x18FD8 unk18FD8, ... | func_00219BC0, func_00220530, func_00220A1C, func_00220D00 +46 |
| Slot | 48 | 12 | 0x0 unk0 | func_00236444, func_00271DB4, func_00293530, func_00401E30 +44 |
| Target | 47 | 22 | 0x0 unk0, 0x4 unk4, 0x8 dist | func_002298C4, func_0022AFB4, func_002325F0, func_00232684 +43 |
| PlayerOptions | 41 | 25 | 0x29 unk29, 0x2A unk2A | func_0023ECF8, func_00247C10, func_00247FB0, func_00248340 +37 |
| MenuSettings | 38 | 9 | 0x20 unk20, 0x21 unk21, 0x22 unk22, 0x23 unk23, 0x24 unk24, 0x25 unk25, 0x26 unk26, 0x27 unk27, ... | func_002237C4, func_00418BEC, func_00418D4C, func_00418E5C +34 |
| MenuItemTmpl | 36 | 3 | 0x14 *text | func_00417DD4, func_00417E28, func_00417F9C, func_004184A8 +32 |
| Timer | 36 | 5 | 0x0 *proc, 0x4 unk4, 0x8 time, 0xC unkC, 0x10 unk10, 0x14 unk14 | func_0040E46C, func_0040E59C, func_0040E648, func_0040E72C +32 |
| MenuEvent | 35 | 14 | 0x20 *controller | func_00421CA0, func_00422F14, func_00422FC0, func_00423040 +31 |
| MenuCaller | 32 | 13 | 0x1C *player | func_00413230, func_00417F9C, func_00418294, func_0041A3B0 +28 |
| OSMesgQueue | 32 | 2 | `OSThread *mtqueue; OSThread *fullqueue; s32 validCount; s32 first; s32...` | func_00202BB0, func_0026FB70, func_002706A0, func_00270A60 +28 |
| Region | 30 | 21 | `s32 type; u8 pad4[0xA]; s8 set0[2]; s8 set1[2]; u8 pad12[0x51]; s8 sta...` | func_0021D39C, func_0021D7BC, func_0021D928, func_0021DA8C +26 |
| OSThread | 28 | 3 | `struct OSThread_s *next; OSPri priority; struct OSThread_s **queue; st...` | func_00202BB0, func_0026FB70, func_002706A0, func_00270A60 +24 |
| Obj | 28 | 19 | 0x140 ai | func_0021F080, func_0021F4E8, func_0021F668, func_002325F0 +24 |
| Mover | 27 | 13 | 0x0 unk0 | func_00215868, func_00231EF8, func_00239F00, func_0023AB18 +23 |
| WeaponDef | 27 | 16 | 0x20 *ammo[3] | func_00247660, func_00247FB0, func_002484F8, func_00248720 +23 |
| Controller | 26 | 15 | `u8 pad0[0xC0]; u32 bits;` | func_00241CB0, func_00241E30, func_00241FC8, func_00242284 +22 |
| Level | 24 | 21 | 0x6C *unk70 | func_002233A8, func_00223F8C, func_00224180, func_002248D4 +20 |
| Manager | 24 | 3 | 0x0 *leader, 0x4 countA, 0x8 *groupA[4], 0x18 countB, 0x1C *groupB[4], 0x2C countC, 0x4F8 *script, 0x4FC command, ... | func_0020367C, func_0040E46C, func_0040E59C, func_0040E648 +20 |
| O06A | 24 | 4 | `u8 pad[8]; u32 unk8; u8 pad2[8]; void *unk14;` | func_0041BF08, func_0041BF6C, func_0041CE6C, func_0041CF68 +20 |
| PlayerList | 24 | 8 | 0x20 *head | func_00251FC4, func_0025247C, func_002525BC, func_00252840 +20 |
| __OSThreadContext | 24 | 2 | `u64 at, v0, v1, a0, a1, a2, a3; u64 t0, t1, t2, t3, t4, t5, t6, t7; u6...` | func_00202BB0, func_0026FB70, func_002706A0, func_00270A60 +20 |
| __OSThreadprofile_s | 24 | 1 | `u32 flag; u32 count; u64 time;` | func_00202BB0, func_0026FB70, func_002706A0, func_00270A60 +20 |
| __OSfp | 24 | 2 | `struct { f32 f_odd; f32 f_even; } f;` | func_00202BB0, func_0026FB70, func_002706A0, func_00270A60 +20 |
| Command | 23 | 1 | 0x0 *m), 0x4 unk4 | func_0040E46C, func_0040E59C, func_0040E648, func_0040E72C +19 |
| Gfx | 23 | 4 | `u32 w0; u32 w1;` | func_0020A33C, func_0020A994, func_0020AF08, func_0020B258 +19 |
| Owner | 23 | 21 | 0x54C unk54C | func_0021AE6C, func_0021ECA0, func_0021ED04, func_00224DF4 +19 |
| V3 | 22 | 1 | `s32 x, y, z;` | func_00206920, func_0020EF60, func_0021810C, func_00218218 +18 |
| GameState | 21 | 14 | `s32 state;` | func_00214844, func_00248BA8, func_00249C20, func_0025295C +17 |
| Arg0 | 20 | 19 | 0x4 unk4, 0xA unkA | func_00204DB8, func_002242F8, func_00226390, func_00236314 +16 |
| Shared_World | 20 | 11 | `s32 mode; u8 unknown4[8]; s32 challengeFile; s32 challengeFileSize; vo...` | func_00214108, func_00220260, func_00220408, func_0022425C +16 |
| Sweep | 20 | 16 | 0x0 *inst, 0x4 *def, 0x4C endX, 0x50 endY, 0x54 endZ | func_00239F00, func_0023A640, func_0023A968, func_0023AB18 +16 |
| ActorInfo | 19 | 12 | `u32 flags; s32 unk4; u8 pad8[4]; s16 unkC; u8 padE[6]; f32 unk14; u8 p...` | func_00221210, func_002233A8, func_00223F8C, func_00224ED0 +15 |
| Quad | 19 | 4 | 0x0 normal | func_00238114, func_0023C678, func_0023C730, func_0023C7E8 +15 |
| ActorDefinitionHeader | 18 | 1 | `s32 kind; s32 gate; u8 pad8[4]; s16 character; s8 first; u8 padF; s8 s...` | func_0021E5D4, func_0021E840, func_0021F428, func_0022425C +14 |
| CIntelligenceSub | 18 | 1 | 0x0 flags, 0x8 id | func_00259F10, func_00259FEC, func_0025A0C8, func_0025A1A4 +14 |
| GameSetup | 18 | 13 | 0x25 minutes, 0x26 unk26, 0x27 level, 0x80 slots[4], 0x1D8 match | func_00417490, func_0041760C, func_0041778C, func_004184A8 +14 |
| MenuController | 18 | 2 | 0x4 port | func_00413230, func_00421CA0, func_00422F14, func_00422FC0 +14 |
| Options | 18 | 8 | 0x3E tagMode | func_0024C47C, func_00250EC0, func_00251FC4, func_002554D0 +14 |
| Quat | 18 | 1 | `f32 x, y, z, w;` | func_0021AE6C, func_0022A4C8, func_0022D2FC, func_002374AC +14 |
| Triangle | 18 | 9 | 0x4 *v[3] | func_00268B98, func_00269DF0, func_00269F7C, func_0026A268 +14 |
| MatchState | 17 | 9 | 0x14 time, 0x18 unk18, 0x1C unk1C, 0x20 unk20, 0x24 unk24, 0x28 unk28, 0x2C unk2C, 0x30 unk30, ... | func_00251FC4, func_0025492C, func_002886D0, func_00417490 +13 |
| MenuOptions | 17 | 2 | 0x25 opt25, 0x28 opt28, 0x2B opt2B, 0x2C opt2C, 0x2D opt2D, 0x2E opt2E, 0x30 opt30 | func_00418090, func_004181A4, func_004183C0, func_004186FC +13 |
| Obj2 | 17 | 4 | 0x140 ai[1] | func_00223F8C, func_00224ED0, func_00224FC0, func_00226024 +13 |
| Particle | 17 | 14 | 0x10 *region, 0x18 velocity, 0x40 flags, 0xF8 *desc, 0x10C *owner, 0x110 ownerId, 0x114 target, 0x168 rotSpeed, ... | func_00229340, func_0022AAE0, func_0022AFB4, func_0022BA88 +13 |
| DamageEvent | 16 | 6 | 0x0 data[0x18] | func_00223A94, func_0024C47C, func_00251E28, func_0025FAD0 +12 |
| Entry | 16 | 15 | 0x0 *next, 0x4 *prev, 0x8 a, 0xC b, 0x10 c, 0x14 value, 0x18 lists[3] | func_00214188, func_002188AC, func_0022D2FC, func_00256B34 +12 |
| GameFlags | 16 | 9 | 0x19 multiplayer | func_00262D74, func_0027D160, func_002892D8, func_00407CB0 +12 |
| MenuPad | 16 | 3 | `u8 pad0[4]; s8 pad;` | func_0041E500, func_0041EA84, func_0041F0C8, func_0041FCF4 +12 |
| O01Flags | 16 | 1 | `u8 pad0[0xA8]; u32 unkA8; u8 padAC[0x24]; u32 unkD0; u8 padD4[0x4C]; u...` | func_00421E78, func_00422E8C, func_0045AD98, func_0045B784 +12 |
| O01Item | 16 | 1 | `u8 pad0[0xC]; O01Flags *unkC; u8 pad10[4]; O01Text *unk14;` | func_00421E78, func_00422E8C, func_0045AD98, func_0045B784 +12 |
| O01Text | 16 | 1 | `char *buf;` | func_00421E78, func_00422E8C, func_0045AD98, func_0045B784 +12 |
| Game | 15 | 10 | 0x40 hud[0x13D8], 0x1418 mode | func_002237C4, func_002478C8, func_00249358, func_00266380 +11 |
| IntelligenceTable | 15 | 1 | 0x76 unk76, 0x78 unk78, 0x7A unk7A, 0x7C unk7C, 0x7E unk7E, 0x80 unk80, 0x82 unk82, 0x84 unk84, ... | func_00259E60, func_00259F3C, func_0025A018, func_0025A0F4 +11 |
| PhaseFuncs | 15 | 1 | 0x0 *state), 0x4 *state) | func_0040D0F0, func_0040D3A8, func_0040D4C8, func_0040D5C4 +11 |
| RegionInfo | 15 | 13 | 0x44 flags | func_00239F00, func_0023B664, func_0023BE24, func_00242E34 +11 |
| ScriptStep | 15 | 1 | 0x0 id, 0x4 unk4, 0x8 unk8 | func_0040D0F0, func_0040D3A8, func_0040D4C8, func_0040D5C4 +11 |
| Slot2 | 15 | 1 | 0x0 data[0x64] | func_0040D0F0, func_0040D3A8, func_0040D4C8, func_0040D5C4 +11 |
| State2 | 15 | 4 | 0x0 *obj, 0x4 count1, 0x8 *actors1[4], 0x18 count2, 0x1C *actors2[4], 0x2C count3, 0x30 *actors3[3], 0x3C slots[12], ... | func_0040D0F0, func_0040D3A8, func_0040D4C8, func_0040D5C4 +11 |
| WorldSector | 15 | 1 | `void *resource; f32 *bounds; u16 ids[2];` | func_00214108, func_0022425C, func_002242B4, func_00225210 +11 |
| Body | 14 | 11 | 0x4 pos, 0x50 yaw | func_0024EAF8, func_0024ED54, func_0024EF70, func_0024F174 +10 |
| OSPfs | 14 | 1 | `s32 status; OSMesgQueue *queue; s32 channel; u8 id[32]; u8 label[32]; ...` | func_002847E0, func_00427780, func_00427E60, func_00428CF0 +10 |
| SetupSlot | 14 | 5 | 0x28 active, 0x2F index, 0x30 unk30 | func_00417490, func_0041760C, func_0041778C, func_0041BA50 +10 |
| VecDst | 14 | 9 | 0x0 unk0, 0x4 unk4, 0x8 unk8 | func_00246108, func_00246A74, func_0025E7E0, func_0025F3A4 +10 |
| WeaponInfo | 14 | 11 | 0x6 icon | func_0024B3E4, func_0024BAD4, func_0024C0B4, func_0024C2A0 +10 |
| WeaponState | 14 | 10 | 0x2C unk_2C, 0x108 unk_108, 0x10C unk_10C, 0x110 unk_110, 0x118 unk_118, 0x128 unk_128 | func_002581C4, func_00258370, func_002584DC, func_00258668 +10 |
| Camera | 13 | 9 | 0x114 pos | func_0022D2FC, func_0023118C, func_00248720, func_00251E28 +9 |
| Def | 13 | 9 | 0x14 params | func_00215868, func_002325F0, func_00232684, func_0023C188 +9 |
| Node | 13 | 11 | 0x0 id, 0x2 flags, 0x10 *link0, 0x14 *link1, 0x18 *link2 | func_00227780, func_00227AE0, func_002316F4, func_0026C678 +9 |
| PfsPak | 13 | 1 | `u8 pad[0x68];` | func_00425BC0, func_00426064, func_00426164, func_00426434 +9 |
| GameOptions | 12 | 5 | 0x21 unk21, 0x22 unk22, 0x28 unk28, 0x29 unk29, 0x2E unk2E | func_00263594, func_00418530, func_0041880C, func_00418FD0 +8 |
| MenuOwner | 12 | 6 | `u8 pad0[0x1C]; s32 player; MenuPad *pad;` | func_0041E500, func_0041EA84, func_0041F0C8, func_0041FCF4 +8 |
| Point3 | 12 | 1 | `f32 x, y, z;` | func_002318D4, func_00231A00, func_00231B2C, func_00231C58 +8 |
| Settings | 12 | 12 | `char pad0[0x19]; u8 multiplayer; char pad1A[0x23 - 0x1A]; s8 levelA; c...` | func_002427E0, func_00249358, func_0024C47C, func_00254044 +8 |
| GfxTask | 11 | 7 | 0x114 *commands | func_0020A33C, func_0020AF08, func_0020B258, func_0022D2FC +7 |
| IntrusiveList | 11 | 2 | `void *head; void *tail; s32 prevOffset; s32 nextOffset; s32 count;` | func_0020367C, func_00227C10, func_00227EE8, func_00232CD8 +7 |
| PakDirectory | 11 | 1 | 0x0 free, 0x4 notes[16] | func_00425E74, func_00426064, func_00426434, func_00426480 +7 |
| PakNote | 11 | 1 | 0x0 file_size, 0x4 game_code, 0x8 company_code, 0xA ext_name[4], 0xE game_name[16] | func_00425E74, func_00426064, func_00426434, func_00426480 +7 |
| Quatf | 11 | 1 | `f32 x, y, z, w;` | func_0021B95C, func_00229340, func_002298C4, func_00229D10 +7 |
| SweepDef | 11 | 9 | 0x0 flags, 0x4 kind | func_0023BE24, func_0023D4D4, func_0023DF70, func_0023E4A0 +7 |
| BossState | 10 | 8 | 0x0 state, 0x4 mode, 0x9 scriptOn, 0xC timer, 0x2C index | func_004056DC, func_00405E04, func_00407284, func_00407374 +6 |
| CVector3 | 10 | 2 | `f32 x, y, z;` | func_0021ABC8, func_0022782C, func_002327F4, func_00238914 +6 |
| Menu | 10 | 8 | 0xC *item | func_0041CD7C, func_0041D050, func_0041DA28, func_0041E1D8 +6 |
| O06B | 10 | 7 | `u8 pad[0x518]; O06C *options;` | func_0041BF08, func_0041BF6C, func_0041CE6C, func_0041CF68 +6 |
| Object | 10 | 8 | 0x1A8 *owner | func_00221D3C, func_002222E0, func_0023801C, func_00256B84 +6 |
| BossModule | 9 | 7 | 0x0 stage, 0xC speed, 0x36C *partA, 0x8012F974 *partB, 0x3CC unk3CC, 0x8012F9D4 state, 0x8012F9D8 mode, 0x3D9 scriptOn, ... | func_00405958, func_00405C68, func_00406000, func_00406598 +5 |
| MenuData | 9 | 2 | 0x514 unk514, 0x518 *options, 0x51C unk51C, 0x92C unk92C, 0xA50 unkA50, 0xBF4 unkBF4, 0xBF8 unkBF8 | func_00418090, func_004183C0, func_00419788, func_0041C034 +5 |
| MenuWidget | 9 | 2 | 0x34 items[14] | func_00418090, func_004183C0, func_00419788, func_0041C034 +5 |
| PoolActor | 9 | 6 | 0x4 pos | func_00227240, func_002274A4, func_002279A4, func_0023F05C +5 |
| SndMgr | 9 | 9 | 0x24 playerCfg[3], 0x7C *bank, 0x80 *bankRes, 0x84 player[0x54], 0xD8 unkD8, 0xDC snd[16], 0x102 unk102, 0x104 unk104, ... | func_00270240, func_00270550, func_00270C80, func_00271514 +5 |
| SpawnPoint | 9 | 3 | 0x0 pos | func_0025247C, func_00252840, func_002540E4, func_00409500 +5 |
| Steer | 9 | 4 | `u8 pad0[0x40];` | func_00260634, func_00260730, func_00260A34, func_00260D60 +5 |
| Tuning | 9 | 5 | 0x0 unk0, 0x4 scale | func_0024F174, func_0024F658, func_002508D0, func_00250BF0 +5 |
| WeaponMenu | 9 | 9 | 0x0 active, 0x8 open, 0x14 phase, 0x18 count, 0x1C slots[36], 0x37C cursor, 0x380 wheel | func_0024AF68, func_0024B3E4, func_0024B5A0, func_0024BAD4 +5 |
| WeaponPlayer | 9 | 8 | 0x51C *object, 0x996 weapon, 0xA64 flags, 0x1284 unk_1284, 0x1288 unk_1288, 0x128C unk_128C | func_002581C4, func_00258370, func_002584DC, func_0025883C +5 |
| Arg | 8 | 8 | 0x4 unk4 | func_00276838, func_00276880, func_002840EC, func_00284358 +4 |
| Box | 8 | 3 | 0x0 min, 0xC max | func_0021E374, func_0022AFB4, func_0023118C, func_0023A640 +4 |
| MenuContext | 8 | 4 | 0x1C *player | func_00450F4C, func_00451804, func_00453310, func_00455298 +4 |
| MenuPage | 8 | 1 | 0x0 state, 0xC *widget, 0x1C *data | func_00418090, func_004183C0, func_00419788, func_0041C034 +4 |
| MenuSlot | 8 | 4 | 0x0 weapon, 0x4 *info, 0x8 available, 0xC angle, 0x10 lift, 0x14 scale | func_0024AF68, func_0024B3E4, func_0024B5A0, func_0024BAD4 +4 |
| MenuWindow | 8 | 6 | 0x0 id, 0x30 fade, 0x34 alpha, 0x38 visible, 0x3C highlighted | func_00413230, func_0041A904, func_0041D050, func_0041D51C +4 |
| Mtx44 | 8 | 2 | `f32 m[4][4];` | func_0021E03C, func_0021E374, func_0022A4C8, func_0022D2FC +4 |
| ObjDef | 8 | 7 | 0x0 type, 0x18 shape | func_0021E03C, func_002671B4, func_002672C8, func_00267648 +4 |
| PickupEntry | 8 | 5 | 0x6 callback, 0xC argument | func_0023ECF8, func_0023F120, func_0023F21C, func_0023F300 +4 |
| Self | 8 | 7 | 0x1C pitch | func_0025F3A4, func_0026115C, func_00261490, func_002616B8 +4 |
| V3f | 8 | 1 | `f32 x, y, z;` | func_0020EF60, func_00238824, func_00239160, func_00241894 +4 |
| B01Actor | 7 | 6 | `u8 pad0[0x4]; Vec3f pos; u8 pad10[0xC4]; u32 flags;` | func_002628B8, func_0026342C, func_00263640, func_002653F8 +3 |
| BossStep | 7 | 2 | 0x0 type, 0x4 arg, 0x8 animSpeed, 0xC chance, 0x10 delay | func_00404C44, func_004056DC, func_00405E04, func_00406000 +3 |
| Emitter | 7 | 7 | 0x8 *info, 0x24 unk24, 0x3C flags, 0x40 *nodes, 0x4C totalLength | func_0023417C, func_00236C64, func_0026E760, func_00282340 +3 |
| Entity | 7 | 5 | `u8 pad0[0x4]; s16 unk4s; u8 pad6[0x2]; Vec3f pos; s32 unk14; ActorDef ...` | func_002233A8, func_00223F8C, func_00224ED0, func_00224FC0 +3 |
| Hit | 7 | 7 | 0x0 fraction, 0x4 type, 0x8 vel, 0x14 slideTo, 0x20 pos, 0x2C *posOut, 0x30 *unk30, 0x34 *unk34, ... | func_00239F00, func_002478C8, func_00247C10, func_00247FB0 +3 |
| IntPair | 7 | 2 | `s32 a, b;` | func_0022BA88, func_0022BD10, func_0022BFA0, func_0022C228 +3 |
| Intel | 7 | 1 | 0x0 type, 0x14 sub | func_0025FDC8, func_002627EC, func_00262B58, func_00264198 +3 |
| IntelSub | 7 | 1 | 0x0 flags, 0x8 unk8, 0x18 unk18, 0x34 unk34, 0x38 unk38, 0x3C unk3C, 0x40 unk40, 0x58 unk58 | func_0025FDC8, func_002627EC, func_00262B58, func_00264198 +3 |
| MenuCtl | 7 | 2 | 0x4 slot | func_00417F9C, func_0041DA28, func_0041DC5C, func_0041E1D8 +3 |
| MenuPlayer | 7 | 2 | 0x518 *options | func_0024AF68, func_00450F4C, func_00455298, func_00455324 +3 |
| NameEntry | 7 | 2 | 0x0 timer, 0x4 length, 0x8 name[12] | func_0041D414, func_0041D6D4, func_0041D76C, func_00455FA0 +3 |
| Pick | 7 | 1 | `s16 id; s16 weight;` | func_00262D74, func_0040A170, func_0040A418, func_0040A6F0 +3 |
| PlayerEvent | 7 | 2 | `u8 pad0[0x1C]; void *player;` | func_004558C0, func_00455A38, func_00455AAC, func_00455B20 +3 |
| PlayerRaw | 7 | 1 | 0x18 velX, 0x1C velY, 0x20 velZ, 0xD4 actorFlags, 0xE2 unkE2, 0xA64 buttons, 0xA68 buttonsSingle, 0xA78 unkA78, ... | func_0025556C, func_0025557C, func_00255B20, func_00255EB0 +3 |
| Quat4 | 7 | 1 | `f32 x, y, z, w;` | func_002318D4, func_00231A00, func_00231B2C, func_00231C58 +3 |
| Rect | 7 | 2 | 0x0 minX, 0x4 minZ, 0x8 maxX, 0xC maxZ | func_002216DC, func_002228E4, func_0023A968, func_0023D050 +3 |
| Shared_Game | 7 | 5 | `char pad0[0x2C]; u8 options[0x10];` | func_004193B4, func_004197FC, func_0041CA00, func_00452314 +3 |
| Shared_PakMenu | 7 | 1 | `s16 state; char pad02[0xA]; struct PakNotesMenuItem *items; char pad10...` | func_004216C0, func_00423478, func_004570F8, func_0045A5E0 +3 |
| SoundEffectInfo | 7 | 1 | 0x0 group, 0x2 sample, 0x4 priority, 0x5 chance, 0x6 flags, 0x8 unk8, 0xA unkA, 0xC unkC, ... | func_0026E44C, func_00276850, func_00276874, func_00276898 +3 |
| Table | 7 | 6 | 0x0 count, 0x4 entries[1] | func_002018F0, func_00201910, func_00209AF4, func_00220D00 +3 |
| TimerState | 7 | 3 | 0x0 active, 0x4 unk4, 0x8 unk8, 0xC fastForward, 0x10 done, 0x14 a[4], 0x94 b[4], 0x114 c[4], ... | func_0042F1C4, func_0042F344, func_0042F514, func_0042F868 +3 |
| View | 7 | 6 | 0xC scaleX, 0x10 scaleY, 0x94 a, 0xE8 unkE8, 0xEC unkEC, 0x12C orientation[3], 0x138 unk138, 0x14C matA[0x40], ... | func_0021D39C, func_00249898, func_0024A678, func_0024C47C +3 |
| Voice | 7 | 5 | 0x0 sampleIndex, 0x4 handle, 0x10 id, 0x50 timer, 0xA4 groupMask, 0xA8 key, 0xAC stopping, 0xB0 *owner, ... | func_00225354, func_0024C47C, func_00271BA8, func_00274144 +3 |
| ALFilter | 6 | 3 | `struct ALFilter_s *source; void *(*handler)(struct ALFilter_s *filter,...` | func_002925E0, func_002946D0, func_00295690, func_00296360 +2 |
| Acmd | 6 | 4 | `u32 w0; u32 w1;` | func_002946D0, func_00295304, func_00295690, func_00296360 +2 |
| Anim | 6 | 3 | 0x8 *ai) | func_00259A4C, func_00259A98, func_0025DBD8, func_0040BC64 +2 |
| B01State | 6 | 6 | `u8 pad0[0xF8]; void (*fnF8)();` | func_002628B8, func_0026342C, func_00263640, func_002653F8 +2 |
| Bounds | 6 | 3 | 0x0 max | func_00221A24, func_002228E4, func_00227240, func_002274A4 +2 |
| Collider | 6 | 4 | 0x4 *group | func_00241960, func_00241CB0, func_00241E30, func_00241FC8 +2 |
| CollisionResult | 6 | 6 | 0x0 *current, 0x8 origin, 0x88 unk88, 0x90 unk90, 0x9C unk9C, 0xA0 unkA0, 0xB0 unkB0, 0xB4 unkB4, ... | func_0022AFB4, func_0022C488, func_0022D2FC, func_0024EAF8 +2 |
| DrawParams | 6 | 3 | 0x0 shadow, 0x14 set0, 0x18 set1 | func_0021D7BC, func_0021D928, func_0021DA8C, func_0025BA80 +2 |
| HitTarget | 6 | 5 | 0x0 active, 0x4 unk4, 0x8 a, 0x14 b | func_0023D4D4, func_0023DF70, func_0023F880, func_00240800 +2 |
| MenuBox | 6 | 3 | 0x0 selected, 0x14 x, 0x1C y | func_00413230, func_0042085C, func_00424780, func_0044C230 +2 |
| ObjectHead | 6 | 3 | 0x0 header, 0x18 m_vVelocity, 0x24 m_vWaterDisplacement, 0x30 *m_pInstanceBelow | func_0022D2FC, func_00233000, func_00245E48, func_00245FC8 +2 |
| ParticleDesc | 6 | 3 | 0x0 flags, 0x18 *events | func_0022BA88, func_0022BD10, func_0022BFA0, func_0022C228 +2 |
| ParticleEvents | 6 | 2 | 0x0 parameters[14], 0x70 particles[14], 0x8C effects[14], 0xA8 sounds[14] | func_0022BA88, func_0022BD10, func_0022BFA0, func_0022C228 +2 |
| S | 6 | 4 | `s32 a; V3 v; s32 d;` | func_00206920, func_0021810C, func_00218218, func_002182BC +2 |
| Selection | 6 | 1 | 0x4 index, 0xB0 flags | func_00419560, func_004217A0, func_00421880, func_00451154 +2 |
| SelectionOwner | 6 | 1 | 0x1C unk1C, 0x20 *sel | func_00419560, func_004217A0, func_00421880, func_00451154 +2 |
| Sound | 6 | 6 | 0x168 volume | func_00258EA0, func_0025C5F8, func_00272D3C, func_00272EC4 +2 |
| Surface | 6 | 3 | 0x14 flags | func_0021B95C, func_0024C47C, func_00250430, func_002505F8 +2 |
| TableOwner | 6 | 6 | 0x5C unk5C | func_00223F50, func_00224F84, func_00225D94, func_00226160 +2 |
| __OSInode | 6 | 1 | `__OSInodeUnit inode_page[128];` | func_00427780, func_00427E60, func_00428CF0, func_00460D80 +2 |
| __OSInodeUnit | 6 | 2 | `struct { u8 bank; u8 page; } inode_t; u16 ushort;` | func_00427780, func_00427E60, func_00428CF0, func_00460D80 +2 |
| __OSPackId | 6 | 1 | `u32 repaired; u32 random; u64 serial_mid; u64 serial_low; u16 deviceid...` | func_00429460, func_00429748, func_00462380, osPfsChecker +2 |
| AIModeVars | 5 | 1 | 0x24 mode[3], 0x27 modeIndex | func_0025E43C, func_0025E44C, func_0025E460, func_0025F708 +1 |
| ActorStats | 5 | 1 | `u8 pad0[0x80]; s8 unk80; u8 pad81[0xD]; u8 unk8E; u8 unk8F; u8 pad90[2...` | func_00223F8C, func_00224ED0, func_00224FC0, func_00226024 +1 |
| AmmoDef | 5 | 1 | 0x0 type, 0x2 max | func_0024C47C, func_00253AC0, func_00253B54, func_00253C14 +1 |
| AnimRequest | 5 | 2 | 0x0 hasEvent, 0x18 current | func_00266570, func_00266AB4, func_00403660, func_00407DC0 +1 |
| AnimSlot | 5 | 1 | `u8 data[0x18];` | func_00266570, func_00266AB4, func_00403660, func_00407DC0 +1 |
| B19Obj | 5 | 5 | `char pad0[4]; B19Vec3 pos; char pad10[0xE8]; B19Parent *parent; char p...` | func_002323E0, func_002324E8, func_0023270C, func_00232EEC +1 |
| BitReader | 5 | 4 | 0x0 value, 0x4 *bits, 0x8 pos | func_0042EC30, func_0042ECF0, func_0042F6C8, func_00467B50 +1 |
| Brain | 5 | 1 | `u8 pad0[0xC]; s32 unkC; u8 pad10[0x210]; s32 unk220; u8 pad224[0x18]; ...` | func_00223F8C, func_00224ED0, func_00224FC0, func_00226024 +1 |
| Channel | 5 | 5 | 0x0 index, 0x8 unk8, 0xC unkC, 0x14 unk14, 0x2C unk2C, 0x34 unk34, 0x38 unk38, 0x3A unk3A, ... | func_00273A34, func_00273E78, func_00273E98, func_00274850 +1 |
| Cinema | 5 | 3 | 0x14 *script | func_00251FC4, func_00253FFC, func_00284188, func_0040F240 +1 |
| CollisionInfo | 5 | 3 | 0x0 flags | func_00268C48, func_002694A8, func_0026961C, func_00269780 +1 |
| EmitActor | 5 | 3 | 0x40 flags, 0xF8 *cfg | func_002316A0, func_00231778, func_00232EAC, func_0023417C +1 |
| Event | 5 | 3 | 0x4 actor, 0xA level | func_002637C4, func_00265E98, func_0026DFB0, func_0026E42C +1 |
| GameMode | 5 | 1 | `u8 multiplayer;` | func_0024C47C, func_00250BF0, func_00250EC0, func_0042DA60 +1 |
| Instance | 5 | 4 | 0x14 *def | func_00221A24, func_00267D94, func_002680A8, func_00268A5C +1 |
| Item | 5 | 5 | `u8 pad0[0xA0]; Bounds bounds; u8 padB8[0xC0 - 0xB8];` | func_002228E4, func_00420F34, func_00443D28, func_0045A7A0 +1 |
| List | 5 | 4 | `void *head; void *tail; s32 count; s32 a; s32 b;` | func_00221A24, func_00290F20, func_0042FBA4, func_00466040 +1 |
| M04Vec3 | 5 | 1 | `f32 x, y, z;` | func_00236EA0, func_002706A0, func_00271E20, func_00272278 +1 |
| Material | 5 | 4 | 0x0 flags, 0x4 unk4, 0x8 texture, 0xC unkC, 0x10 prim[4], 0x14 env[4], 0x18 maskS, 0x1A maskT | func_00207FEC, func_0020A568, func_0020A994, func_00269050 +1 |
| MenuArgs | 5 | 1 | `u8 pad0[4]; s8 mode;` | func_00456948, func_00456B7C, func_004595D8, func_0045C398 +1 |
| Msg | 5 | 3 | `s16 kind; s16 pad; s32 a; s32 b; s32 c;` | func_00290DE0, func_00290E10, func_00290E90, func_00290EC0 +1 |
| O01Actor | 5 | 4 | `u8 pad0[0x518]; O01SaveSrc *save;` | func_0041DF50, func_004206B8, func_00421454, func_00456E70 +1 |
| O01Save | 5 | 4 | `u16 w[4]; u8 b0; u8 b1; u8 b2; u8 b3;` | func_0041DF50, func_004206B8, func_00421454, func_00456E70 +1 |
| O06C | 5 | 3 | `u8 pad[0x2B]; s8 f;` | func_0041BF08, func_0041BF6C, func_00454ECC, func_00454F94 +1 |
| Pager | 5 | 3 | 0x24 completion, 0xAD8 pending | func_0028D820, func_0028E068, func_0028E7A4, func_0028E8A4 +1 |
| Placed | 5 | 4 | 0x0 pos, 0xC angle, 0x10 kind, 0x12 id | func_0026872C, func_00268838, func_0042BA40, func_0042C194 +1 |
| Plane | 5 | 3 | `u8 pad0[0x30]; Vec3 normal;` | func_0023C2B0, func_0023C390, func_0023C478, func_0026F080 +1 |
| Projectile | 5 | 5 | 0x18 vel, 0xF8 *def, 0x11C speed, 0x150 dir, 0x178 phaseX, 0x17C phaseY, 0x180 freqX, 0x184 freqY, ... | func_00229618, func_0022A1A4, func_0022A4C8, func_0025CAC8 +1 |
| Rec14 | 5 | 1 | `u8 pad0[0xE]; u8 flags; u8 key; u8 pad10[0x4];` | func_00223F8C, func_00224ED0, func_00224FC0, func_00226024 +1 |
| SndCtx | 5 | 5 | 0x2B98 listener | func_002718D8, func_00271D54, func_002726DC, func_00273DD8 +1 |
| SoundBank | 5 | 4 | 0x2BAC unk2BAC | func_00271790, func_002718BC, func_002718C4, func_00272EC4 +1 |
| SoundDef | 5 | 2 | 0x0 unk0, 0x2 unk2, 0x4 unk4, 0x6 flags6, 0x8 unk8, 0xA unkA, 0x10 unk10 | func_00225354, func_0027682C, func_0027685C, func_00276868 +1 |
| SweepInst | 5 | 4 | 0x0 type, 0x4 x, 0x8 y, 0xC z, 0x144 health | func_0023E4A0, func_0023E6D8, func_00240800, func_00240E90 +1 |
| Unk800C1BB0 | 5 | 1 | 0x0 unk0, 0x4 unk4, 0x8 unk8, 0xC unkC, 0x10 unk10, 0x14 unk14, 0x18 unk18, 0x1C unk1C, ... | func_002837E0, func_00284144, func_00284240, func_00284284 +1 |
| ALEvent | 4 | 4 | `s16 type; union { ALMIDIEvent midi; } msg;` | func_0028F1F0, func_0028FB80, func_002900B0, func_00291330 |
| ALEventQueue | 4 | 4 | 0x0 lists[0x14] | func_0028FB80, func_002900B0, func_00291330, func_002925E0 |
| ALPlayer | 4 | 3 | `struct ALPlayer_s *next; void *clientData; void *handler; s32 callTime...` | func_00291330, func_002925E0, func_002946D0, func_00295690 |
| ALVoice | 4 | 3 | `ALLink node; ALPVoice *pvoice; void *table; void *clientPrivate; s16 s...` | func_0028FB80, func_002925E0, func_002946D0, func_00295690 |
| ActionState | 4 | 3 | 0x2C *table, 0xC6 anim, 0xC7 active, 0xF0 *), 0xF8 *) | func_00265200, func_00265954, func_00265A30, func_00266380 |
| ActorPool | 4 | 1 | 0x0 items[32], 0x3600 *free, 0x3604 *head, 0x3608 *tail | func_00227240, func_002274A4, func_002279A4, func_00465EB0 |
| Arg1 | 4 | 4 | 0x1C unk1C | func_00255840, func_00255C5C, func_002593A4, func_0045C050 |
| B01IoReq | 4 | 1 | `s32 w0; s32 w4; s32 w8; union { s32 i; struct { u16 hi; u16 cmd; } h; ...` | func_002A2858, func_002A2B08, func_002A2BE8, func_002A2D64 |
| Bank | 4 | 4 | 0x4 f4, 0xC *tab | func_00270240, func_00274850, func_00292844, func_0045F898 |
| BossFight | 4 | 3 | 0x0 *obj, 0x4 *def, 0x8 state, 0xC timer, 0x10 count, 0x14 warned | func_00407F20, func_0040813C, func_004082A0, func_004085A0 |
| CPlayerList | 4 | 1 | 0xC m_IdleList, 0x20 m_ActiveList | func_00253148, func_002531D0, func_00254B6C, func_00254BE4 |
| CRenderInfo | 4 | 1 | 0x0 flags, 0x14 unk14 | func_0023181C, func_002328A8, func_00232D68, func_00236170 |
| CRenderInst | 4 | 1 | 0x0 type, 0x2 objType, 0x4 pos, 0x40 flags, 0x48 mtx[2][0x40], 0xC8 bounds[2][0x18], 0xF8 *info, 0x108 *link, ... | func_0023181C, func_002328A8, func_00232D68, func_00236170 |
| CollisionDef | 4 | 4 | 0x0 flags, 0x4 unk4, 0x5 unk5, 0x8 gravity, 0x10 friction, 0x14 drag, 0x18 waterDrag | func_00239F00, func_0023AB18, func_0023B664, func_0023D050 |
| ControllerButtonHistory | 4 | 1 | `s8 taps; s8 age; s8 repeat; s8 held;` | func_00450C0C, func_00450CA0, func_00453A08, func_00454CC4 |
| Ctl | 4 | 2 | 0x0 present, 0x4 port, 0x8 f8, 0xC fC, 0x10 f10, 0x14 f14, 0x18 f18, 0x1C f1C, ... | func_00430070, func_004301C8, func_004531B4, func_00468F90 |
| CueContext | 4 | 3 | 0x38 active | func_00283BC0, func_00283CD4, func_00284254, func_00284640 |
| DIunion | 4 | 1 | `struct DIstruct s; DItype ll;` | func_002A3AE0, func_002A40D0, func_002A4650, func_002A4BE0 |
| Dest | 4 | 4 | 0x4 maxHealth, 0x8 handle, 0xC unk10, 0x10 level | func_002427E0, func_00242C00, func_00242E34, func_00246918 |
| Ent12 | 4 | 1 | `s32 **tblp; s32 b; s32 c;` | func_00223F8C, func_00224ED0, func_00224FC0, func_00226024 |
| EventParams | 4 | 4 | 0x0 unk4 | func_0023801C, func_00238A7C, func_00238DF8, func_00239194 |
| Game80119870 | 4 | 2 | 0x1418 m_Mode | func_00226490, func_0027C1E8, func_0027D160, func_0027E770 |
| Handle | 4 | 3 | `void *data;` | func_0020BFB0, func_0026C590, func_0042BA40, func_00464960 |
| HitDef | 4 | 3 | 0x0 kind | func_002376F8, func_0023DF70, func_0023F880, func_0023FDF0 |
| HitInst | 4 | 4 | 0x0 type, 0x4 pos, 0x14 *def, 0x1C fallSpeed, 0x30 *standingOn | func_0023DF70, func_0023F880, func_0023FDF0, func_0024032C |
| Intelligence | 4 | 3 | 0x14 flags | func_002541E4, func_00255298, func_00255340, func_0025748C |
| IoRequest | 4 | 2 | 0x0 *buffer, 0x4 size, 0x8 write, 0xC flags, 0x10 arg0, 0x14 arg1, 0x18 arg2, 0x1C *reply | func_002A2650, func_002A2804, func_002A28D8, func_002A29F0 |
| ItemDef | 4 | 1 | 0x20 type, 0x22 index | func_00424780, func_00424978, func_0045D6A0, func_0045D898 |
| Link | 4 | 3 | 0x0 *next, 0x4 *prev | func_00268A2C, func_0028E9E0, func_0028EA10, func_002933B0 |
| Location | 4 | 3 | 0x0 pos, 0xC region | func_002438C0, func_00244338, func_002449E0, func_0025D9E4 |
| MenuState | 4 | 4 | 0x18E50 *lights, 0x18E54 dir[3] | func_0042AA80, func_0042C3BC, func_0042C46C, func_0042C758 |
| Model | 4 | 3 | 0x0 index, 0xB0 header | func_0021ED04, func_0024C47C, func_00274798, func_002747EC |
| ModelSweep | 4 | 4 | 0x0 *sweep, 0x4 view[4][4], 0x44 scale, 0x48 hitDist, 0x4C *model, 0x50 kind, 0x54 mtx[4][4], 0x94 inverse[4][4], ... | func_0023D050, func_0023D4D4, func_0023D7CC, func_0023DBBC |
| Mtx | 4 | 2 | `long m[4][4];` | func_0022D2FC, func_002975D0, func_00297D50, func_00297FB0 |
| O01SaveSrc | 4 | 2 | `u16 w[4]; u8 pad8[0x28]; u8 b0; u8 pad31[2]; u8 tail[8];` | func_004206B8, func_00421454, func_00456E70, func_0045A374 |
| OwnedState | 4 | 3 | 0x10C *owner | func_00264B0C, func_00264DF0, func_00264F58, func_0026508C |
| OwnerActor | 4 | 4 | 0x4 pos[3], 0x98 *jointMatrices | func_00264B0C, func_00264DF0, func_00264F58, func_0026508C |
| PadState | 4 | 3 | 0x0 channel, 0xC8 fieldCC, 0xCC fieldD0, 0xD0 fieldD4 | func_0027BB80, func_00284B94, func_00285304, func_00285410 |
| PageChain | 4 | 2 | `PageMapping *head; PageMapping *tail;` | func_0028D820, func_0028E7A4, func_0028E8A4, func_0028E98C |
| PageJob | 4 | 1 | 0x0 type, 0x4 *source, 0x8 size, 0xC page, 0x10 *mapping, 0x14 *thread, 0x18 *slot | func_0028E068, func_0028E7A4, func_0028E8A4, func_0028E98C |
| PageMapping | 4 | 2 | 0x0 *next, 0x4 *prev, 0x8 id, 0xA index, 0xB slot | func_0028E068, func_0028E7A4, func_0028E8A4, func_0028E98C |
| ParticleDef | 4 | 3 | 0x0 flags, 0x18 *events | func_00229340, func_0022AFB4, func_0022C488, func_0022CA90 |
| PlacedProp | 4 | 4 | 0x0 state, 0x2 id, 0x4 pos, 0x10 *segment, 0x14 *model, 0x18 owner, 0x1C unk1C, 0x28 matrix[16], ... | func_002141F0, func_00214564, func_00214844, func_00214B58 |
| Plan | 4 | 4 | 0x4 active, 0x30 *info, 0x68 *ignore, 0x80 *approachFrom, 0x84 unk84, 0x8C rotY, 0x90 height, 0x94 location, ... | func_002438C0, func_00243C90, func_00244338, func_002449E0 |
| PlayerMenu | 4 | 2 | 0x1C *player | func_00418408, func_00450F00, func_00454F30, func_00455420 |
| PlayerState | 4 | 3 | `s32 data[0x4E8 / 4];` | func_002613D8, func_00261B78, func_0028D100, func_0028D264 |
| Pool | 4 | 4 | 0x3600 *head, 0x3604 *tail | func_00227AE0, func_00293530, func_00466040, func_00468E78 |
| Request | 4 | 3 | 0x8 count, 0xC flags | func_00201BDC, func_00204D9C, func_00205110, func_00296340 |
| Resource | 4 | 4 | 0x0 resource, 0xC flags | func_002062F8, func_00216A9C, func_00225E88, func_0027B40C |
| Screen | 4 | 3 | 0xC *layer | func_00280EE8, func_004531B4, func_00453380, func_00453430 |
| Setting | 4 | 3 | 0x0 unk0, 0x4 *config, 0x8 value, 0xC enabled, 0x10 index, 0x14 (*handler)(), 0x18 unk18, 0x1C unk1C | func_00281B78, func_004181C8, func_0042EF2C, func_00467E4C |
| SndLock | 4 | 2 | 0x0 q[0x18], 0x1C count | func_00270240, func_00270C80, func_00271514, func_002717C0 |
| SndVoice | 4 | 4 | 0xC8 sound | func_00270C80, func_002726DC, func_00273DD8, func_002746AC |
| SoundOwner | 4 | 2 | 0x104 currentId | func_00274144, func_0027421C, func_002742F4, func_00275E1C |
| Spawn | 4 | 4 | 0x10 kind | func_002237C4, func_00254044, func_0040E30C, func_0044700C |
| SpawnParams | 4 | 2 | 0x0 arg, 0x4 pos | func_002318D4, func_00231A00, func_00231B2C, func_00231C58 |
| Src | 4 | 3 | `u8 pad0[0x13]; u8 unk13;` | func_002248D4, func_00224A68, func_00224B58, func_0023C120 |
| Strokes | 4 | 2 | 0x0 count, 0x4 a, 0x8 b, 0x37C last | func_0024C47C, func_002508D0, func_00250BF0, func_00251FC4 |
| SweepBox | 4 | 2 | 0x0 radius, 0x4 bottom, 0x8 top, 0xC startTop, 0x10 startBottom, 0x14 endTop, 0x18 endBottom, 0x1C minX, ... | func_0023DF70, func_0023E4A0, func_0023E6D8, func_0024032C |
| TargetInfo | 4 | 3 | 0x8 unk8 | func_0025DF04, func_0025F068, func_002603F0, func_00408C10 |
| Triple | 4 | 3 | 0x0 value, 0x4 unk4, 0x8 unk8 | func_0026D230, func_0026EE30, func_0028D0B8, func_00401B30 |
| VertexGroup | 4 | 2 | 0x0 unk0, 0x4 count, 0x8 verts[1] | func_00212DA4, func_00212F98, func_002130A8, func_00214F68 |
| ViewState | 4 | 3 | `s32 mask; u8 pad4[0xD]; u8 unk11;` | func_00221210, func_00221D3C, func_002222E0, func_0024C47C |
| Viewport | 4 | 2 | 0x0 scaleX, 0x4 scaleY, 0x8 offsetX, 0xC offsetY | func_00249898, func_0024A678, func_0042F9B0, func_004688D0 |
| __OSDir | 4 | 1 | `u32 game_code; u16 company_code; __OSInodeUnit start_page; u8 status; ...` | func_00427780, func_00428CF0, func_00461C10, osPfsChecker |
| ALCSPlayer | 3 | 3 | 0x0 node[0x14], 0x14 *drvr, 0x18 *target, 0x1C curTime, 0x20 *bank, 0x24 uspt, 0x28 nextDelta, 0x2C state, ... | func_0028FB80, func_002900B0, func_00291330 |
| ALLink | 3 | 1 | `struct ALLink_s *next; struct ALLink_s *prev;` | func_002925E0, func_002946D0, func_00295690 |
| ALMainBus | 3 | 2 | `ALFilter filter; s32 sourceCount; s32 maxSources; ALFilter **sources;` | func_002946D0, func_002953B4, func_00296360 |
| ALVoiceState | 3 | 3 | 0x0 *next, 0x4 voice, 0x20 *sound, 0x24 envEndTime, 0x28 pitch, 0x2C vibrato, 0x30 envGain, 0x31 channel, ... | func_0028FB80, func_00291330, func_002925E0 |
| ActorHead | 3 | 2 | 0x4 pos | func_00243C90, func_0040F240, func_00448240 |
| AdecStream | 3 | 3 | 0x0 hdr0, 0x4 hdr2, 0x8 *hdr, 0xC nframes, 0x10 remaining, 0x14 *rd | func_0029FE70, func_002A0448, func_002A0B6C |
| AmmoInfo | 3 | 3 | 0x0 index, 0x2 max | func_002484F8, func_00248644, func_00248720 |
| AnimChannel | 3 | 3 | 0x0 time, 0x4 current, 0x6 next, 0x8 frames, 0xA active, 0xB blending, 0xC loopStart, 0x10 **resource | func_00218B10, func_0021CDAC, func_0021F428 |
| AnimInfo | 3 | 3 | 0x1C intelligence, 0x1E angle | func_00227C10, func_0022D2FC, func_00234348 |
| Attachment | 3 | 3 | 0x0 field0, 0x2 type, 0x4 field4, 0x8 offset, 0x14 params | func_00214B58, func_00267090, func_0026D710 |
| B01Def | 3 | 2 | `u8 pad0[0x14]; u32 flags14; u8 pad18[0x4]; s32 unk1C; u8 pad20[0x4]; s...` | func_002628B8, func_0026342C, func_00266284 |
| B01IF | 3 | 1 | `s32 i; f32 f;` | func_002628B8, func_0026342C, func_00263640 |
| B01Info | 3 | 2 | `u32 flags; s32 unk4; s16 unk8; s16 unkA; B01W4 unkC; B01FH unk10; s16 ...` | func_002628B8, func_0026342C, func_00263640 |
| B01Pair | 3 | 1 | `s32 a; s32 b;` | func_002653F8, func_002655F8, func_002656BC |
| B15Def | 3 | 1 | `s32 type; u8 pad4[0xA]; s8 unkE;` | func_0023726C, func_00239058, func_00239254 |
| B19Parent | 3 | 3 | `char pad0[0x18]; B19Tbl *tbl;` | func_002323E0, func_002324E8, func_0023270C |
| B19Vec3 | 3 | 1 | `f32 x, y, z;` | func_002323E0, func_002324E8, func_00233190 |
| B21Game | 3 | 3 | `u8 pad0[0x23FE4]; f32 unk23FE4;` | func_00288B1C, func_0028932C, func_00289F30 |
| Barrel | 3 | 2 | 0x0 unk0, 0x4 unk4, 0x8 unk8, 0x37C unk37C | func_0025247C, func_002551A0, func_00256450 |
| BigState | 3 | 3 | 0x23FF4 paused | func_00288FDC, func_00289700, func_004692CC |
| Block | 3 | 2 | 0x4 entries[16] | func_00220D00, func_0045F62C, func_0045F8EC |
| Bone | 3 | 2 | 0x0 *anim, 0x90 type | func_0022C488, func_00259A4C, func_00259A98 |
| BootState | 3 | 1 | 0x23FDC state, 0x23FE0 unk23FE0, 0x23FE1 unk23FE1, 0x23FE4 unk23FE4, 0x23FE8 ready, 0x23FF8 unk23FF8, 0x23FFC unk23FFC | func_00288284, func_0028891C, func_004303AC |
| BossAI | 3 | 2 | 0xC4 animId, 0xC6 anim, 0xC7 unkC7 | func_00407F20, func_0040813C, func_004088B0 |
| BossMode | 3 | 2 | 0x0 mode, 0x1C cooldown, 0x8012F9F8 *script, 0x8012F9FC scriptPrev, 0x8012FA00 scriptIndex | func_00404C44, func_00405C68, func_0043DC44 |
| BossScript | 3 | 3 | 0x0 cooldown, 0x4 *script, 0x8 cursor, 0xC index | func_00406E0C, func_00407BBC, func_004400B4 |
| Box3 | 3 | 1 | `Vec3f mn; Vec3f mx;` | func_002132B8, func_00213A70, func_00213FA0 |
| CAIModeInfo | 3 | 3 | 0x8 *) | func_0026352C, func_00264D98, func_0044459C |
| CIntelligence | 3 | 3 | 0x0 type | func_00219BC0, func_002682A4, func_002682B8 |
| CRequest | 3 | 1 | 0x0 key, 0x8 count, 0xC flags, 0x24 *next | func_00204D90, func_002057B0, func_00206498 |
| CachedResource | 3 | 2 | 0x0 size, 0x4 count, 0x8 flags, 0xC stamp | func_0020367C, func_002044A8, func_002051F4 |
| Cameras | 3 | 2 | 0x0 **handle, 0x4 *cameras, 0x8 count, 0xC active, 0x20 free, 0x34 word34, 0x38 word38, 0x3C word3C, ... | func_0042FBA4, func_0042FEEC, func_00468AC4 |
| Context | 3 | 3 | 0x23FDC unk23FDC, 0x23FE1 unk23FE1, 0x23FF8 unk23FF8, 0x23FFC unk23FFC | func_002886D0, func_00288D3C, func_00469250 |
| CounterOwner | 3 | 1 | 0x52C counters[1] | func_00256A8C, func_00256AAC, func_00256AF0 |
| Counters | 3 | 3 | 0x0 pending, 0x4 count | func_00204DE0, func_0040E30C, func_0044700C |
| CtlEnt | 3 | 1 | `u8 a; u8 b; u8 c; u8 d;` | func_00430070, func_004301C8, func_00468F90 |
| Cursor | 3 | 3 | 0x0 ptr, 0x4 stride | func_00218490, func_0027ACC4, func_0041A904 |
| Effect | 3 | 2 | 0x0 obj, 0x288 links[8], 0x290 *ownerCount | func_002280B0, func_00228484, func_0022D2FC |
| EmitCfg | 3 | 1 | 0x14 unk14 | func_00232EAC, func_0023417C, func_00236C64 |
| EmitterOwner | 3 | 3 | 0x8A0 freeList, 0x8B4 activeList, 0x8C8 mask | func_00282E4C, func_00283238, func_002836E4 |
| Enemy | 3 | 1 | 0x144 health, 0x28C *next | func_00401E30, func_004021C8, func_00402404 |
| Floor | 3 | 3 | `u8 data[0x20];` | func_00220A1C, func_0024EF70, func_0025005C |
| GameSession | 3 | 3 | 0x23FD0 previousMode, 0x23FD4 mode, 0x23FD8 target | func_00288E58, func_00430330, func_00430454 |
| HudItem | 3 | 2 | `u8 pad[0x3C];` | func_0024C47C, func_0042DE74, func_00466D94 |
| HudMeter | 3 | 1 | 0x3C unk3C, 0x40 active, 0x44 percent | func_0027AD24, func_0027AD90, func_0027ADA0 |
| HudMeters | 3 | 1 | 0x0 meters[2] | func_0027AD24, func_0027AD90, func_0027ADA0 |
| HudModel | 3 | 3 | 0x0 current, 0x4 requested, 0x8 *model, 0x90 scaleX, 0x94 scaleY, 0x98 scaleZ, 0x9C pitch, 0xA0 roll, ... | func_00247170, func_00247438, func_00247660 |
| IF32 | 3 | 1 | `s32 i; f32 f;` | func_002706A0, func_00270A60, func_00271128 |
| Info | 3 | 2 | 0x0 flags | func_00221D3C, func_002222E0, func_00441BFC |
| ItemList | 3 | 3 | 0x4 items[1] | func_002228E4, func_00429F20, func_0042CD04 |
| Job | 3 | 2 | 0x0 result, 0xC unkC, 0x1C (*func)() | func_0028A43C, func_0042F670, func_00468570 |
| KeyLookup | 3 | 2 | 0x0 *posKeys, 0x4 *rotKeys, 0x8 *posTracks, 0xC *rotTracks, 0x10 offset0, 0x14 offset1, 0x18 rootOffset0, 0x1C rootOffset1, ... | func_00218B10, func_0021B95C, func_0021CDAC |
| Layer | 3 | 3 | 0x80 flagsA, 0xA8 flagsB | func_004531B4, func_00453380, func_00453430 |
| LevelLights | 3 | 2 | 0x10 flags | func_0042AA80, func_0042C758, func_00465678 |
| Light | 3 | 3 | `u8 pad0[0x8]; u8 colour[2]; u8 padA[0x6]; s8 dir[3]; u8 pad13[0x5];` | func_0021DC84, func_00236324, func_0040EDB0 |
| M04IF | 3 | 1 | `s32 i; f32 f;` | func_002706A0, func_00272278, func_00272B54 |
| M04VP | 3 | 1 | `s32 w0; s32 w4; s32 vol; s32 wC; s16 s10; u8 pad12[2]; M04IF v14; s16 ...` | func_002706A0, func_00272278, func_00272B54 |
| MemHandle | 3 | 2 | `s32 addr;` | func_00425220, func_004567B0, func_0045E140 |
| MenuRow | 3 | 3 | 0x1C id, 0x24 available | func_0024BAD4, func_0041D050, func_0041DA28 |
| MessageBuffer | 3 | 1 | `s32 *allocation; s32 first; s32 count;` | func_004196B0, func_0041A44C, func_004533AC |
| ModeInfo | 3 | 2 | 0x8 (*onEvent)() | func_002627B8, func_00266814, func_0040B59C |
| ModeState | 3 | 2 | 0x0 unk0, 0x4 value, 0x8 mode, 0x40 unk40, 0x44 unk44 | func_0027AD00, func_0027AD88, func_0027AD98 |
| ModelInstance | 3 | 1 | 0x0 *matrix, 0x4 *lights, 0x8 *textures, 0xC segment | func_0020A568, func_0020B6A0, func_0020B7F8 |
| ModelVertex | 3 | 3 | 0x0 z, 0x6 bone | func_00212DA4, func_00212F98, func_002130A8 |
| Morph | 3 | 3 | 0x0 count, 0x4 *items, 0x8 *pointsA, 0xC *pointsB, 0x10 scaleA, 0x14 scaleB | func_0023353C, func_00233868, func_00234348 |
| MorphVtx | 3 | 2 | 0x0 ob[3], 0x6 flag, 0x8 tc[2], 0xC cn[4] | func_002097AC, func_0023353C, func_00233868 |
| MoveParams | 3 | 1 | 0x10 accel, 0x12 maxSpeed | func_002298C4, func_00229D10, func_00231EF8 |
| Mtx4 | 3 | 1 | `f32 m[4][4];` | func_00232168, func_0025FEB0, func_00260B40 |
| Mtx4f | 3 | 1 | `f32 m[4][4];` | func_00251A2C, func_0040F240, func_00448240 |
| Mtx64 | 3 | 2 | `f32 m[16];` | func_00236A34, func_00236D50, func_00247438 |
| OSPiHandle | 3 | 3 | `struct OSPiHandle_s *next; u8 type; u8 latency; u8 pageSize; u8 relDur...` | func_0029CAF0, func_0029D370, func_0029D690 |
| OSScMsg | 3 | 1 | `s16 type; char misc[0x1E];` | func_004196B0, func_0041A44C, func_004533AC |
| OSScTask_s | 3 | 1 | `struct OSScTask_s *next; u32 state; u32 flags; void *framebuffer; OSTa...` | func_004196B0, func_0041A44C, func_004533AC |
| OwnerPlayer | 3 | 1 | 0x996 weapon, 0xAAC unkAAC, 0xB2C unkB2C, 0xB38 *unkB38 | func_002438C0, func_00244338, func_002449E0 |
| PadInput | 3 | 1 | 0xB4 m_Pressed | func_00285558, func_002855A8, func_002855E8 |
| Pair | 3 | 3 | 0x0 a, 0x4 b | func_00207908, func_0020793C, func_002254C0 |
| Params | 3 | 3 | 0x40 biteRange, 0x4E kind | func_002616B8, func_00261A18, func_00263A20 |
| ParentDef | 3 | 1 | 0x30 *move | func_002298C4, func_00229D10, func_00231EF8 |
| ParticleSystem | 3 | 2 | 0x0 particles[128], 0xEC00 free, 0xEC14 active, 0xEC28 lists[3], 0xEC64 unkEC64 | func_00230CB4, func_0042D220, func_00466140 |
| PathNode | 3 | 3 | 0x0 *region, 0x4 *parent, 0x8 count, 0xC cost | func_002393D0, func_002399F8, func_0025D474 |
| PlayerBlob | 3 | 1 | `s32 data[0x4E8 / 4];` | func_0041D890, func_00421528, func_0045A448 |
| PlayerDef | 3 | 2 | 0xC kind, 0x18 health, 0x38 scale[3] | func_0023F3E4, func_0042E430, func_00467350 |
| PlayerSettings | 3 | 2 | `u8 pad0[0x28]; u8 enabled; u8 pad29[0x40 - 0x29];` | func_00417C0C, func_004197FC, func_0045275C |
| PortStatus | 3 | 1 | `u8 b[4];` | func_00430070, func_004301C8, func_00468F90 |
| RenderCounters | 3 | 3 | `f32 ticks, seconds, minutes, hours; f32 countdown; f32 timeLimit; s32 ...` | func_00417D78, func_0041CA00, func_00455960 |
| RenderSettings | 3 | 1 | `u8 reserved0[0x14]; CVector3 direction; s32 ready;` | func_004196B0, func_0041A44C, func_004533AC |
| S10Ctrl | 3 | 1 | `s32 active; s8 port; u8 pad5[0x224 - 5];` | func_00418158, func_00451108, func_0045121C |
| S10Ctx | 3 | 2 | `u8 pad0[0x518]; S10Sub *sub;` | func_00418158, func_00451108, func_0045121C |
| S10Flags | 3 | 1 | `u8 pad0[0x58]; u32 flags;` | func_00418158, func_00451108, func_0045121C |
| S10Menu | 3 | 1 | `s16 state; u8 pad2[0xA]; S10Flags *fC; u8 pad10[0xC]; S10Ctx *ctx; S10...` | func_00418158, func_00451108, func_0045121C |
| S10Sub | 3 | 2 | `u8 pad0[0x2B]; s8 b2B;` | func_00418158, func_00451108, func_0045121C |
| SaveBuffer | 3 | 2 | 0x0 size, 0x4 extra, 0x8 match[4], 0x18 player, 0x500 setup, 0x684 text[4] | func_0041D890, func_00421528, func_0045A448 |
| SavedPlayers | 3 | 3 | 0x0 active, 0x4 state, 0x8 timer, 0xC warp | func_0028CF80, func_0028D100, func_0028D264 |
| SceneEffect | 3 | 3 | 0x150 minZ, 0x154 maxX, 0x158 maxY, 0x15C maxZ | func_0022782C, func_00227C10, func_00227EE8 |
| ScreenRectangle | 3 | 2 | 0x288 x, 0x28C y, 0x290 width, 0x294 height | func_00254834, func_0040F240, func_00448240 |
| SetupBlob | 3 | 1 | `s32 data[0x184 / 4];` | func_0041D890, func_00421528, func_0045A448 |
| ShadowActor | 3 | 3 | 0x4 x, 0x8 y, 0xC z, 0x10 region, 0xB8 shadowMatrices[4][16] | func_002154DC, func_0021E03C, func_0022D2FC |
| Sortable | 3 | 3 | 0xC key | func_002571D8, func_00283784, func_002837B0 |
| SoundHandle | 3 | 1 | 0x8 state, 0xC sound | func_002298C4, func_00229D10, func_00232AC4 |
| Sphere | 3 | 2 | 0x0 center, 0xC radius, 0x10 radiusSq | func_00239DB0, func_0023CE94, func_0023CF04 |
| State800F56B0 | 3 | 2 | 0x0 _unk0, 0x4 m_bA, 0x8 m_bB, 0x1B4 m_Value | func_0027BB80, func_0027BC34, func_0027BC68 |
| Step | 3 | 1 | 0x0 active, 0x4 value, 0x14 slot, 0x18 onTime, 0x1C offTime | func_00401E30, func_004021C8, func_00402404 |
| Swoosh | 3 | 2 | 0x4 *next, 0x8 *point, 0x1C *owner, 0x24 top, 0x3C flags | func_0023624C, func_002366C8, func_002368B0 |
| SwooshOwner | 3 | 1 | 0x10F activeSwooshes, 0x1B5 activeSwooshes2 | func_0023624C, func_002366C8, func_002368B0 |
| Track | 3 | 2 | 0x0 stride, 0x4 count | func_0028F784, func_0040F240, func_00448240 |
| Trail | 3 | 3 | 0x8 *track, 0xC *first, 0x10 *second, 0x28 phase, 0x3C flags, 0x40 *tail, 0x44 *clock, 0x4C length | func_00234348, func_0023685C, func_00236BC4 |
| TriggerData | 3 | 1 | 0x4 unk4, 0xA unkA | func_0026376C, func_002657D8, func_00266A00 |
| Unk0022425C | 3 | 1 | 0x44 flags, 0x58 unk58, 0x5B unk5B, 0x5C unk5C | func_0026C0E0, func_0026C5C4, func_0026C724 |
| Vertex | 3 | 3 | 0x0 ob[3], 0x6 flag, 0x8 tc[2], 0xC cn[4] | func_00214F68, func_0026A798, func_0026CA3C |
| VideoMode | 3 | 2 | 0x0 width, 0x4 height | func_00423E10, func_004242EC, func_0045D20C |
| VoiceSet | 3 | 1 | 0x4 voices[17] | func_00274144, func_0027421C, func_002742F4 |
| WRec | 3 | 3 | `u8 pad0[0xA0]; Box3 box; u8 padB8[0x2]; u8 bBA; u8 padBB[0x2]; u8 bBD;...` | func_002132B8, func_00213A70, func_00213FA0 |
| WTab | 3 | 2 | `u8 pad0[0x4]; s32 count; WRec r[1];` | func_002132B8, func_00213A70, func_00213FA0 |
| AIModeParam | 2 | 1 | 0x6 value | func_002668D4, func_0026E40C |
| ALAuxBus | 2 | 2 | `ALFilter filter; s32 sourceCount; s32 maxSources; ALFilter **sources; ...` | func_002946D0, func_00295410 |
| ALBankFile | 2 | 1 | `s16 revision; s16 bankCount; ALBank_s *bankArray[1];` | func_002712E8, func_00273EC0 |
| ALMIDIEvent | 2 | 1 | `s32 ticks; u8 status; u8 byte1; u8 byte2; u32 duration;` | func_0028F1F0, func_002925E0 |
| ALPVoice | 2 | 1 | `ALLink node; struct ALVoice_s *vvoice; ALFilter *channelKnob; u8 decod...` | func_002925E0, func_00295690 |
| ALParam | 2 | 2 | `struct ALParam_s *next; s32 delta; s16 type; union { f32 f; s32 i; } d...` | func_002946D0, func_00295690 |
| ALSave | 2 | 1 | `ALFilter filter; s32 dramout; s32 first;` | func_002946D0, func_002972E0 |
| ALSynth | 2 | 2 | `ALPlayer *head; ALLink pFreeList; ALLink pAllocList; ALLink pLameList;...` | func_002946D0, func_00295690 |
| ActionInfo | 2 | 2 | 0x0 flags, 0x4 duration, 0x8 eventFrame | func_00265200, func_00266380 |
| ActionParams | 2 | 1 | 0x0 upper, 0x2 lower, 0x4 extra | func_0022C488, func_0022CA90 |
| Actor00403CF8 | 2 | 1 | 0x0 unk0, 0x4 pos | func_00403CF8, func_0043CCF8 |
| Actor0040457C | 2 | 1 | 0x140 ai[1] | func_0040457C, func_0043D57C |
| ActorBlock | 2 | 1 | 0x0 size, 0x4 count, 0x8 objects[1] | func_0042BA40, func_00464960 |
| ActorLink | 2 | 1 | `u8 pad0[8]; void (*cb)();` | func_002628B8, func_0026342C |
| ActorMotion | 2 | 1 | 0x0 kind, 0x4 pos, 0x10 *region, 0x18 velocity | func_0026961C, func_00269780 |
| ActorMotionState | 2 | 2 | `char pad0[0x2C]; void *table; char pad30[0xC0]; void *update;` | func_002633D8, func_002638A0 |
| ActorPlacement | 2 | 1 | 0x4 actorIndex, 0xA level | func_0026488C, func_002664F4 |
| ActorPos | 2 | 1 | 0x0 unk0, 0x4 pos, 0x10 *region, 0x14 unk14 | func_002688C8, func_00268C48 |
| AnimData | 2 | 2 | 0x14 unk14 | func_00232ECC, func_00290D5C |
| AnimFrame | 2 | 2 | 0x0 kind, 0x2 shiftS, 0x3 shiftT | func_0022D2FC, func_00234348 |
| AnimHeader | 2 | 2 | 0x0 radius, 0x4 frames, 0x8 encoding, 0xC fC, 0x10 rotTracks, 0x14 posTracks, 0x18 frameCount, 0x1C frameStep | func_00216A9C, func_00218B10 |
| AnimPlayer | 2 | 2 | 0x0 time, 0x4 current, 0x6 next, 0x8 frames, 0xA active, 0xB dirty, 0xC keys, 0x10 *data | func_00218B10, func_00284660 |
| Arena | 2 | 2 | 0x0 base, 0x4 cur, 0x8 size, 0xC count | func_002912F0, func_00402658 |
| Attached002534E0 | 2 | 1 | 0xD0 unkD0, 0x140 sub140[1] | func_002534E0, func_0025359C |
| AttackActor | 2 | 2 | 0x10 target, 0x14 *def | func_0025DF04, func_0025E248 |
| AttackState | 2 | 2 | 0x0 flags, 0xC4 anim, 0x133 index | func_0025DF04, func_0025E248 |
| AudioState | 2 | 1 | 0x0 flags, 0x2 unk2, 0x10 unk10, 0x14 unk14, 0x24 value | func_0029AA20, func_0029AC40 |
| Awords | 2 | 1 | `unsigned int w0; unsigned int w1;` | func_002946D0, func_00295304 |
| B01FH | 2 | 1 | `f32 f; s32 i; s16 h;` | func_002628B8, func_0026342C |
| B01Vec4 | 2 | 1 | `f32 x, y, z, w;` | func_002628B8, func_0026342C |
| B01W4 | 2 | 1 | `s32 i; f32 f; s16 h[2]; u8 b[4];` | func_002628B8, func_0026342C |
| B04A | 2 | 2 | `u8 pad0[4]; Vec3f pos; u8 pad10[0x40]; f32 unk50;` | func_0024599C, func_00246108 |
| B04B | 2 | 2 | `u8 pad0[0x48]; Vec3f pos; u8 pad54[0xC]; f32 unk60;` | func_0024599C, func_00246108 |
| B04Ent | 2 | 1 | `u32 mask; s32 val;` | func_0024646C, func_00246B24 |
| B04O | 2 | 1 | `u8 pad0[4]; s32 x, y, z;` | func_0024646C, func_00246B24 |
| B15Blk8 | 2 | 1 | `s16 a, b; s32 c;` | func_00239058, func_00239254 |
| B15Src | 2 | 2 | `u8 type; u8 pad1[3]; Vec3f pos;` | func_00239058, func_00239254 |
| B19Rec8 | 2 | 1 | `s32 a; s32 b;` | func_002323E0, func_002324E8 |
| B19Tbl | 2 | 1 | `B19Rec8 rec[14]; u16 w70[14]; u16 w8C[14]; u16 wA8[14];` | func_002323E0, func_002324E8 |
| BigState2BB0 | 2 | 2 | 0x2BB0 unk2BB0, 0x2BB4 unk2BB4 | func_002718D0, func_002718E0 |
| BindSlot | 2 | 1 | 0x0 *source, 0x4 unk4, 0x6 unk6, 0x7 unk7, 0x8 unk8, 0x9 unk9, 0xA unkA, 0xB unkB, ... | func_002929A0, func_00292A20 |
| BindSource | 2 | 1 | 0x0 unk0, 0x1 unk1, 0x2 unk2, 0xC unkC | func_002929A0, func_00292A20 |
| BindTable | 2 | 1 | 0x60 *slots | func_002929A0, func_00292A20 |
| BlastActor | 2 | 2 | 0x0 kind, 0x4 pos, 0x54 height, 0xD4 flags, 0x1A8 *owner | func_00237AFC, func_00238334 |
| BobDef | 2 | 1 | 0x30 eyeHeight | func_0024FC08, func_0024FE10 |
| BobObject | 2 | 1 | 0x50 yaw | func_0024FC08, func_0024FE10 |
| BobPlayer | 2 | 2 | 0x14 *def, 0xA10 time, 0xACC roll, 0xAD0 tilt, 0xAD4 kickX, 0xAD8 kickY, 0xADC kickZ, 0xAE0 bobX, ... | func_0024FC08, func_0024FE10 |
| BossAttack | 2 | 1 | 0x0 mode, 0x4 minDist, 0x8 maxDist | func_00404C44, func_0043DC44 |
| BossCtl | 2 | 1 | 0x0 unk0, 0x8012F9D4 state, 0x5C dest | func_00406000, func_0043F000 |
| BossDef | 2 | 2 | 0x40 time, 0x44 maxHelpers, 0x46 kills | func_004082A0, func_004088B0 |
| BossScript2 | 2 | 1 | 0x0 count, 0x8012F950 target, 0x8012F954 used, 0xB8 timer, 0x8012FA08 *script, 0x8012FA0C index, 0x8012FA10 current, 0x8012FA14 enabled | func_00404FE0, func_004050E0 |
| BossScript3 | 2 | 1 | 0x0 timer, 0x8012FA1C *script, 0x8012FA20 index, 0x8012FA24 current | func_00405288, func_004053C4 |
| BossStep2 | 2 | 2 | 0x0 type, 0x4 arg, 0x10 chance, 0x18 delay | func_00404FE0, func_004050E0 |
| BossStep3 | 2 | 1 | 0x0 type, 0x4 arg, 0x8 chance, 0xC delay, 0x10 delayRange | func_00405288, func_004053C4 |
| BossWait | 2 | 1 | 0x0 waitMin, 0x8012F618 waitMax | func_00404C44, func_0043DC44 |
| BreakableDefinition | 2 | 1 | `char reserved0[0x14]; s32 flags;` | func_002633D8, func_00265E20 |
| CKeyEvent | 2 | 1 | 0x6 param | func_0026E3EC, func_0026E46C |
| CLoaderEntry | 2 | 1 | 0x14 inUse | func_00201CC0, func_002020C8 |
| CacheEntry | 2 | 1 | 0x0 data, 0x4 key, 0x8 refCount, 0xC flags | func_00205904, func_002060E0 |
| CameraPlayer | 2 | 2 | 0x140 m_AIFlags, 0x51C *m_pCamera, 0xA1C m_Flags, 0x1320 *m_pNext | func_0027CAB0, func_0027D160 |
| CameraText | 2 | 1 | `u8 data[0x40];` | func_0042FBA4, func_00468AC4 |
| Cell | 2 | 1 | 0x0 maxZ | func_00221D3C, func_002222E0 |
| Chars4 | 2 | 1 | `char c[4];` | func_0041FCF4, func_00458C14 |
| Chunk | 2 | 2 | 0x0 *data, 0x4 flag | func_0027B40C, func_004034E0 |
| CinemaEntry | 2 | 1 | 0x0 id0, 0x4 id4, 0x8 id, 0xC flags, 0x3C aspect | func_0040F240, func_00448240 |
| CinemaTable | 2 | 1 | 0x8 entries[1] | func_0040F240, func_00448240 |
| Client | 2 | 1 | 0x0 *next, 0x4 *queue | func_0028A020, func_0028A17C |
| CollisionBuildVertex | 2 | 1 | `f32 x; f32 y; f32 z; f32 ceiling;` | func_00269A90, func_0042B6B0 |
| CollisionOut | 2 | 2 | 0x8 unk8, 0x9C unk9C, 0xA0 unkA0, 0xD8 vel | func_00239F00, func_0023AB18 |
| ColorF | 2 | 1 | `f32 r, g, b;` | func_00211DB0, func_00212DA4 |
| ControllerProfile | 2 | 2 | `u8 data[0x224];` | func_002568CC, func_00417C0C |
| Crosshair | 2 | 1 | 0x0 unk0, 0x4 unk4, 0x8 unk8, 0x90 unk90, 0x94 unk94, 0x98 unk98, 0x9C unk9C, 0xA0 unkA0, ... | func_0042DE74, func_00466D94 |
| Cue | 2 | 1 | 0x10 time | func_0040F240, func_00448240 |
| CueList | 2 | 1 | 0x4 count, 0x8 cues[1] | func_0040F240, func_00448240 |
| CueObject | 2 | 1 | 0x4 id4, 0xA idA, 0xE unkE, 0x11 kind, 0x12 val | func_0040F240, func_00448240 |
| CueRef | 2 | 1 | `s32 unk0; s32 unk4; s32 unk8;` | func_0040F240, func_00448240 |
| Cylinder | 2 | 2 | 0x0 pos, 0xC unkC, 0x10 radiusSq | func_0023CEC0, func_00242284 |
| DLNode | 2 | 2 | 0x0 *next, 0x4 *prev | func_0026D2D0, func_002767F8 |
| Damage | 2 | 2 | `char pad[0x18];` | func_00242E34, func_0025B2D4 |
| DefInfo | 2 | 1 | `char pad0[4]; u16 kind;` | func_00241CB0, func_00242604 |
| DirLight | 2 | 1 | 0x0 x, 0x2 y, 0x4 z, 0x6 w, 0x8 angle0, 0xA angle1, 0xC color[4] | func_0042D220, func_00466140 |
| DlCmd | 2 | 1 | `u32 w0; u32 w1;` | func_00223F8C, func_00235C30 |
| DrawArgs | 2 | 2 | 0x0 near, 0x4 *view, 0x8 **model, 0xC *offset, 0x10 unk14, 0x14 unk18, 0x18 unk1C | func_0021D39C, func_0025883C |
| DrawItem | 2 | 2 | 0x0 *list, 0x4 *matrix, 0x8 *lights, 0xC *textures, 0x10 unk10, 0x12 unk12, 0x14 segment, 0x18 *next | func_0020A568, func_0020A994 |
| Drawable | 2 | 2 | 0x80 unk80, 0x88 flags, 0x8A frame | func_00214E88, func_0022077C |
| EffectInfo | 2 | 1 | 0x0 flags | func_0022C488, func_0022CA90 |
| EffectSystem | 2 | 2 | 0xEC00 free, 0xEC14 groups, 0xEC28 lists[3], 0xEC64 resource | func_0022D2FC, func_00232CD8 |
| EmitDef | 2 | 1 | `u8 pad0[2]; u8 unk2; u8 unk3; u8 pad4[0x19]; u8 unk1D; u8 pad1E[3]; u8...` | func_00236A34, func_00236D50 |
| EmitOwner | 2 | 2 | `u8 pad0[0xF8]; struct EmitOwnerInfo *info;` | func_00236A34, func_00236D50 |
| EmitOwnerInfo | 2 | 1 | `u8 pad0[0x14]; s32 unk14;` | func_00236A34, func_00236D50 |
| EmitterDef | 2 | 2 | 0x0 texture, 0x2 blue, 0x5 envBlue, 0x8 spinRate, 0xC textureScale, 0x10 phaseRate, 0x14 baseSpeed, 0x18 fadeDepth, ... | func_00282340, func_00282E4C |
| EmitterView | 2 | 2 | 0x24C fan[5], 0x2F8 clipPlane[4] | func_00282340, func_00283238 |
| Enemy0040457C | 2 | 1 | 0x144 health, 0x1EC unk1EC, 0x1F0 range, 0x1F4 unk1F4, 0x1F8 region, 0x28C *next | func_0040457C, func_0043D57C |
| Entity0040457C | 2 | 1 | 0x0 unk0, 0x4 pos, 0x10 region, 0x18 rot | func_0040457C, func_0043D57C |
| Entry0026C5C4 | 2 | 1 | 0x0 key, 0x2 flags, 0x10 *child0, 0x14 *child1, 0x18 *child2 | func_0026C5C4, func_0026C934 |
| Floats | 2 | 1 | `f32 unk0, unk4, unk8, unkC, unk10, unk14;` | func_0042F9B0, func_004688D0 |
| FontStyle | 2 | 2 | 0xA0 envR, 0xA4 envG, 0xA8 envB, 0xAC primR, 0xB0 primG, 0xB4 primB, 0xB8 width, 0xBC height, ... | func_0027874C, func_00278CDC |
| FrameBuffers | 2 | 2 | `u8 pad0[0x110]; u32 depthBuffer;` | func_00285E60, func_004004D8 |
| GameSetupCopy | 2 | 1 | `s32 data[0x184 / 4];` | func_0042EF2C, func_00467E4C |
| GameSetupPlayer | 2 | 1 | 0x0 active, 0x8 team | func_0042EF2C, func_00467E4C |
| GameState_41363C | 2 | 1 | 0x1 multiplayer, 0x198 mode | func_00413230, func_0044C230 |
| GameState_413F74 | 2 | 1 | 0x1 multiplayer, 0x198 mode | func_00413230, func_0044C230 |
| GameState_4143DC | 2 | 1 | 0x1 multiplayer, 0x198 mode, 0x1B4 paused | func_00413230, func_0044C230 |
| GameplayCamera | 2 | 2 | `char pad0[0x2D8]; char clipPlanes[0x60]; CVector3 clipBounds[2];` | func_0022782C, func_002745DC |
| GeomHeader | 2 | 1 | `s32 data[7];` | func_00429FA0, func_00462EC0 |
| GfxPool | 2 | 1 | 0x0 *gfx, 0x4 *dynamic, 0x8 *fb[3], 0xC *depth, 0x18 *vtx | func_00425220, func_0045E140 |
| GfxWords | 2 | 1 | `u32 w0; u32 w1;` | func_00424780, func_0045D6A0 |
| GfxWords_414938 | 2 | 1 | `u32 w0, w1;` | func_00413230, func_0044C230 |
| Globals | 2 | 1 | 0x0 *menus, 0xCC5 timed, 0xE78 paused, 0xE84 match | func_00251FC4, func_0025492C |
| GraphNode | 2 | 1 | 0x0 group, 0x2 flags, 0x10 *neighbours[3] | func_0026CE44, func_00401CE8 |
| Heap | 2 | 2 | 0x0 *start, 0x4 *end, 0x8 *free8, 0xC *freeC, 0x10 *first | func_00202474, func_002912A0 |
| HitBuffer | 2 | 1 | `u8 data[0xF0];` | func_002688C8, func_00268C48 |
| HitInfo | 2 | 2 | `u8 data[0x18];` | func_00237AFC, func_0025B078 |
| Holder | 2 | 2 | 0x10 *unk10 | func_0021793C, func_00407CB0 |
| Hud | 2 | 1 | 0x90 unk90, 0x94 unk94, 0x98 unk98, 0x9C unk9C, 0x148 unk148[0x80] | func_0042EB50, func_00467A70 |
| Info0040457C | 2 | 1 | 0x24 unk24 | func_0040457C, func_0043D57C |
| K7Blk | 2 | 1 | `s32 w[6];` | func_0041F32C, func_0045824C |
| K7Mem | 2 | 1 | `s32 addr;` | func_0041F32C, func_0045824C |
| K7Obj | 2 | 1 | `s16 state; u8 pad2[0x12]; s32 f14; u8 pad18[4]; s32 f1C; K7Sub *f20;` | func_0041F32C, func_0045824C |
| K7Sub | 2 | 1 | `u8 pad0[4]; s8 b4;` | func_0041F32C, func_0045824C |
| KeyInfo | 2 | 2 | `char pad0[0x14]; s32 flags;` | func_0023ECF8, func_00268E60 |
| Level00403CF8 | 2 | 1 | 0xC unkC, 0x2C *data, 0x37 unk37, 0xF0 *onReset, 0xF4 *onEvent, 0xF8 *onHit, 0x108 *onDraw | func_00403CF8, func_0043CCF8 |
| LevelNode | 2 | 1 | 0x0 unk0, 0x4 count, 0x8 records[1] | func_0042BA40, func_00464960 |
| LevelRecord | 2 | 1 | 0x0 unk0, 0x4 pos, 0x20 spawnId, 0x22 defId, 0x24 angle | func_0042BA40, func_00464960 |
| LevelState | 2 | 2 | 0x98 *handle98, 0x9C *handle9C, 0xA0 *handleA0, 0xA4 *handleA4, 0xA8 *handleA8, 0xAC *handleAC, 0xB0 *handleB0, 0xB4 *handleB4, ... | func_0042C9DC, func_0042CD04 |
| Light00403CF8 | 2 | 1 | `u8 data[0x64];` | func_00403CF8, func_0043CCF8 |
| Limits | 2 | 1 | 0x18 accel | func_00260730, func_00260D60 |
| LinkList | 2 | 1 | 0x0 *head, 0x4 *tail | func_0028E9E0, func_0028EA10 |
| List002534E0 | 2 | 1 | 0x20 *head | func_002534E0, func_0025359C |
| ListNode | 2 | 2 | 0x16 unk16 | func_00228B4C, func_002933E0 |
| ListOwner | 2 | 2 | 0x14 m_List | func_00228B4C, func_00281974 |
| M04Ctx | 2 | 1 | `u8 pad0[0x84]; u8 player[0x58]; s16 snd[16]; u8 padFC[8]; s32 unk104; ...` | func_00272278, func_00272B54 |
| M04Emitter | 2 | 1 | `s32 w0; s32 w4; s32 w8; s32 id; M04VP vp; M04Rec12 env; f32 envA; f32 ...` | func_00272278, func_00272B54 |
| M04Rec12 | 2 | 1 | `u16 w0, w1, w2; s16 w3; u16 w4, w5;` | func_00272278, func_00272B54 |
| M04Tgt | 2 | 1 | `u8 pad0[0x114]; f32 x; f32 y; f32 z; u8 pad120[0x528 - 0x120]; f32 f52...` | func_00272278, func_00272B54 |
| M1B1 | 2 | 1 | `s8 s; u8 u;` | func_00263DD8, func_00265B58 |
| M1IF | 2 | 1 | `s32 i; f32 f;` | func_00263DD8, func_00265B58 |
| M1W4 | 2 | 1 | `s32 i; f32 f; s16 h[2]; u16 hu[2]; u8 b[4]; u32 u;` | func_00263DD8, func_00265B58 |
| M5Cell | 2 | 1 | `f32 x0, z0, x1, z1;` | func_002132B8, func_00213A70 |
| M5Ent | 2 | 2 | `s32 *a; M5Cell *cell;` | func_002132B8, func_00213A70 |
| M5Grid | 2 | 1 | 0x14A8 ent[1] | func_002132B8, func_00213A70 |
| MapSweep | 2 | 2 | 0x0 *sweep, 0x4 *a, 0x8 *b, 0xC side, 0x18 edge, 0x24 toEnd, 0x30 wallPoint, 0x3C normal, ... | func_00240800, func_00240E90 |
| MatchConfig | 2 | 1 | 0x0 mode, 0x2 playerCount, 0x4 unk4, 0x6 unk6, 0x8 teams[4], 0x10 a[4], 0x18 b[4], 0x20 c[4] | func_0042EF2C, func_00467E4C |
| MatchSettings | 2 | 2 | 0x19 messages | func_002478C8, func_00247FB0 |
| MatrixF | 2 | 1 | `f32 m[16];` | func_0021A7EC, func_0022A1A4 |
| MemoryRange | 2 | 1 | 0x0 first, 0x4 last, 0x8 flags | func_002A1DEC, func_002A2104 |
| MenuArg | 2 | 1 | `u8 pad0[0x1C]; s32 player; MenuPad *pad;` | func_0042019C, func_004590BC |
| MenuBinding_413230 | 2 | 1 | 0x0 *name, 0x4 type, 0x8 min, 0xC max, 0x10 step, 0x14 scale, 0x18 *item) | func_00413230, func_0044C230 |
| MenuBinding_4133A4 | 2 | 1 | 0x0 *name, 0x4 type, 0x8 min, 0xC max, 0x10 step, 0x14 unk14, 0x18 *item) | func_00413230, func_0044C230 |
| MenuBlob | 2 | 1 | 0x0 unk0, 0x4 unk4, 0x8 unk8, 0x14 unk14, 0x18 unk18, 0x1C unk1C, 0x30 unk30 | func_00413230, func_0044C230 |
| MenuBox_4143DC | 2 | 1 | 0x4 width, 0x8 height, 0xC scaleX, 0x10 scaleY, 0x14 x, 0x1C y | func_00413230, func_0044C230 |
| MenuBox_4145B0 | 2 | 1 | 0x14 x, 0x1C y | func_00413230, func_0044C230 |
| MenuCamera | 2 | 1 | 0x288 width, 0x28C height | func_00413230, func_0044C230 |
| MenuCamera_414938 | 2 | 1 | 0x28 yaw, 0x288 width, 0x28C height, 0x368 mtx[2] | func_00413230, func_0044C230 |
| MenuColumn_4145B0 | 2 | 1 | 0x14 left, 0x18 right | func_00413230, func_0044C230 |
| MenuController_41531C | 2 | 1 | 0xB0 buttons | func_00413230, func_0044C230 |
| MenuEntry | 2 | 1 | `u8 pad0[8]; u32 flags; u8 padC[0x1C];` | func_00455FA0, func_00456948 |
| MenuFx_414938 | 2 | 1 | 0x4 pos, 0x14 *sub, 0x34 scale, 0x50 rotY, 0x194 pos194, 0x234 *draw | func_00413230, func_0044C230 |
| MenuImage | 2 | 1 | 0x24 width, 0x28 height | func_00413230, func_0044C230 |
| MenuInfo | 2 | 1 | 0x0 f0, 0x4 width, 0x8 height, 0xC scaleX, 0x10 scaleY, 0x14 y1 | func_00413230, func_0044C230 |
| MenuInfo_4145B0 | 2 | 1 | 0x0 f0, 0x4 width, 0x8 height, 0xC scaleX, 0x10 scaleY, 0x14 y1 | func_00413230, func_0044C230 |
| MenuItemDef_41531C | 2 | 1 | 0x18 *arg), 0x1C *arg) | func_00413230, func_0044C230 |
| MenuItemExt | 2 | 1 | 0x0 id, 0x4 blob, 0x328 valid, 0x32C unk32C | func_00413230, func_0044C230 |
| MenuItemTmpl_4160F0 | 2 | 1 | 0x4 type, 0x14 *text | func_00413230, func_0044C230 |
| MenuItemTmpl_416188 | 2 | 1 | 0x17 ch | func_00413230, func_0044C230 |
| MenuItemTmpl_416894 | 2 | 1 | 0x0 type | func_00413230, func_0044C230 |
| MenuItemTmpl_416BAC | 2 | 1 | 0x4 type, 0x14 *text | func_00413230, func_0044C230 |
| MenuItem_413230 | 2 | 1 | 0x14 *binding | func_00413230, func_0044C230 |
| MenuItem_4133A4 | 2 | 1 | 0x14 *binding | func_00413230, func_0044C230 |
| MenuItem_41363C | 2 | 1 | 0x0 idx, 0x4 kind, 0x8 flags, 0xC x, 0xE y, 0x10 b13, 0x14 data, 0x18 *def, ... | func_00413230, func_0044C230 |
| MenuItem_413F74 | 2 | 1 | 0x0 idx, 0x4 kind, 0x8 flags | func_00413230, func_0044C230 |
| MenuItem_4143DC | 2 | 1 | 0x8 flags, 0x14 image | func_00413230, func_0044C230 |
| MenuItem_4145B0 | 2 | 1 | 0x0 idx, 0x4 kind, 0x8 flags, 0xC x, 0xE y, 0x10 b13, 0x14 data, 0x18 *def, ... | func_00413230, func_0044C230 |
| MenuItem_414938 | 2 | 1 | 0x8 flags, 0x14 *part, 0x20 *model | func_00413230, func_0044C230 |
| MenuItem_41531C | 2 | 1 | 0x0 index, 0x4 type, 0x8 flags, 0x18 *def, 0x1C value | func_00413230, func_0044C230 |
| MenuItem_416214 | 2 | 1 | 0x20 *ext | func_00413230, func_0044C230 |
| MenuMatchGlobals | 2 | 2 | `u8 unobserved0[0x20]; GameplayCamera *cameras; u8 unobserved24[0x40 - ...` | func_002712E8, func_00271694 |
| MenuModelPart_414938 | 2 | 1 | 0x0 type, 0x4 model, 0x8 variant, 0xC pos, 0x18 scale, 0x2C spin | func_00413230, func_0044C230 |
| MenuModel_414938 | 2 | 1 | 0x0 id, 0x4 f4, 0x8 f8, 0xC fC, 0x18 f18, 0x1C f1C, 0x20 f20, 0x34 height, ... | func_00413230, func_0044C230 |
| MenuPanel | 2 | 1 | 0x44 *ctx | func_00413230, func_0044C230 |
| MenuPool_416894 | 2 | 1 | 0x14 opened | func_00413230, func_0044C230 |
| MenuSelection_414938 | 2 | 1 | `s32 id;` | func_00413230, func_0044C230 |
| MenuStyle_41363C | 2 | 1 | 0x0 mode, 0x4 charWidth, 0x8 lineHeight | func_00413230, func_0044C230 |
| MenuStyle_413F74 | 2 | 1 | 0x0 mode, 0x4 f4, 0x8 f8, 0xC scaleX, 0x10 scaleY, 0x14 shadow, 0x15 b1 | func_00413230, func_0044C230 |
| MenuTemplate_41531C | 2 | 1 | 0x14 (*poll)(void) | func_00413230, func_0044C230 |
| MenuTmpl_416894 | 2 | 1 | 0x0 *items, 0x4 count | func_00413230, func_0044C230 |
| MenuTmpl_416B30 | 2 | 1 | 0xC *pool) | func_00413230, func_0044C230 |
| MenuWindow_4143DC | 2 | 1 | 0x30 fade, 0x34 alpha, 0x3C highlighted | func_00413230, func_0044C230 |
| MenuWindow_414938 | 2 | 1 | 0x0 id, 0x30 fade, 0x34 alpha, 0x40 *cam | func_00413230, func_0044C230 |
| Menu_4133A4 | 2 | 1 | 0x20 controller | func_00413230, func_0044C230 |
| Menu_41531C | 2 | 1 | 0x0 cursor, 0xC *items, 0x10 count, 0x14 *tmpl, 0x20 *ctl, 0x28 state, 0x2C time, 0x30 fade, ... | func_00413230, func_0044C230 |
| MesgLock | 2 | 1 | 0x0 queue[0x1C], 0x1C count | func_002019F0, func_002714B0 |
| Missile | 2 | 2 | 0x4 pos, 0x18 vel, 0x40 flags, 0xF8 *parent, 0x114 *target, 0x11C speed, 0x150 dir, 0x1B8 *loop | func_002298C4, func_00229D10 |
| ModeHandler | 2 | 2 | 0x0 *ai), 0x4 *ai), 0x8 anim[3] | func_0025C5F8, func_0040ECDC |
| ModeState00403C14 | 2 | 1 | 0x0 mode, 0x354 unk354, 0x380 values[2][8], 0x3C0 unk3C0, 0x3C4 unk3C4, 0x3C8 unk3C8, 0x3C9 unk3C9 | func_00403C14, func_0043CC14 |
| ModeState00403CF8 | 2 | 1 | 0x0 mode, 0xC unk00C, 0x18 unk018, 0x1C unk01C, 0x24 lights[8], 0x344 *handle, 0x348 unk348, 0x350 unk350, ... | func_00403CF8, func_0043CCF8 |
| ModeState0040457C | 2 | 1 | 0x0 mode, 0x344 handle, 0x348 spawnsLeft, 0x34C wavesLeft, 0x350 enemiesAlive, 0x354 spawnTimer, 0x358 region, 0x35C doorsOpen, ... | func_0040457C, func_0043D57C |
| ModelInfo_414938 | 2 | 1 | `s32 w[0x27];` | func_00413230, func_0044C230 |
| ModelRecord | 2 | 1 | 0x44 flags | func_00251698, func_0026C7A8 |
| ModelSub_414938 | 2 | 1 | 0xC h0C | func_00413230, func_0044C230 |
| ModuleObj | 2 | 2 | 0x174 animState | func_0040A25C, func_0040BEE4 |
| MorphClock | 2 | 1 | 0x8 time | func_0023353C, func_00233868 |
| MorphItem | 2 | 1 | 0x0 colorA[4], 0x4 colorB[4], 0x8 pointA, 0xA pointB | func_0023353C, func_00233868 |
| MorphOwner | 2 | 1 | 0x8 *timing, 0x24 remaining | func_0023353C, func_00233868 |
| MorphPoint | 2 | 1 | 0x0 pos | func_0023353C, func_00233868 |
| MorphTiming | 2 | 1 | 0x4 fadeTime, 0x8 duration | func_0023353C, func_00233868 |
| MotionState | 2 | 1 | `u32 flags; u8 pad4[0x3C]; f32 timer;` | func_00264478, func_00264644 |
| Mount | 2 | 2 | 0x0 kind, 0x4 pos, 0x50 yaw, 0xB8 type, 0x140 ai[4] | func_0024EAF8, func_00251A2C |
| Mtx44f | 2 | 1 | `f32 m[4][4];` | func_00215884, func_002374AC |
| Mtx4f_414938 | 2 | 1 | `f32 m[16];` | func_00413230, func_0044C230 |
| NameEdit | 2 | 2 | 0x0 length, 0x4 cursor, 0x8 end | func_0041D050, func_0041D51C |
| Node002534E0 | 2 | 1 | 0xA50 *attached, 0x1320 *next | func_002534E0, func_0025359C |
| NodePool | 2 | 1 | 0x2C *freeList | func_00294B58, func_00294B84 |
| O01Cfg | 2 | 1 | `u8 pad0[0xC]; f32 unkC; f32 unk10; f32 unk14; u8 pad18[4]; s32 unk1C; ...` | func_00421E78, func_0045AD98 |
| O01OwnC | 2 | 1 | `u8 pad0[0x1C]; O01Actor *unk1C; O01SubC *unk20;` | func_0041DF50, func_00456E70 |
| O01OwnJ | 2 | 1 | `u8 pad0[0x14]; s32 unk14; u8 pad18[4]; O01Plr3 *unk1C; O01Sub9 *unk20;` | func_0041EDE0, func_00457D00 |
| O01Pad | 2 | 1 | `s32 v; s32 freeb; s32 cnt;` | func_0041EDE0, func_00457D00 |
| O01Plr3 | 2 | 1 | `u8 pad0[0xA50]; s32 unkA50;` | func_0041EDE0, func_00457D00 |
| O01Sel | 2 | 1 | `s32 v; s32 w;` | func_0041DF50, func_00456E70 |
| O01Sub9 | 2 | 1 | `u8 pad0[4]; s8 unk4;` | func_0041EDE0, func_00457D00 |
| O01SubC | 2 | 1 | `u8 pad0[4]; s8 unk4;` | func_0041DF50, func_00456E70 |
| O06A_415FB8 | 2 | 1 | `u8 pad[0x14]; s16 unk14;` | func_00413230, func_0044C230 |
| O06A_416644 | 2 | 1 | `u8 pad[0x14]; s16 unk14;` | func_00413230, func_0044C230 |
| O06A_416760 | 2 | 1 | `u8 pad[0x14]; s16 unk14;` | func_00413230, func_0044C230 |
| O06A_416C74 | 2 | 1 | `u8 pad[0x40]; O06V *unk40; u8 pad2[0x184]; s32 unk1C8; s32 unk1CC;` | func_00413230, func_0044C230 |
| O06A_4171F4 | 2 | 1 | `u8 pad[0x110]; O06Seg seg[1];` | func_00413230, func_0044C230 |
| O06B_415FB8 | 2 | 1 | `O06I_415FB8 *unk0; s16 unk4; u8 pad6[0x1A]; s32 unk20;` | func_00413230, func_0044C230 |
| O06B_416644 | 2 | 1 | `O06I_416644 *unk0; s16 unk4;` | func_00413230, func_0044C230 |
| O06B_416760 | 2 | 1 | `O06I_416760 *unk0; s16 unk4;` | func_00413230, func_0044C230 |
| O06C_415FB8 | 2 | 1 | `u8 pad[0x18]; O06B_415FB8 *unk18; s32 unk1C; s32 unk20; s32 unk24;` | func_00413230, func_0044C230 |
| O06C_416760 | 2 | 1 | `u8 pad[0x14]; s32 unk14; s32 unk18; s32 unk1C; s32 unk20; s32 unk24;` | func_00413230, func_0044C230 |
| O06Cam | 2 | 1 | `f32 f0; s32 unk4; s32 unk8; f32 fC; s32 unk10;` | func_00413230, func_0044C230 |
| O06Gfx | 2 | 1 | `u32 w0; u32 w1;` | func_00413230, func_0044C230 |
| O06I_415FB8 | 2 | 1 | `s16 unk0; u8 pad[0x22];` | func_00413230, func_0044C230 |
| O06I_416644 | 2 | 1 | `s16 unk0; u8 pad[0x22];` | func_00413230, func_0044C230 |
| O06I_416760 | 2 | 1 | `s16 unk0; u8 pad[0x22];` | func_00413230, func_0044C230 |
| O06Seg | 2 | 1 | `u32 w[6];` | func_00413230, func_0044C230 |
| O06V | 2 | 1 | `u8 pad[0x288]; f32 unk288; f32 unk28C; f32 unk290; f32 unk294;` | func_00413230, func_0044C230 |
| Obj1C8 | 2 | 1 | `char pad0[0x1C8]; s32 unk_1C8;` | func_00413230, func_0044C230 |
| OptionsBlock | 2 | 2 | `s8 pad0[0x20]; s8 option20;` | func_00418A34, func_00418EF4 |
| PackedBitWords | 2 | 1 | `u32 low; u32 high;` | func_002177DC, func_00217EFC |
| PackedRange | 2 | 1 | 0x0 base, 0x2 range | func_0022D2FC, func_0026D128 |
| PadSel | 2 | 1 | `s32 v; s32 w;` | func_0041EA84, func_004579A4 |
| PakInfo | 2 | 1 | `s32 v; s32 freeb; s32 cnt;` | func_0041F0C8, func_00457FE8 |
| ParticleEffect | 2 | 2 | 0x0 flags, 0x18 *impact | func_00230CB4, func_00232CD8 |
| ParticleView | 2 | 2 | 0x8 index, 0x24 unk24, 0x114 eye, 0x20C viewMtx[4][4], 0x510 fadeRange | func_0022D2FC, func_0023118C |
| Path | 2 | 2 | 0x40 *segments, 0x4C totalLength | func_00236B7C, func_00402658 |
| PathSearch | 2 | 2 | 0x0 *obj, 0x4 *startRegion, 0x8 startPos, 0x14 *goalRegion, 0x18 goalPos, 0x24 bounds[4], 0x34 pool[0x960], 0x994 *poolNext, ... | func_002393D0, func_002399F8 |
| Pickup | 2 | 2 | 0x4 pos, 0x14 *def, 0x18 vel, 0x14C boundsMin, 0x158 boundsMax, 0x19C timer | func_00227680, func_00265F90 |
| PickupDef | 2 | 2 | `char pad0[0x14]; PickupParams params;` | func_0023F634, func_00265F90 |
| PickupInfo | 2 | 2 | 0xC ammoType | func_00253C14, func_00265F90 |
| PlacedPropRecord | 2 | 2 | 0x0 value, 0x4 scale, 0x8 id, 0xA segment, 0xC flags, 0xE model, 0x10 y, 0x12 extents[6], ... | func_002141F0, func_00214564 |
| PlayClock | 2 | 2 | 0x0 frames, 0x4 seconds, 0x8 minutes, 0xC hours | func_00288FDC, func_004692CC |
| PlayerRecAt1C | 2 | 2 | `char pad0[0x1C]; Player *actor;` | func_0041CA00, func_00455960 |
| PlayerStateInfo | 2 | 2 | 0x0 *), 0x4 *), 0x8 *flags | func_0024C47C, func_0024E5F0 |
| Players | 2 | 2 | 0x1281 multiplayer | func_00255D70, func_00263594 |
| PoolNode | 2 | 2 | 0x0 *next, 0x14 *self, 0x31 unk31, 0x32 unk32, 0x33 unk33 | func_0029247C, func_00466040 |
| PoseInfo | 2 | 1 | 0x0 mode | func_00257D48, func_00258000 |
| Progress | 2 | 1 | 0x4 keys, 0x19 multiplayer, 0x1A warping | func_0041A5DC, func_0045353C |
| ProjVertex | 2 | 2 | 0x0 x, 0x4 y, 0x10 *pos | func_0023D4D4, func_0023D7CC |
| ProjectileDef | 2 | 2 | 0x2C *wobble | func_00229618, func_0022A1A4 |
| Q04B | 2 | 1 | `u8 pad[8]; u32 unk8;` | func_00413230, func_0044C230 |
| Q04Cm | 2 | 1 | `u8 pad[0x288]; f32 f288; f32 f28C; f32 f290; f32 f294;` | func_00413230, func_0044C230 |
| Q04DefB | 2 | 1 | `u8 pad[0x14]; void (*f14)(void *, void *, void *, void *);` | func_00413230, func_0044C230 |
| Q04Hdr | 2 | 1 | `void *unk0; s16 n; u8 pad6[2]; void (*unk8)(void *, void *); u8 pad0C[...` | func_00413230, func_0044C230 |
| Q04Info | 2 | 1 | `s32 f0, f4, f8; f32 fC, f10; s32 x0, x1, y0, y1;` | func_00413230, func_0044C230 |
| Q04Info_4156FC | 2 | 1 | `s32 f0, f4, f8; f32 fC, f10; s32 x0, x1, y0, y1;` | func_00413230, func_0044C230 |
| Q04Item | 2 | 1 | `s32 idx; s16 w4; u8 pad6[2]; u32 flags; u16 wC; u16 wE; u8 b10, b11, b...` | func_00413230, func_0044C230 |
| Q04ItemDef | 2 | 1 | `u8 pad[4]; u32 flags; u8 pad8[0x1C - 8]; void (*unk1C)(void *, void *,...` | func_00413230, func_0044C230 |
| Q04Item_4156FC | 2 | 1 | `s32 idx; s16 w4; u8 pad6[2]; u32 flags; u16 wC; u16 wE; u8 b10, b11, b...` | func_00413230, func_0044C230 |
| Q04Itm | 2 | 1 | `s32 idx; u16 w4; u8 pad6[2]; u32 flags; u16 wC; u16 wE; u8 b10, b11, b...` | func_00413230, func_0044C230 |
| Q04Panel_414EA8 | 2 | 1 | `u16 w0, w2, w4; u8 pad6[2]; void *p8; Q04Itm *items; s32 n; void *hdr;...` | func_00413230, func_0044C230 |
| Q04Pn | 2 | 1 | `s16 cur; s16 w2; s16 w4; u8 pad6[6]; Q04Item_4156FC *items; s32 n; u8 ...` | func_00413230, func_0044C230 |
| Q04Rec | 2 | 1 | `u16 w0, w2, w4, w6, w8, wA; u8 bC, bD, bE, bF;` | func_00413230, func_0044C230 |
| Q04Tab | 2 | 1 | `u16 a; u8 pad2[2]; u16 b; u8 pad6[2]; u16 c; u8 padA[3]; u8 d; u8 pad0...` | func_00413230, func_0044C230 |
| Q04Tpl | 2 | 1 | `s16 type; u8 pad2[2]; u32 flags; u16 w8; u16 wA; u8 bC, bD, bE, bF; s3...` | func_00413230, func_0044C230 |
| Q04V | 2 | 1 | `u8 pad[0x288]; f32 unk288; f32 unk28C; f32 unk290; f32 unk294;` | func_00413230, func_0044C230 |
| Q04W | 2 | 1 | `u8 pad0[2]; s16 unk2; s16 unk4; u8 pad6[6]; Q04Item *unkC; s32 unk10; ...` | func_00413230, func_0044C230 |
| Q4Z | 2 | 1 | `u8 pad0[8]; u32 f8; u8 padC[8]; void *f14;` | func_0041B05C, func_00453FC4 |
| QuadVtx | 2 | 1 | 0x0 ob[3], 0x6 flag, 0x8 tc[2], 0xC cn[4] | func_004302A0, func_004691C0 |
| Reader | 2 | 2 | 0x8 *p | func_002932BC, func_004034E0 |
| Record | 2 | 1 | 0x0 data[2], 0x8 valid, 0xC rest[4] | func_0042CE10, func_00465D30 |
| Record14 | 2 | 2 | `u8 data[0x14];` | func_00225058, func_002470E8 |
| Region80121378 | 2 | 1 | 0x0 *next, 0x4 start, 0x6 pages | func_0028E53C, func_0028EA40 |
| ResidentDataChunk | 2 | 1 | `s32 width; s32 count; u8 data[1];` | func_0042B3E4, func_00464304 |
| ResourceLoadRequest | 2 | 2 | 0x0 *extra, 0x4 key, 0x8 priority, 0xC flags, 0x10 *tag, 0x14 owner, 0x18 mode, 0x1C *replyQueue, ... | func_0020367C, func_002044A8 |
| S10Item | 2 | 1 | `u8 pad0[0x14]; void *f14;` | func_00418158, func_00451108 |
| ScTask | 2 | 2 | 0x0 *next, 0x4 state, 0x8 flags, 0xC *framebuffer, 0x10 list, 0x50 *msgQ, 0x54 *msg, 0x58 startTime, ... | func_00286B58, func_0028A17C |
| ScalarKey | 2 | 1 | 0xC value, 0x10 time | func_0040F240, func_00448240 |
| SceneEffectPool | 2 | 1 | 0x0 effects[0x20], 0x5300 freeEffects, 0x5314 activeEffects | func_00227C10, func_00227EE8 |
| Sched | 2 | 2 | 0x20 retraceMsg, 0x40 queue[0x18], 0x2E0 *clients | func_0028A020, func_0028A17C |
| ShadowParams | 2 | 2 | `u8 pad0[0x10]; s32 blobType;` | func_0021DC84, func_0022D2FC |
| Slot24 | 2 | 1 | `s32 w[6];` | func_0021ECA0, func_0021ED04 |
| SortEntry | 2 | 2 | 0x0 dist | func_0022077C, func_00226D0C |
| SortItem | 2 | 1 | 0x4 key | func_00226D30, func_002407D8 |
| SortNode | 2 | 2 | 0x0 key, 0x4 *material, 0x8 *head, 0xC *tail, 0x10 *prev, 0x14 *next, 0x18 *left, 0x1C *right | func_0020A568, func_0020A994 |
| SoundEvent | 2 | 2 | `LevelActor *object; char pad4[0x14 - 0x4]; SoundRange *range; char pad...` | func_00256738, func_0026E3A8 |
| SoundNode | 2 | 2 | 0x4 *next, 0x8 handle, 0xE sound, 0x10 pos, 0x1C flags | func_00275E1C, func_00275E98 |
| SoundObject | 2 | 2 | 0x4 pos[3], 0x1B8 *loop | func_00232AC4, func_00232B40 |
| SoundSlot | 2 | 2 | 0x4 *next, 0x8 handle, 0xC voice | func_00275EFC, func_00276DFC |
| Source | 2 | 2 | `u8 pad0[4]; Vec3 position; u8 pad10[4]; s32 *kind; u8 pad18[0x50 - 0x1...` | func_00245848, func_0025B078 |
| SpawnSource | 2 | 2 | 0x24 id, 0x80 spawnFlags, 0x88 flags | func_00214D04, func_00232168 |
| SpawnView | 2 | 2 | 0x0 pos, 0xD0 dir | func_00232044, func_00232168 |
| SpinState | 2 | 2 | 0x34 mode, 0x10C angle, 0x110 spin | func_002591F4, func_002649A0 |
| Spline | 2 | 1 | 0x0 nodeSize, 0x4 count, 0x8 nodes[1] | func_0040F240, func_00448240 |
| SplineNode | 2 | 1 | 0x0 p, 0xC d2p, 0x18 s, 0x1C w, 0x20 d2s | func_0040F240, func_00448240 |
| SplineSource | 2 | 1 | `s32 stride,count; union { char data[0]; Vec3 points[0]; f32 weights[0]...` | func_0040F240, func_00448240 |
| SplineSourceCursor | 2 | 1 | `SplineSource *header; Vec3 *points; f32 *weights;` | func_0040F240, func_00448240 |
| SplineTable | 2 | 1 | `s32 count;` | func_0040F240, func_00448240 |
| StateInfo | 2 | 2 | `u8 pad0[0x24]; u32 flags; u8 pad28[0x24]; f32 delay;` | func_00264478, func_00264644 |
| SwooshPoint | 2 | 1 | 0x4 y | func_002366C8, func_002368B0 |
| TableHolder | 2 | 1 | 0x14 *table | func_00421E44, func_00422608 |
| TargetActor | 2 | 2 | 0x4 pos | func_0022A1A4, func_002623EC |
| Task | 2 | 2 | 0x238 pending | func_00201C08, func_00468590 |
| Thread | 2 | 2 | 0xD4 *sp | func_0028BDFC, func_0028C0B0 |
| TimedCueRef | 2 | 1 | `f32 time; s32 unk4; s32 unk8;` | func_0040F240, func_00448240 |
| TimedNode | 2 | 2 | 0x0 *prev, 0x4 *next, 0x8 *item, 0xC timer | func_002253F8, func_00294C70 |
| TrackResult | 2 | 2 | 0x0 kind, 0x4 *target, 0x8 range, 0xC pos, 0x18 delta, 0x24 dist, 0x28 flatPos, 0x34 flatDelta, ... | func_00244338, func_002449E0 |
| Tracker | 2 | 2 | 0x0 unk0, 0x4 value, 0xB4 repeated | func_0025360C, func_0040E55C |
| TrailInfo | 2 | 2 | 0xC count | func_00232934, func_002350EC |
| TrailMgr | 2 | 2 | 0x7514 freeList[0xC], 0x7520 *trails | func_002350EC, func_002352F8 |
| TrailSlot | 2 | 1 | `u8 pad0[0x3C]; u8 trail[0x64 - 0x3C];` | func_0040ECDC, func_0040EEA0 |
| Unk00264900 | 2 | 1 | 0x4 unk4, 0xA unkA | func_00264900, func_00266540 |
| Unk00401C78 | 2 | 1 | 0x4 state | func_00401C78, func_00401C88 |
| Unk004220F0 | 2 | 1 | 0x14 *table | func_0041C034, func_004220F0 |
| Unk2B4C | 2 | 1 | 0x2B4C unk2B4C, 0x2B50 table, 0x2BA0 unk2BA0, 0x2BB0 unk2BB0 | func_00271740, func_002718EC |
| Unk800F56B0 | 2 | 1 | 0x0 active, 0x4 unk4, 0x8 unk8, 0xC unkC, 0x1C8 unk1C8, 0x350 index | func_0042F4F0, func_0042F590 |
| Unk8011ACB0 | 2 | 2 | 0x1C unk1C, 0x20 unk20, 0x24 unk24, 0x28 unk28, 0x44 unk44, 0x48 unk48 | func_002534E0, func_00286440 |
| Vec3_00403CF8 | 2 | 1 | `f32 x, y, z;` | func_00403CF8, func_0043CCF8 |
| Vec3_0040457C | 2 | 1 | `f32 x, y, z;` | func_0040457C, func_0043D57C |
| Vec3f_414938 | 2 | 1 | `f32 x, y, z;` | func_00413230, func_0044C230 |
| Vec3i | 2 | 1 | `s32 x, y, z;` | func_00228BC4, func_00273E98 |
| Vec5 | 2 | 1 | `f32 v[6];` | func_0042F9B0, func_004688D0 |
| VoiceOwner | 2 | 2 | 0x1180 activeVoices, 0x1194 freeVoices | func_00225354, func_002748D0 |
| WarpPoint | 2 | 1 | 0x0 pos, 0xC angle, 0x10 level, 0x12 room | func_00248BA8, func_00430454 |
| Wave0040457C | 2 | 1 | 0x0 flag, 0x8 count, 0xC delay, 0x14 type | func_0040457C, func_0043D57C |
| WeaponEntry | 2 | 2 | 0x0 weapon | func_00254F88, func_002550E8 |
| WeaponEntryList | 2 | 2 | 0x18 count, 0x1C entries[1] | func_00254F88, func_002550E8 |
| Widget | 2 | 2 | `char pad0[0x2F]; s8 style; char pad30[0x33 - 0x30]; u8 colors[8];` | func_0041BC50, func_00454B64 |
| ZoomPlayer | 2 | 2 | 0xB90 zoom | func_00247170, func_00247660 |
| __OSBlockInfo | 2 | 1 | `u32 errStatus; void *dramAddr; void *C2Addr; u32 sectorSize; u32 C1Err...` | func_0029CAF0, func_0029D690 |
| __OSTranxInfo | 2 | 1 | `u32 cmdType; u16 transferMode; u16 blockNum; s32 sectorNum; u32 devAdd...` | func_0029CAF0, func_0029D690 |
| du | 2 | 1 | `struct { u32 hi; u32 lo; } w; f64 d;` | func_002974C0, func_002982F0 |
| fu | 2 | 1 | `u32 w; f32 f;` | func_002974C0, func_002982F0 |

## Files that are readable only on paper

`tools/progress.py` counts these as readable (no m2c offset macros), but
they still reach fields by raw pointer arithmetic (`*(s32 *) ((u8 *) p + 0x1C)`
and the like), because no struct was found that kept the codegen:

func_00204FD0, func_00271904, func_00293254, func_0041B9DC, func_004533F4,
func_00268534, func_00268080, func_002680D0, func_002715F0, func_00271570,
func_004194F8, func_00281B14.

These keep a few casted stores on purpose, for the codegen (the rest of the
file is named): func_004402C8, func_00455D10, func_002748D0.

Still m2c (the M2C_ macros are left in; they are the 10 files
`tools/progress.py` does not count as readable): func_0044068C,
func_0043EC68, func_00214DBC, func_0025EEA4, func_0022629C, func_002065F8,
func_002578AC, func_002555CC, func_00258964, func_0025A304.
