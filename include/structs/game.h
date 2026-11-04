#ifndef STRUCTS_GAME_H
#define STRUCTS_GAME_H

/* The game settings block, D_8011AAD8 (the game state at D_80119870 +
 * 0x1268), and the match state inside it, D_8011ACB0 (+ 0x1D8).
 *
 * The first 0x184 bytes are what the save and the multiplayer menu copy
 * (func_0042F514, func_0041D890). Rage Wars' Shared_Game (D_801462C8, at the
 * same place in its game state) is the same block with 4 more bytes before
 * 0x08, so its `local` at 0x1D is `multiplayer` here at 0x19.
 *
 * Several fields also have symbols of their own, which some files still
 * use: D_8011AAE0 (difficulty), D_8011AAF1 (multiplayer), D_8011AB08
 * (weaponEnabled), D_8011AB18 (defaultSlot), D_8011AB58 (slots), D_8011AC88
 * (mode), D_8011ACA8 (matchMode) and D_8011ACB0 (match). Three files keep a
 * local array view of D_8011AAD8 / D_8011ACB0 (func_00222A9C, func_00286440,
 * func_00450D78): through an array the compiler bases the accesses on the
 * accessed field's address, as the ROM does there. */

#include "structs/player_options.h"

/* The match state, D_8011ACB0: the play clock, then the rules of the
 * running match (Rage Wars' RenderCounters / GameRules at D_801468A0). */
typedef struct GameMatch {
    /* 0x00 */ f32 frames;             /* the play clock (func_00288FDC) */
    /* 0x04 */ f32 seconds;
    /* 0x08 */ f32 minutes;
    /* 0x0C */ f32 hours;
    /* 0x10 */ f32 countdown;
    /* 0x14 */ f32 timeLeft;           /* minutes * 900; -1: no limit (func_00417BB0) */
    /* 0x18 */ s32 scoreLimit;
    /* 0x1C */ s32 over;
    /* 0x20 */ s32 done;
    /* 0x24 */ s32 teams;              /* a team match */
    /* 0x28 */ s32 teamOver;
    /* 0x2C */ s32 teamScore[2];       /* func_00256804 */
    /* 0x34 */ s32 tag;                /* a tag match */
    /* 0x38 */ s32 tagPick;            /* who is "it" (func_002525BC) */
    /* 0x3C */ s32 tagPickStart;
    /* 0x40 */ s32 tagPhase;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 unk4C;
} GameMatch; /* size 0x50 */

/* The game settings, D_8011AAD8. The options at 0x20 come in three groups
 * of five, one per kind of match (func_0041760C, func_00417490 and
 * func_0041778C start them): deathmatch, tag and team. */
typedef struct GameSettings {
    /* 0x000 */ u32 cheatFlags;        /* the cheats on: 0x4000 black fog, 0x800/0x1000 big heads... */
    /* 0x004 */ u32 unlockedCheats;    /* set by the cheat codes (func_0041AD64) and bosses (func_00407F20) */
    /* 0x008 */ s32 difficulty;         /* D_8011AAE0: 8, 16 or 32 (func_002427E0) */
    /* 0x00C */ u32 unkC;              /* shown in sixteenths */
    /* 0x010 */ u16 unk10;
    /* 0x012 */ u8 unk12[0x14 - 0x12];
    /* 0x014 */ u8 unk14;              /* 0x80-centred */
    /* 0x015 */ u8 unk15;              /* 0x80-centred */
    /* 0x016 */ u8 unk16;              /* 0x80-centred */
    /* 0x017 */ u8 unk17;
    /* 0x018 */ u8 unk18;
    /* 0x019 */ u8 multiplayer;        /* D_8011AAF1 */
    /* 0x01A */ u8 playerCount;
    /* 0x01B */ u8 unk1B;
    /* 0x01C */ f32 speed;             /* game speed, 0.5 to 2 (func_00451804) */
    /* 0x020 */ s8 dmTimeLimit;        /* minutes */
    /* 0x021 */ s8 dmScoreLimit;
    /* 0x022 */ s8 dmArena;            /* 0: random */
    /* 0x023 */ s8 dmSkill;            /* the level actors spawn at (func_002427E0) */
    /* 0x024 */ u8 dmUnk24;
    /* 0x025 */ s8 tagTimeLimit;
    /* 0x026 */ s8 tagScoreLimit;
    /* 0x027 */ s8 tagArena;
    /* 0x028 */ s8 tagSkill;
    /* 0x029 */ u8 tagUnk29;
    /* 0x02A */ s8 teamTimeLimit;
    /* 0x02B */ s8 teamScoreLimit;
    /* 0x02C */ s8 teamArena;
    /* 0x02D */ s8 teamSkill;
    /* 0x02E */ u8 teamUnk2E;
    /* 0x02F */ u8 unk2F;
    /* 0x030 */ u8 weaponEnabled[10];  /* D_8011AB08 */
    /* 0x03A */ u8 unk3A[0x40 - 0x3A];
    /* 0x040 */ CPlayerOptions defaultSlot;  /* D_8011AB18 */
    /* 0x080 */ CPlayerOptions slots[4];     /* D_8011AB58 */
    /* 0x180 */ u8 unk180;
    /* 0x181 */ u8 unk181[0x1A8 - 0x181];
    /* 0x1A8 */ f32 modeTimer;         /* the mode's clock: a spin in degrees, a fade threshold (func_00288D3C, func_00289700) */
    /* 0x1AC */ s32 prevMode;
    /* 0x1B0 */ s32 mode;              /* D_8011AC88: the game mode (17: switching) */
    /* 0x1B4 */ s32 nextMode;          /* the mode to switch to (func_00288BC8, func_00288C5C) */
    /* 0x1B8 */ u8 fastFade;
    /* 0x1B9 */ u8 fadeMode;           /* 1 fading in, 2 fading out (switch mode) */
    /* 0x1BA */ u8 unk1BA[0x1BC - 0x1BA];
    /* 0x1BC */ f32 fade;
    /* 0x1C0 */ s32 ready;             /* func_00288284 */
    /* 0x1C4 */ s32 unk1C4;
    /* 0x1C8 */ s32 unk1C8;            /* set at a retrace while switching (func_00286674) */
    /* 0x1CC */ s32 paused;
    /* 0x1D0 */ u32 matchMode;         /* D_8011ACA8; also the warp the mode switch goes to (func_00430454) */
    /* 0x1D4 */ s32 warpFlags;
    /* 0x1D8 */ GameMatch match;       /* D_8011ACB0 */
} GameSettings; /* size 0x228 */

/* The engine, D_800F6CB0 (CEngineApp__Boot's object): GameSettings
 * D_8011AAD8 is inside it at 0x23E28, so its mode block (0x23FD0-0x23FFC)
 * is GameSettings 0x1A8-0x1D4. Only that part is described here. */
typedef struct CEngineApp {
    /* 0x00000 */ u8 pad0[0x23E28];
    /* 0x23E28 */ GameSettings m_Settings; /* D_8011AAD8 */
} CEngineApp; /* size 0x24050 */

extern GameSettings D_8011AAD8;
extern GameMatch D_8011ACB0;

/* ---- The game state, D_80119870 (CGame) ---- */

#include "structs/player_list.h"

/* The engine's globals at D_80119870: the players, the cameras and the
 * settings. Some parts have symbols of their own, which the ROM uses where
 * it reaches them through those: D_80119890 (m_PlayerList.m_ActiveList),
 * D_801198B0 (m_CameraPool), D_80119E2C (m_MenuPool) and D_8011AAD8
 * (m_Settings). The camera pool is CCameraPool (structs/camera.h), kept as
 * bytes here so that this header does not pull in the cameras and the game
 * objects; the menus' pool is the one of its main camera
 * (m_MainCamera.m_MenuPool), so it is written out. */
typedef struct CGame {
    /* 0x0000 */ CPlayerList m_PlayerList;
    /* 0x003C */ s32 _pad3C;
    /* 0x0040 */ u8 m_CameraPool[0x57C];   /* CCameraPool, D_801198B0 (0x11F0 bytes) */
    /* 0x05BC */ u8 m_MenuPool[0x18];      /* its m_MainCamera.m_MenuPool (MenuPool, structs/menu.h), D_80119E2C */
    /* 0x05D4 */ u8 m_CameraPoolEnd[0x1230 - 0x5D4];
    /* 0x1230 */ u8 unk1230[0x1268 - 0x1230];
    /* 0x1268 */ GameSettings m_Settings;  /* D_8011AAD8 */
} CGame; /* size 0x1490 */

extern CGame D_80119870;

#endif
