#ifndef STRUCTS_FRAME_H
#define STRUCTS_FRAME_H

/* One frame in flight (Turok 3's CFrameData, references/turok3/include/
 * tengine.h; 8 bytes longer here: the word at 0x128). The engine keeps three
 * in a ring through m_pPrev / m_pNext (CEngineApp.m_FrameData, structs/
 * game.h; set up by func_00286440). D_800F7070 (CEngineApp.m_pCurrentFrameData)
 * is the frame being built; func_00287ABC copies it to D_8011F140, the frame
 * func_00287248 finishes and sends (m_Msg) to the RSP task queue D_8011AD28.
 * The display list starts at m_pDisplayList and the engine writes it through
 * D_800D8E24 (gfx.h); m_pLineList is the second list (the debug lines and the
 * pause/menu overlay). The draw code checks the list against D_800C2214 (its
 * length, in commands) before adding an object.
 * The lists are void * so that the header does not need gfx.h (game.h
 * includes this one). tools/check_structs.py checks the offsets. */

#include "common.h"

/* The message the scheduler gets when the frame is done (an OSScMsg,
 * Turok 3's GFXMsg). */
typedef struct CFrameMsg {
    /* 0x00 */ s16 m_Type;               /* 2 at start (func_00286440) */
    /* 0x02 */ u8 unk2[0x20 - 0x2];
} CFrameMsg; /* size 0x20 */

typedef struct CFrameData {
    /* 0x000 */ u8 m_SunFrame[0x20];     /* CSunFrameData (CSunFrameData__Construct) */
    /* 0x020 */ u8 m_Task[0x68];         /* OSScTask */
    /* 0x088 */ u8 m_LineTask[0x68];     /* OSScTask */
    /* 0x0F0 */ CFrameMsg m_Msg;         /* sent to D_8011AD28 when the frame is done */
    /* 0x110 */ u32 m_pFrameBuffer;      /* its colour image (D_80130A88[i], gDPSetColorImage) */
    /* 0x114 */ void *m_pDisplayList;    /* Gfx *: the start of the frame's list */
    /* 0x118 */ void *m_pLineList;       /* Gfx *: the line / overlay list */
    /* 0x11C */ s32 m_DisplayListSize;   /* in bytes */
    /* 0x120 */ s32 m_LineListSize;
    /* 0x124 */ s32 m_nPredictTicks;     /* the retraces the frame took (D_8011AD10) */
    /* 0x128 */ s32 m_bLines;            /* the line list has lines (func_00401C1C) */
    /* 0x12C */ struct CFrameData *m_pPrev;
    /* 0x130 */ struct CFrameData *m_pNext;
    /* 0x134 */ u8 unk134[0x140 - 0x134];
} CFrameData; /* size 0x140 */

/* Read as a plain pointer, D_800F7070 makes some files schedule their code
 * differently from the ROM: func_0027D160, func_00286E14, func_00287248 and
 * func_0028852C read it as D_800F6CB0.m_pCurrentFrameData (structs/game.h,
 * the same address), func_004004D8 keeps a local array declaration. */
extern CFrameData *D_800F7070;    /* the frame being built */
extern CFrameData *D_8011F140;    /* the frame being finished */

/* A screen mode, as func_0042464C / func_0042456C start it: D_800C2054 holds
 * the NTSC modes, then the PAL/MPAL ones (D_800C20E0, a symbol of its own
 * that some files use), each indexed by the resolution set D_800C2038
 * (1 with the expansion pak). */
typedef struct VideoMode {
    /* 0x00 */ s32 width;
    /* 0x04 */ s32 height;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
} VideoMode; /* size 0x1C */

extern VideoMode D_800C2054[2][5];
extern VideoMode D_800C20E0[5];

#endif
