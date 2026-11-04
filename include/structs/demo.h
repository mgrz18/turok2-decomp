#ifndef STRUCTS_DEMO_H
#define STRUCTS_DEMO_H

/* The attract-mode demo player, D_800F56B0. A demo (DemoHeader, from the
 * zero-terminated list D_004384E8, played in turn by func_0042F590) is a
 * bit stream per input: for each player the buttons (decoded by
 * func_0042ECF0), the stick x and y (func_0042F75C), plus one stream for the
 * whole match (func_0042F7C8, copied to D_8011AD10 each frame). Each value
 * lasts a run of frames read after it (func_0042EC30). func_0042EF2C starts
 * a demo (saving the game settings first), func_0042F344 feeds the decoded
 * values to the controllers (D_800F5F70) and keeps a checksum
 * (func_0042F868), func_0042F514 puts the saved settings back.
 * tools/check_structs.py checks the offsets. */

#include "common.h"

/* The header of a demo: the match it plays and where each stream starts. */
typedef struct DemoHeader {
    /* 0x00 */ u16 matchMode;           /* GameSettings.matchMode */
    /* 0x02 */ s16 playerCount;
    /* 0x04 */ s16 frames;              /* how long it plays */
    /* 0x06 */ s16 matchStart;          /* bit offset of the match stream */
    /* 0x08 */ s16 characters[4];       /* the character is the low byte */
    /* 0x10 */ s16 buttonsStart[4];     /* bit offsets of the streams */
    /* 0x18 */ s16 stickXStart[4];
    /* 0x20 */ s16 stickYStart[4];
} DemoHeader; /* size 0x28 */

/* One input stream: the value it holds now and for how many more frames,
 * read from the demo's bits by its decode callback. */
typedef struct DemoStream {
    /* 0x00 */ s32 value;
    /* 0x04 */ u8 *bits;                /* the demo (DemoHeader); pos counts from it */
    /* 0x08 */ u32 pos;                 /* in bits */
    /* 0x0C */ s32 frames;              /* frames left before the next value is read */
    /* 0x10 */ s32 player;
    /* 0x14 */ void (*decode)(struct DemoStream *stream);
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
} DemoStream; /* size 0x20 */

/* The first 0x184 bytes of GameSettings (structs/game.h), as one block. */
typedef struct DemoSettings {
    /* 0x000 */ s32 words[0x184 / 4];
} DemoSettings; /* size 0x184 */

typedef struct DemoPlayer {
    /* 0x000 */ s32 active;             /* the attract mode plays demos (func_0042F57C) */
    /* 0x004 */ s32 unk4;
    /* 0x008 */ s32 playing;            /* a demo is running (func_0042EF2C) */
    /* 0x00C */ s32 fastForward;        /* read every stream each frame */
    /* 0x010 */ s32 done;
    /* 0x014 */ DemoStream buttons[4];
    /* 0x094 */ DemoStream stickX[4];
    /* 0x114 */ DemoStream stickY[4];
    /* 0x194 */ DemoStream match;       /* D_800F5844 */
    /* 0x1B4 */ s32 playerCount;
    /* 0x1B8 */ s32 unk1B8;
    /* 0x1BC */ s32 countdown;          /* frames left */
    /* 0x1C0 */ f32 time;
    /* 0x1C4 */ s32 checksum;
    /* 0x1C8 */ s32 saved;              /* settings holds the game settings */
    /* 0x1CC */ DemoSettings settings;
    /* 0x350 */ s32 index;              /* the next demo of the list */
} DemoPlayer; /* size 0x354 */

extern DemoPlayer D_800F56B0;

#endif
