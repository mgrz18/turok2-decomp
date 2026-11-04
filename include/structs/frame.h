#ifndef STRUCTS_FRAME_H
#define STRUCTS_FRAME_H

/* A frame's graphics task: what the engine draws into and hands to the
 * scheduler. D_800F7070 is the frame being built; func_00287ABC copies it
 * to D_8011F140, the frame func_00287248 finishes and sends (its msg) to
 * the RSP task queue D_8011AD28. The display list starts at dlStart and
 * the engine writes it through D_800D8E24 (gfx.h); dlStart2 is a second
 * list for the pause/menu overlay. The draw code checks the list against
 * D_800C2214 (its length, in commands) before adding an object.
 * tools/check_structs.py checks the offsets. */

#include "common.h"
#include "gfx.h"

typedef struct GfxFrame {
    /* 0x000 */ u8 unk0[0xF0];
    /* 0x0F0 */ u8 msg[0x20];           /* sent to D_8011AD28 when the frame is done */
    /* 0x110 */ u32 colorImage;         /* the frame buffer (gDPSetColorImage) */
    /* 0x114 */ Gfx *dlStart;           /* the frame's display list */
    /* 0x118 */ Gfx *dlStart2;          /* the overlay's display list */
    /* 0x11C */ s32 dlSize;             /* in bytes */
    /* 0x120 */ s32 dlSize2;
    /* 0x124 */ s32 frame;              /* the retraces the frame took (D_8011AD10) */
    /* 0x128 */ s32 hasOverlay;         /* func_00401C1C drew the overlay */
} GfxFrame;

extern GfxFrame *D_800F7070;    /* the frame being drawn */
extern GfxFrame *D_8011F140;    /* the frame being finished */

#endif
