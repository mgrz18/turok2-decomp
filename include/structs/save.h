#ifndef STRUCTS_SAVE_H
#define STRUCTS_SAVE_H

/* The save game as it is written to and read from the controller pak: a
 * header with the play clock, the player's CPlayerLoadSaveData, the first
 * 0x184 bytes of the game settings, then the level flags (D_800EBAC0, as
 * many bytes as the level header says: D_800F70A8, CWorld +0x30), with a
 * checksum (func_00426E78) in its last word. func_0041D890 / func_004567B0
 * build one into D_800C2018; func_0041F32C reads one into D_800C201C and
 * func_00421528 / func_00420454 load the game from it. The pak menu items
 * show its clock (func_00422060), health, lives and difficulty.
 * tools/check_structs.py checks the offsets. */

#include "common.h"
#include "structs/player.h"

/* The first 0x184 bytes of GameSettings (structs/game.h), copied as one
 * block to and from D_8011AAD8. */
typedef struct SaveSettings {
    /* 0x000 */ u32 cheatFlags;
    /* 0x004 */ u32 unlockedCheats;
    /* 0x008 */ u32 difficulty;        /* 8, 16 or 32 */
    /* 0x00C */ s32 rest[(0x184 - 0xC) / 4];
} SaveSettings; /* size 0x184 */

typedef struct SaveGame {
    /* 0x000 */ s32 size;              /* the whole save, checksum included: levelFlagsSize + 0x688 */
    /* 0x004 */ s32 levelFlagsSize;    /* D_800F70A8 when written */
    /* 0x008 */ f32 frames;            /* the play clock (GameMatch, D_8011ACB0) */
    /* 0x00C */ f32 seconds;
    /* 0x010 */ f32 minutes;
    /* 0x014 */ f32 hours;
    /* 0x018 */ CPlayerLoadSaveData player;
    /* 0x500 */ SaveSettings settings;
    /* 0x684 */ u8 levelFlags[4];      /* levelFlagsSize bytes, then the checksum */
} SaveGame; /* size 0x688 + the level flags */

extern SaveGame *D_800C2018;    /* the save being written */
extern SaveGame *D_800C201C;    /* the save read from the pak */

#endif
