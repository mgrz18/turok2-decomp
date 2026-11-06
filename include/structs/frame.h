#ifndef STRUCTS_FRAME_H
#define STRUCTS_FRAME_H

/* One frame in flight (Turok 3's CFrameData, references/turok3/include/
 * tengine.h; 8 bytes longer here: the word at 0x128). The engine keeps three
 * in a ring through m_pPrev / m_pNext (CEngineApp.m_FrameData, structs/
 * game.h; set up by CEngineApp__Construct). gEngineApp_m_pCurrentFrameData (CEngineApp.m_pCurrentFrameData)
 * is the frame being built; CEngineApp__Update copies it to gFrameData, the frame
 * CEngineApp__Draw finishes and sends (m_Msg) to the RSP task queue gGfxFrameMessageQueue.
 * The display list starts at m_pDisplayList and the engine writes it through
 * gDList (gfx.h); m_pLineList is the second list (the debug lines and the
 * pause/menu overlay). The draw code checks the list against gDisplayListSize (its
 * length, in commands) before adding an object.
 * The lists are void * so that the header does not need gfx.h (game.h
 * includes this one). tools/check_structs.py checks the offsets. */

#include "common.h"

/* The message the scheduler gets when the frame is done (an OSScMsg,
 * Turok 3's GFXMsg). */
typedef struct CFrameMsg {
    /* 0x00 */ s16 m_Type;               /* 2 at start (CEngineApp__Construct) */
    /* 0x02 */ u8 unk2[0x20 - 0x2];
} CFrameMsg; /* size 0x20 */

typedef struct CFrameData {
    /* 0x000 */ u8 m_SunFrame[0x20];     /* CSunFrameData (CSunFrameData__Construct) */
    /* 0x020 */ u8 m_Task[0x68];         /* OSScTask */
    /* 0x088 */ u8 m_LineTask[0x68];     /* OSScTask */
    /* 0x0F0 */ CFrameMsg m_Msg;         /* sent to gGfxFrameMessageQueue when the frame is done */
    /* 0x110 */ u32 m_pFrameBuffer;      /* its colour image (gCFB[i], gDPSetColorImage) */
    /* 0x114 */ void *m_pDisplayList;    /* Gfx *: the start of the frame's list */
    /* 0x118 */ void *m_pLineList;       /* Gfx *: the line / overlay list */
    /* 0x11C */ s32 m_DisplayListSize;   /* in bytes */
    /* 0x120 */ s32 m_LineListSize;
    /* 0x124 */ u32 m_nPredictTicks;     /* the retraces the frame took (gNextTicks) */
    /* 0x128 */ s32 m_bLines;            /* the line list has lines (Automap_IsShown) */
    /* 0x12C */ struct CFrameData *m_pPrev;
    /* 0x130 */ struct CFrameData *m_pNext;
    /* 0x134 */ u8 unk134[0x140 - 0x134];
} CFrameData; /* size 0x140 */

/* Read as a plain pointer, gEngineApp_m_pCurrentFrameData makes some files schedule their code
 * differently from the ROM: CCamera__Advance, Render_DrawTextLines, CEngineApp__Draw and
 * CEngineApp__ClearDepthRect read it as gEngineApp.m_pCurrentFrameData (structs/game.h,
 * the same address), Automap_Draw keeps a local array declaration. */
extern CFrameData *gEngineApp_m_pCurrentFrameData;    /* the frame being built */

/* A screen mode, as Video_SetMode / func_0042456C start it: gVideoModes holds
 * the NTSC modes, then the PAL/MPAL ones (gVideoModesPAL, a symbol of its own
 * that some files use), each indexed by the resolution set gResolution
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

extern VideoMode gVideoModes[2][5];
extern VideoMode gVideoModesPAL[5];

#endif
