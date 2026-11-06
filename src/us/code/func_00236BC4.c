#include "common.h"

/* A swoosh trail (see Trail_DrawTextured / Trail_PlaceNodes). */
typedef struct {
    /* 0x00 */ u8 pad0[0x34];
    /* 0x34 */ s32 texture;            /* < 0: drawn untextured (Trail_DrawUntextured) */
    /* 0x38 */ u8 pad38[0x3C - 0x38];
    /* 0x3C */ s32 flags;
    /* 0x40 */ u8 pad40[0x48 - 0x40];
    /* 0x48 */ s32 nodeCount;
} Trail;

s32 Trail_ComputeFrames(Trail *, s32);             /* extern */
s32 Trail_PlaceNodes(Trail *);                  /* extern */
s32 Trail_DrawTextured(s32, Trail *, s32);        /* extern */
s32 Trail_DrawUntextured(s32, Trail *, s32);        /* extern */

void Trail_Draw(s32 arg0, Trail *arg1, s32 arg2) {
    if (arg1->flags & 8) {
        Trail_PlaceNodes(arg1);
    }
    if (arg1->flags & 4) {
        Trail_ComputeFrames(arg1, arg2);
    }
    if (arg1->nodeCount >= 2) {
        if (arg1->texture >= 0) {
            Trail_DrawTextured(arg0, arg1, arg2);
            return;
        }
        Trail_DrawUntextured(arg0, arg1, arg2);
    }
}
