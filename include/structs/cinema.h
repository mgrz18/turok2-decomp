#ifndef STRUCTS_CINEMA_H
#define STRUCTS_CINEMA_H

/* The cinema (cutscene) state, *D_800C1BB0, and the entries of the cinema
 * table it plays (func_0040F240's module, linked again at func_00448240).
 * func_002837E0 clears it; func_00283CD4 / func_00284568 advance its clock;
 * func_002838A4 runs once at startTime (resets the player, warps); the end
 * and close callbacks run at endTime and clipEnd. Rage Wars has the same
 * block, longer, as Shared_MenuContext (include/rw/abu_types.h). */

#include "structs/vector.h"

struct CGameObjectInstance;

/* An entry of the cinema table (0x4C bytes; Rage Wars' EntryRecAt48). */
typedef struct CinemaEntry {
    /* 0x00 */ s32 id0;                /* the warp id it starts at (func_0040F240) */
    /* 0x04 */ s32 id4;
    /* 0x08 */ s32 id;
    /* 0x0C */ u32 flags;              /* 1: follow the target, 2: follow the player, 8: save the start */
    /* 0x10 */ u8 pad10[0x18 - 0x10];
    /* 0x18 */ char name[0x38 - 0x18];
    /* 0x38 */ f32 scale;              /* func_002843D4 returns it * 10.24 */
    /* 0x3C */ f32 aspect;
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
    /* 0x48 */ s32 unk48;
} CinemaEntry; /* size 0x4C */

typedef struct CinemaTable {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ CinemaEntry entries[1];
} CinemaTable;

typedef void (*CinemaCallback)();

/* The cinema state, *D_800C1BB0. */
typedef struct Cinema {
    /* 0x000 */ s32 **owner;           /* the loaded track (freed with func_002052D8) */
    /* 0x004 */ void *resource;
    /* 0x008 */ s32 unk8;
    /* 0x00C */ CinemaCallback onClose; /* run at clipEnd */
    /* 0x010 */ CinemaCallback onEnd;   /* run at endTime */
    /* 0x014 */ s32 unk14;
    /* 0x018 */ struct CGameObjectInstance *target;
    /* 0x01C */ f32 time;              /* in frames (15 a second) */
    /* 0x020 */ f32 prevTime;
    /* 0x024 */ f32 time24;
    /* 0x028 */ f32 clipStart;
    /* 0x02C */ f32 clipEnd;
    /* 0x030 */ f32 startTime;         /* func_002838A4 runs once the clock reaches it */
    /* 0x034 */ f32 endTime;
    /* 0x038 */ s32 active;
    /* 0x03C */ s32 finished;
    /* 0x040 */ s32 started;           /* func_002838A4 ran */
    /* 0x044 */ s32 ended;             /* onEnd ran */
    /* 0x048 */ s32 closed;
    /* 0x04C */ s32 restored;          /* func_00283A4C put the player back */
    /* 0x050 */ s32 unk50;
    /* 0x054 */ s32 resetPlayer;       /* func_002838A4 resets the current player */
    /* 0x058 */ s32 resetPlayerNext;   /* the next cinema's resetPlayer */
    /* 0x05C */ s32 restoreAtEnd;      /* run func_00283A4C at endTime */
    /* 0x060 */ s32 fading;
    /* 0x064 */ f32 fade;
    /* 0x068 */ CinemaEntry spec;      /* the playing entry */
    /* 0x0B4 */ s32 stopAtEnd;         /* at clipEnd: finished = 1 */
    /* 0x0B8 */ s32 saveStart;         /* the entry's flag 8: remember where the player starts */
    /* 0x0BC */ s32 startLevel;        /* -1: none */
    /* 0x0C0 */ s32 startWarp;         /* -1: none */
    /* 0x0C4 */ CVector3 startPos;
    /* 0x0D0 */ f32 startRotY;
    /* 0x0D4 */ s32 startRegion;
    /* 0x0D8 */ s32 warpId;            /* the warp func_002838A4 goes to; -1: none */
    /* 0x0DC */ s32 entryIndex;        /* the entry in the cinema table; -1: none */
    /* 0x0E0 */ s32 scriptId;          /* the requested cinema (0x50...); -1: none */
    /* 0x0E4 */ CinemaCallback nextOnClose; /* func_00283FF8 queues them for scriptId; */
    /* 0x0E8 */ CinemaCallback nextOnEnd;   /* func_0040FD6C moves them to onClose/onEnd */
    /* 0x0EC */ s32 unkEC;
    /* 0x0F0 */ CVector3 position;
    /* 0x0FC */ f32 yaw;
} Cinema; /* size 0x100 */

#endif
