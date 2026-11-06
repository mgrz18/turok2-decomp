#ifndef STRUCTS_CINEMA_H
#define STRUCTS_CINEMA_H

/* The cinema (cutscene) state, *gCinemaPlayerP, and the entries of the cinema
 * table it plays (Cinema_SplineSecondDerivs's module, linked again at Cinema_SplineSecondDerivs_2).
 * Cinema__Initialize clears it; Cinema_Update / Cinema_AdvanceClock advance its clock;
 * Cinema_Start runs once at startTime (resets the player, warps); the end
 * and close callbacks run at endTime and clipEnd. Rage Wars has the same
 * block, longer, as Shared_MenuContext (include/rw/abu_types.h). */

#include "structs/vector.h"

struct CGameObjectInstance;

/* An entry of the cinema table (0x4C bytes; Rage Wars' EntryRecAt48). */
typedef struct CinemaEntry {
    /* 0x00 */ s32 id0;                /* the warp id it starts at (Cinema_SplineSecondDerivs) */
    /* 0x04 */ s32 id4;
    /* 0x08 */ s32 id;
    /* 0x0C */ u32 flags;              /* 1: follow the target, 2: follow the player, 8: save the start */
    /* 0x10 */ u8 pad10[0x18 - 0x10];
    /* 0x18 */ char name[0x38 - 0x18];
    /* 0x38 */ f32 scale;              /* Cinema_GetFarClip returns it * 10.24 */
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

/* The cinema state, *gCinemaPlayerP. */
typedef struct Cinema {
    /* 0x000 */ s32 **owner;           /* the loaded track (freed with CCache__Deallocate) */
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
    /* 0x030 */ f32 startTime;         /* Cinema_Start runs once the clock reaches it */
    /* 0x034 */ f32 endTime;
    /* 0x038 */ s32 active;
    /* 0x03C */ s32 finished;
    /* 0x040 */ s32 started;           /* Cinema_Start ran */
    /* 0x044 */ s32 ended;             /* onEnd ran */
    /* 0x048 */ s32 closed;
    /* 0x04C */ s32 restored;          /* Cinema_RestorePlayer put the player back */
    /* 0x050 */ s32 unk50;
    /* 0x054 */ s32 resetPlayer;       /* Cinema_Start resets the current player */
    /* 0x058 */ s32 resetPlayerNext;   /* the next cinema's resetPlayer */
    /* 0x05C */ s32 restoreAtEnd;      /* run Cinema_RestorePlayer at endTime */
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
    /* 0x0D8 */ s32 warpId;            /* the warp Cinema_Start goes to; -1: none */
    /* 0x0DC */ s32 entryIndex;        /* the entry in the cinema table; -1: none */
    /* 0x0E0 */ s32 scriptId;          /* the requested cinema (0x50...); -1: none */
    /* 0x0E4 */ CinemaCallback nextOnClose; /* Cinema_SetNextCallbacks queues them for scriptId; */
    /* 0x0E8 */ CinemaCallback nextOnEnd;   /* Cinema_Load moves them to onClose/onEnd */
    /* 0x0EC */ s32 unkEC;
    /* 0x0F0 */ CVector3 position;
    /* 0x0FC */ f32 yaw;
} Cinema; /* size 0x100 */

/* The cinema track's data (Cinema_SplineSecondDerivs's module): the camera
 * splines (eye and target), the cues it fires and the scalar keys (fade, FOV). */

/* A spline record: a point, the second derivatives of its coordinates over the
 * arc length, the arc length, the weight (time) and the second derivative of the
 * arc length over the weight. */
typedef struct {
    /* 0x00 */ CVector3 p;
    /* 0x0C */ CVector3 d2p;
    /* 0x18 */ f32 s;
    /* 0x1C */ f32 w;
    /* 0x20 */ f32 d2s;
} SplineNode; /* 0x24 bytes */

typedef struct {
    /* 0x00 */ s32 nodeSize;
    /* 0x04 */ s32 count;
    /* 0x08 */ SplineNode nodes[1];
} Spline;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ f32 time;
} Cue; /* 0x14 bytes */

typedef struct {
    /* 0x00 */ u8 pad0[0x4];
    /* 0x04 */ s32 count;
    /* 0x08 */ Cue cues[1];
} CueList;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ f32 value;
    /* 0x10 */ f32 time;
} ScalarKey; /* 0x14 bytes */

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
} CueRef;

typedef struct {
    f32 time;
    s32 unk4;
    s32 unk8;
} TimedCueRef;

#endif
