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

/* One player's multiplayer options (0x40 bytes): defaultSlot and slots[]
 * below; CPlayer::m_pOptions points at one of them. */
typedef struct GameSlot {
    /* 0x00 */ u8 unk0[0x28];
    /* 0x28 */ u8 active;
    /* 0x29 */ u8 unk29;               /* 0x80-centred */
    /* 0x2A */ u8 unk2A;               /* 0x80-centred */
    /* 0x2B */ u8 unk2B[0x2F - 0x2B];
    /* 0x2F */ s8 controller;
    /* 0x30 */ s8 character;           /* func_0042DC18; the team in func_0042EF2C */
    /* 0x31 */ u8 unk31[0x40 - 0x31];
} GameSlot; /* size 0x40 */

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
    /* 0x040 */ GameSlot defaultSlot;  /* D_8011AB18 */
    /* 0x080 */ GameSlot slots[4];     /* D_8011AB58 */
    /* 0x180 */ u8 unk180;
    /* 0x181 */ u8 unk181[0x1B0 - 0x181];
    /* 0x1B0 */ s32 mode;              /* D_8011AC88: the game mode */
    /* 0x1B4 */ u8 unk1B4[0x1CC - 0x1B4];
    /* 0x1CC */ s32 unk1CC;
    /* 0x1D0 */ u32 matchMode;         /* D_8011ACA8 */
    /* 0x1D4 */ u8 unk1D4[0x1D8 - 0x1D4];
    /* 0x1D8 */ GameMatch match;       /* D_8011ACB0 */
} GameSettings; /* size 0x228 */

extern GameSettings D_8011AAD8;
extern GameMatch D_8011ACB0;

#endif
