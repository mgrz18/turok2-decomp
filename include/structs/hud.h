#ifndef STRUCTS_HUD_H
#define STRUCTS_HUD_H

/* Hud and overlay views (code/Hud). */

#include "common.h"

/* A table of frames: a count, then for each an offset from the table's start
 * and a byte. */
typedef struct {
    /* 0x0 */ s32 m_Offset;
    /* 0x4 */ u8 m_Byte;
} FrameEntry; /* size 0x8 */

typedef struct {
    /* 0x0 */ s16 m_nFrames;
    /* 0x2 */ u8 _pad[6];
    /* 0x8 */ FrameEntry m_Frames[1];
} FrameTable;

/* The state COverlay__StartScript sets up: a value and a mode, and two more words. */
typedef struct ModeState {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 value;
    /* 0x08 */ s32 mode;
    /* 0x0C */ u8 padC[0x40 - 0xC];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ s32 unk44;
} ModeState;

#endif
