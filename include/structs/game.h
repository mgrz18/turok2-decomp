#ifndef STRUCTS_GAME_H
#define STRUCTS_GAME_H

/* The game settings block, gEngineApp_m_Settings (the game state at gEngineApp_m_Game +
 * 0x1268), and the match state inside it, gEngineApp_m_Settings_match (+ 0x1D8).
 *
 * The first 0x184 bytes (GameSettingsBlock) are what the save and the demo
 * player copy (SaveGame_Build, Demo_End). Rage Wars' Shared_Game (D_801462C8, at the
 * same place in its game state) is the same block with 4 more bytes before
 * 0x08, so its `local` at 0x1D is `multiplayer` here at 0x19.
 *
 * Several fields also have symbols of their own, which some files still
 * use: gEngineApp_m_Settings_difficulty (difficulty), gEngineApp_m_Settings_multiplayer (multiplayer), gEngineApp_m_Settings_weaponEnabled
 * (weaponEnabled), gEngineApp_m_Settings_defaultSlot (defaultSlot), gEngineApp_m_Settings_slots (slots), gEngineApp_m_Settings_mode
 * (mode), gEngineApp_m_Settings_matchMode (matchMode) and gEngineApp_m_Settings_match (match). Files that read
 * a field through one of those symbols now name it from gEngineApp_m_Settings (the
 * offsets resolve to the same addresses); Hud_GetSpriteValue keeps a view based at
 * gEngineApp_m_Settings_multiplayer, as the ROM bases its loads there. Three files keep a
 * local array view of gEngineApp_m_Settings / gEngineApp_m_Settings_match (CScene__ComputeLights, CEngineApp__Construct,
 * FinalScoreMenu_Poll_2): through an array the compiler bases the accesses on the
 * accessed field's address, as the ROM does there. */

#include "structs/vector.h"
#include "structs/player_options.h"
#include "structs/frame.h"

/* The match state, gEngineApp_m_Settings_match: the play clock, then the rules of the
 * running match (Rage Wars' RenderCounters / GameRules at D_801468A0). */
typedef struct GameMatch {
    /* 0x00 */ f32 frames;             /* the play clock (CEngineApp__UpdatePlayTime) */
    /* 0x04 */ f32 seconds;
    /* 0x08 */ f32 minutes;
    /* 0x0C */ f32 hours;
    /* 0x10 */ f32 countdown;
    /* 0x14 */ f32 timeLeft;           /* minutes * 900; -1: no limit (Mp_SetupMatchRules) */
    /* 0x18 */ s32 scoreLimit;
    /* 0x1C */ s32 over;
    /* 0x20 */ s32 done;
    /* 0x24 */ s32 teams;              /* a team match */
    /* 0x28 */ s32 teamOver;
    /* 0x2C */ s32 teamScore[2];       /* CPlayerPool__AddTeamScore */
    /* 0x34 */ s32 tag;                /* a tag match */
    /* 0x38 */ s32 tagPick;            /* who is "it" (CPlayerPool__PickTagIt) */
    /* 0x3C */ s32 tagPickStart;
    /* 0x40 */ s32 tagPhase;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
    /* 0x4C */ s32 unk4C;
} GameMatch; /* size 0x50 */

/* The sun (Turok 3's CSun, same size; Light_SetDefaultDirection is CSun__Construct). */
typedef struct CSun {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ f32 m_ScreenX;
    /* 0x0C */ f32 m_ScreenY;
    /* 0x10 */ f32 m_Opacity;
    /* 0x14 */ CVector3 m_vDir;
    /* 0x20 */ s32 m_bVisible;
} CSun; /* size 0x24 */

/* The game settings, gEngineApp_m_Settings. The options at 0x20 come in three groups
 * of five, one per kind of match (MpMenu_StartBloodLust, MpMenu_StartFragTag and
 * MpMenu_StartTeamBlood start them): deathmatch, tag and team. */
/* The first 0x184 bytes of GameSettings: the block the save (SaveGame,
 * structs/save.h) and the demo player (DemoPlayer, structs/demo.h) copy to and
 * from gEngineApp_m_Settings in one go, as *(GameSettingsBlock *)&gEngineApp_m_Settings. */
typedef struct GameSettingsBlock {
    /* 0x000 */ u32 cheatFlags;
    /* 0x004 */ u32 unlockedCheats;
    /* 0x008 */ u32 difficulty;        /* 8, 16 or 32 */
    /* 0x00C */ s32 rest[(0x184 - 0xC) / 4];
} GameSettingsBlock; /* size 0x184 */

typedef struct GameSettings {
    /* 0x000 */ u32 cheatFlags;        /* the cheats on: 0x4000 black fog, 0x800/0x1000 big heads... */
    /* 0x004 */ u32 unlockedCheats;    /* set by the cheat codes (EnterCheatMenu_Enter) and bosses (TotemDefense_Complete) */
    /* 0x008 */ s32 difficulty;         /* gEngineApp_m_Settings_difficulty: 8, 16 or 32 (AI_InitHealth) */
    /* 0x00C */ u32 unkC;              /* shown in sixteenths */
    /* 0x010 */ union {
        u16 value;                 /* 0 to 0x100 in sixteenths (OptionsMenu_StepOpacity, OptionsMenu_OpacityText) */
        u8 bytes[2];
    } hudAlphaSetting;             /* gEngineApp_m_Settings_hudAlphaSetting */
    /* 0x012 */ u8 hudAlpha;          /* gEngineApp_m_Settings_hudAlpha: hudAlphaSetting clamped to 0xFF, what the HUD draws with */
    /* 0x013 */ u8 unk13;
    /* 0x014 */ u8 unk14;              /* 0x80-centred */
    /* 0x015 */ u8 unk15;              /* 0x80-centred */
    /* 0x016 */ u8 unk16;              /* 0x80-centred */
    /* 0x017 */ u8 unk17;
    /* 0x018 */ u8 unk18;
    /* 0x019 */ u8 multiplayer;        /* gEngineApp_m_Settings_multiplayer */
    /* 0x01A */ u8 playerCount;
    /* 0x01B */ u8 unk1B;
    /* 0x01C */ f32 speed;             /* game speed, 0.5 to 2 (MpMenu_StepPlayerSpeed_2) */
    /* 0x020 */ s8 dmTimeLimit;        /* minutes */
    /* 0x021 */ s8 dmScoreLimit;
    /* 0x022 */ s8 dmArena;            /* 0: random */
    /* 0x023 */ s8 dmTextureSet;       /* 'texture set:' 0..8 (BloodLustMenu_StepTextureSet); AI_InitHealth uses it for m_nTexture */
    /* 0x024 */ u8 dmCrosshairs;       /* 'crosshairs : on/off' (BloodLustMenu_StepCrosshairs) */
    /* 0x025 */ s8 tagTimeLimit;
    /* 0x026 */ s8 tagScoreLimit;
    /* 0x027 */ s8 tagArena;
    /* 0x028 */ s8 tagTextureSet;      /* FragTagMenu_StepTextureSet */
    /* 0x029 */ u8 tagCrosshairs;      /* FragTagMenu_StepCrosshairs */
    /* 0x02A */ s8 teamTimeLimit;
    /* 0x02B */ s8 teamScoreLimit;
    /* 0x02C */ s8 teamArena;
    /* 0x02D */ s8 teamTextureSet;     /* TeamBloodMenu_StepTextureSet */
    /* 0x02E */ u8 teamCrosshairs;     /* TeamBloodMenu_StepCrosshairs */
    /* 0x02F */ u8 unk2F;
    /* 0x030 */ u8 weaponEnabled[10];  /* gEngineApp_m_Settings_weaponEnabled */
    /* 0x03A */ u8 unk3A[0x3C - 0x3A];
    /* 0x03C */ u8 unk3C;
    /* 0x03D */ u8 unk3D;              /* 3 by default (GameSettings_SetDefaults) */
    /* 0x03E */ u8 unk3E;
    /* 0x03F */ u8 unk3F;              /* 3 by default */
    /* 0x040 */ CPlayerOptions defaultSlot;  /* gEngineApp_m_Settings_defaultSlot */
    /* 0x080 */ CPlayerOptions slots[4];     /* gEngineApp_m_Settings_slots */
    /* 0x180 */ u8 unk180;
    /* 0x181 */ u8 unk181[0x184 - 0x181];
    /* 0x184 */ CSun m_Sun;            /* CSun__Construct: Light_SetDefaultDirection */
    /* 0x1A8 */ f32 modeTimer;         /* the mode's clock: a spin in degrees, a fade threshold (CEngineApp__TimedModeSwitch, EngineMode08_MainMenu_Retrace) */
    /* 0x1AC */ s32 prevMode;
    /* 0x1B0 */ s32 mode;              /* gEngineApp_m_Settings_mode: the game mode (17: switching) */
    /* 0x1B4 */ s32 nextMode;          /* the mode to switch to (CEngineApp__SetMode, CEngineApp__SetupFadeTo) */
    /* 0x1B8 */ u8 fastFade;
    /* 0x1B9 */ u8 fadeMode;           /* 1 fading in, 2 fading out (switch mode) */
    /* 0x1BA */ u8 unk1BA[0x1BC - 0x1BA];
    /* 0x1BC */ f32 fade;
    /* 0x1C0 */ s32 ready;             /* CEngineApp__Idle */
    /* 0x1C4 */ s32 unk1C4;
    /* 0x1C8 */ s32 unk1C8;            /* set at a retrace while switching (CEngineApp__Main) */
    /* 0x1CC */ s32 paused;
    /* 0x1D0 */ u32 matchMode;         /* gEngineApp_m_Settings_matchMode; also the warp the mode switch goes to (func_00430454) */
    /* 0x1D4 */ s32 warpFlags;
    /* 0x1D8 */ GameMatch match;       /* gEngineApp_m_Settings_match */
} GameSettings; /* size 0x228 */

/* ---- The game state, gEngineApp_m_Game (CGame) ---- */

#include "structs/player_list.h"

/* The automap's state at CGame+0x1230 (Automap_Update, Automap_Draw and
 * Automap_IsShown take &m_Automap): mode, state, blend, then the yaw and
 * pitch words from 0x20. func_00430500 clears the counts and fills the
 * coloured vertex buffers sAutomapLineVtx, sAutomapMagentaVtx, sAutomapRedVtx, sAutomapYellowVtx.
 * Named CAutomap / m_Automap (Turok 3 and Rage Wars give no name); it was
 * CDebugLines / m_DebugLines before the naming rounds showed what it is. */
typedef struct CAutomap {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 m_Counts[11];
#if defined(TARGET_PORT) && !defined(__mips__)
    /* The port: the player's region pointer-wide (Automap_SetPlayer; on the N64 the word at 0xC,
     * between fields that leave no room for a host pointer). */
    struct CGameRegion *m_pHostRegion;
#endif
} CAutomap; /* size 0x38 */

/* The engine's globals at gEngineApp_m_Game: the players, the cameras and the
 * settings. Some parts have symbols of their own, which the ROM uses where
 * it reaches them through those: gEngineApp_m_Game_m_PlayerList_m_ActiveList_m_pHead (m_PlayerList.m_ActiveList),
 * gEngineApp_m_Game_m_CameraPool (m_CameraPool), gEngineApp_m_Game_m_MenuPool (m_MenuPool) and gEngineApp_m_Settings
 * (m_Settings). The camera pool is CCameraPool (structs/camera.h), kept as
 * bytes here so that this header does not pull in the cameras and the game
 * objects; the menus' pool is the one of its main camera
 * (m_MainCamera.m_MenuPool), so it is written out. */
#ifdef TARGET_PORT
/* The port: the parts CGame and CEngineApp embed are their real types, so that each is laid out
 * at its host size and gendata's aliases (gEngineApp_m_Game_m_CameraPool, gScene_*, ...) land on
 * the fields (docs/port/RUNTIME.md). The N64 offsets are the same as the byte views below
 * (asserted for the mips layout at the end of this file). A one-element array keeps each
 * `x.m_CameraPool` / `x.m_Scene` expression a pointer, as the byte arrays are. */
#include "structs/camera.h"
#include "structs/world.h"
#include "structs/player.h"
#ifdef TARGET_PORT
#include "structs/swoosh.h"
#endif
#endif

typedef struct CGame {
    /* 0x0000 */ CPlayerPool m_PlayerList;
    /* 0x003C */ s32 _pad3C;
#ifdef TARGET_PORT
    union {
        CCameraPool m_CameraPool[1];        /* gEngineApp_m_Game_m_CameraPool */
        struct {
            u8 _padMenuPool[__builtin_offsetof(CCameraPool, m_MainCamera.m_MenuPool)];
            u8 m_MenuPool[0x18];            /* gEngineApp_m_Game_m_MenuPool */
        };
    };
#else
    /* 0x0040 */ u8 m_CameraPool[0x57C];   /* CCameraPool, gEngineApp_m_Game_m_CameraPool (0x11F0 bytes) */
    /* 0x05BC */ u8 m_MenuPool[0x18];      /* its m_MainCamera.m_MenuPool (MenuPool, structs/menu.h), gEngineApp_m_Game_m_MenuPool */
    /* 0x05D4 */ u8 m_CameraPoolEnd[0x1230 - 0x5D4];
#endif
    /* 0x1230 */ CAutomap m_Automap;   /* the automap (gEngineApp_m_Game_m_DebugLines); reset by func_00430500 */
    /* 0x1268 */ GameSettings m_Settings;  /* gEngineApp_m_Settings */
} CGame; /* size 0x1490 */

/* CGame seen from its camera pool, gEngineApp_m_Game_m_CameraPool (CGame + 0x40): some files
 * read the settings through that symbol, as the ROM does there (through
 * gEngineApp_m_Settings their code no longer matches); they declare gEngineApp_m_Game_m_CameraPool so. The
 * camera pool files declare it CCameraPool (structs/camera.h). */
typedef struct CGameFromCameraPool {
#ifdef TARGET_PORT
    CCameraPool m_CameraPool[1];
#else
    /* 0x0000 */ u8 m_CameraPool[0x11F0];   /* CCameraPool: m_MainCamera at 0x40 */
#endif
    /* 0x11F0 */ CAutomap m_Automap;
    /* 0x1228 */ GameSettings m_Settings;   /* gEngineApp_m_Settings */
} CGameFromCameraPool; /* size 0x1450 */

/* ---- The engine, gEngineApp (CEngineApp) ---- */

/* CFrameData (one frame in flight, gEngineApp_m_pCurrentFrameData) is in structs/frame.h. */

/* The engine, gEngineApp (CEngineApp__Boot's object; the field names after
 * Turok 3's CEngineApp, which has the same parts in the same order): the
 * frames, the world (CScene gScene, structs/world.h, kept as bytes so
 * that this header does not pull in the world), then CGame gEngineApp_m_Game, whose
 * settings (gEngineApp_m_Settings, also reachable as m_Settings) end with the engine's
 * mode block (0x23FD0-0x23FFC is GameSettings 0x1A8-0x1D4: Turok 3's
 * m_ModeTime, m_LastMode, m_Mode...). */
typedef struct CEngineApp {
    /* 0x00000 */ CFrameData m_FrameData[3];
    /* 0x003C0 */ CFrameData *m_pCurrentFrameData;   /* gEngineApp_m_pCurrentFrameData */
    /* 0x003C4 */ s32 m_CartridgeIndex;              /* the ROM address of the cartridge index (0xFCBFB4: CEngineApp__Main, CScene__Construct) */
#ifdef TARGET_PORT
    CScene m_Scene[1];                               /* gScene */
    COnScreen unk19440[1];                           /* gEngineApp_m_OnScreen */
    union {
        CSwooshSystem m_SwooshSystem[1];             /* gEngineApp_m_SwooshSystem (structs/swoosh.h) */
        u8 _SwooshBytes[0x22BC0 - 0x19608];          /* the N64's 0x9598 bytes and unk22BA0 (D_80119850) */
    };
    union {
        CGame m_Game;                                /* gEngineApp_m_Game */
        struct {
            u8 _pad22BC0[__builtin_offsetof(CGame, m_Settings)];
            GameSettings m_Settings;                 /* gEngineApp_m_Settings */
        };
    };
#else
    /* 0x003C8 */ u8 m_Scene[0x19440 - 0x3C8];       /* CScene gScene (0x19078 bytes) */
    /* 0x19440 */ u8 unk19440[0x22BC0 - 0x19440];
    union {
        /* 0x22BC0 */ CGame m_Game;                  /* gEngineApp_m_Game */
        struct {
            u8 _pad22BC0[0x23E28 - 0x22BC0];
            /* 0x23E28 */ GameSettings m_Settings;   /* gEngineApp_m_Settings, m_Game.m_Settings */
        };
    };
#endif
} CEngineApp; /* size 0x24050 */

#if defined(TARGET_PORT) && defined(__mips__)
/* gendata parses the port's headers for mips too: the typed members keep the N64 offsets. */
_Static_assert(__builtin_offsetof(CGame, m_CameraPool) == 0x40, "CGame.m_CameraPool");
_Static_assert(__builtin_offsetof(CGame, m_MenuPool) == 0x5BC, "CGame.m_MenuPool");
_Static_assert(__builtin_offsetof(CGame, m_Automap) == 0x1230, "CGame.m_Automap");
_Static_assert(sizeof(CGame) == 0x1490, "CGame");
_Static_assert(__builtin_offsetof(CGameFromCameraPool, m_Settings) == 0x1228, "CGameFromCameraPool");
_Static_assert(__builtin_offsetof(CEngineApp, m_Scene) == 0x3C8, "CEngineApp.m_Scene");
_Static_assert(__builtin_offsetof(CEngineApp, unk19440) == 0x19440, "CEngineApp.m_OnScreen");
_Static_assert(__builtin_offsetof(CEngineApp, m_SwooshSystem) == 0x19608, "CEngineApp.m_SwooshSystem");
_Static_assert(__builtin_offsetof(CEngineApp, m_Game) == 0x22BC0, "CEngineApp.m_Game");
_Static_assert(__builtin_offsetof(CEngineApp, m_Settings) == 0x23E28, "CEngineApp.m_Settings");
_Static_assert(sizeof(CEngineApp) == 0x24050, "CEngineApp");
#endif

extern CEngineApp gEngineApp;

#endif
