#ifndef STRUCTS_GAME_H
#define STRUCTS_GAME_H

/* The game settings block, D_8011AAD8 (the game state at D_80119870 +
 * 0x1268), and the match state inside it, D_8011ACB0 (+ 0x1D8).
 *
 * The first 0x184 bytes (GameSettingsBlock) are what the save and the demo
 * player copy (func_0041D890, func_0042F514). Rage Wars' Shared_Game (D_801462C8, at the
 * same place in its game state) is the same block with 4 more bytes before
 * 0x08, so its `local` at 0x1D is `multiplayer` here at 0x19.
 *
 * Several fields also have symbols of their own, which some files still
 * use: D_8011AAE0 (difficulty), D_8011AAF1 (multiplayer), D_8011AB08
 * (weaponEnabled), D_8011AB18 (defaultSlot), D_8011AB58 (slots), D_8011AC88
 * (mode), D_8011ACA8 (matchMode) and D_8011ACB0 (match). Files that read
 * a field through one of those symbols now name it from D_8011AAD8 (the
 * offsets resolve to the same addresses); func_00256E6C keeps a view based at
 * D_8011AAF1, as the ROM bases its loads there. Three files keep a
 * local array view of D_8011AAD8 / D_8011ACB0 (func_00222A9C, func_00286440,
 * func_00450D78): through an array the compiler bases the accesses on the
 * accessed field's address, as the ROM does there. */

#include "structs/vector.h"
#include "structs/player_options.h"
#include "structs/frame.h"

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

/* The sun (Turok 3's CSun, same size; func_0042D1C0 is CSun__Construct). */
typedef struct CSun {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ f32 m_ScreenX;
    /* 0x0C */ f32 m_ScreenY;
    /* 0x10 */ f32 m_Opacity;
    /* 0x14 */ CVector3 m_vDir;
    /* 0x20 */ s32 m_bVisible;
} CSun; /* size 0x24 */

/* The game settings, D_8011AAD8. The options at 0x20 come in three groups
 * of five, one per kind of match (func_0041760C, func_00417490 and
 * func_0041778C start them): deathmatch, tag and team. */
/* The first 0x184 bytes of GameSettings: the block the save (SaveGame,
 * structs/save.h) and the demo player (DemoPlayer, structs/demo.h) copy to and
 * from D_8011AAD8 in one go, as *(GameSettingsBlock *)&D_8011AAD8. */
typedef struct GameSettingsBlock {
    /* 0x000 */ u32 cheatFlags;
    /* 0x004 */ u32 unlockedCheats;
    /* 0x008 */ u32 difficulty;        /* 8, 16 or 32 */
    /* 0x00C */ s32 rest[(0x184 - 0xC) / 4];
} GameSettingsBlock; /* size 0x184 */

typedef struct GameSettings {
    /* 0x000 */ u32 cheatFlags;        /* the cheats on: 0x4000 black fog, 0x800/0x1000 big heads... */
    /* 0x004 */ u32 unlockedCheats;    /* set by the cheat codes (func_0041AD64) and bosses (func_00407F20) */
    /* 0x008 */ s32 difficulty;         /* D_8011AAE0: 8, 16 or 32 (func_002427E0) */
    /* 0x00C */ u32 unkC;              /* shown in sixteenths */
    /* 0x010 */ union {
        u16 value;                 /* 0 to 0x100 in sixteenths (func_0041C458, func_0041C3C4) */
        u8 bytes[2];
    } hudAlphaSetting;             /* D_8011AAE8 */
    /* 0x012 */ u8 hudAlpha;          /* D_8011AAEA: hudAlphaSetting clamped to 0xFF, what the HUD draws with */
    /* 0x013 */ u8 unk13;
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
    /* 0x03A */ u8 unk3A[0x3C - 0x3A];
    /* 0x03C */ u8 unk3C;
    /* 0x03D */ u8 unk3D;              /* 3 by default (func_0041BA50) */
    /* 0x03E */ u8 unk3E;
    /* 0x03F */ u8 unk3F;              /* 3 by default */
    /* 0x040 */ CPlayerOptions defaultSlot;  /* D_8011AB18 */
    /* 0x080 */ CPlayerOptions slots[4];     /* D_8011AB58 */
    /* 0x180 */ u8 unk180;
    /* 0x181 */ u8 unk181[0x184 - 0x181];
    /* 0x184 */ CSun m_Sun;            /* CSun__Construct: func_0042D1C0 */
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

extern GameSettings D_8011AAD8;
extern GameMatch D_8011ACB0;

/* ---- The game state, D_80119870 (CGame) ---- */

#include "structs/player_list.h"

/* The debug line counts (func_00430500 resets them and fills the coloured
 * vertex buffers D_80129A40, D_8012DA40, D_8012DB40, D_8012F340). */
typedef struct CDebugLines {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 m_Counts[11];
} CDebugLines; /* size 0x38 */

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
    /* 0x1230 */ CDebugLines m_DebugLines;   /* reset by func_00430500 */
    /* 0x1268 */ GameSettings m_Settings;  /* D_8011AAD8 */
} CGame; /* size 0x1490 */

extern CGame D_80119870;

/* CGame seen from its camera pool, D_801198B0 (CGame + 0x40): some files
 * read the settings through that symbol, as the ROM does there (through
 * D_8011AAD8 their code no longer matches); they declare D_801198B0 so. The
 * camera pool files declare it CCameraPool (structs/camera.h). */
typedef struct CGameFromCameraPool {
    /* 0x0000 */ u8 m_CameraPool[0x11F0];   /* CCameraPool: m_MainCamera at 0x40 */
    /* 0x11F0 */ CDebugLines m_DebugLines;
    /* 0x1228 */ GameSettings m_Settings;   /* D_8011AAD8 */
} CGameFromCameraPool; /* size 0x1450 */

/* ---- The engine, D_800F6CB0 (CEngineApp) ---- */

/* CFrameData (one frame in flight, D_800F7070) is in structs/frame.h. */

/* The engine, D_800F6CB0 (CEngineApp__Boot's object; the field names after
 * Turok 3's CEngineApp, which has the same parts in the same order): the
 * frames, the world (CWorld D_800F7078, structs/world.h, kept as bytes so
 * that this header does not pull in the world), then CGame D_80119870, whose
 * settings (D_8011AAD8, also reachable as m_Settings) end with the engine's
 * mode block (0x23FD0-0x23FFC is GameSettings 0x1A8-0x1D4: Turok 3's
 * m_ModeTime, m_LastMode, m_Mode...). */
typedef struct CEngineApp {
    /* 0x00000 */ CFrameData m_FrameData[3];
    /* 0x003C0 */ CFrameData *m_pCurrentFrameData;   /* D_800F7070 */
    /* 0x003C4 */ s32 m_CartridgeIndex;              /* the ROM address of the cartridge index (0xFCBFB4: func_00286674, func_00429FA0) */
    /* 0x003C8 */ u8 m_Scene[0x19440 - 0x3C8];       /* CWorld D_800F7078 (0x19078 bytes) */
    /* 0x19440 */ u8 unk19440[0x22BC0 - 0x19440];
    union {
        /* 0x22BC0 */ CGame m_Game;                  /* D_80119870 */
        struct {
            u8 _pad22BC0[0x23E28 - 0x22BC0];
            /* 0x23E28 */ GameSettings m_Settings;   /* D_8011AAD8, m_Game.m_Settings */
        };
    };
} CEngineApp; /* size 0x24050 */

extern CEngineApp D_800F6CB0;

#endif
