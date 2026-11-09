#include "common.h"

#include "structs/swoosh.h"

/* A swoosh (structs/swoosh.h); layer < 0: drawn untextured (Trail_DrawUntextured). */
typedef Swoosh Trail;

s32 Trail_ComputeFrames(Trail *, s32);             /* extern */
s32 Trail_PlaceNodes(Trail *);                  /* extern */
s32 Trail_DrawTextured(sptr, Trail *, s32);        /* extern */
s32 Trail_DrawUntextured(sptr, Trail *, s32);        /* extern */

void Trail_Draw(sptr arg0, Trail *arg1, s32 arg2) {
    if (arg1->flags & 8) {
        Trail_PlaceNodes(arg1);
    }
    if (arg1->flags & 4) {
        Trail_ComputeFrames(arg1, arg2);
    }
    if (arg1->nodes.count >= 2) {
        if (arg1->layer >= 0) {
            Trail_DrawTextured(arg0, arg1, arg2);
            return;
        }
        Trail_DrawUntextured(arg0, arg1, arg2);
    }
}
