#include "common.h"

#include "structs/swoosh.h"

/* A swoosh (structs/swoosh.h); layer < 0: drawn untextured (Trail_DrawUntextured). */
typedef Swoosh Trail;

void Trail_ComputeFrames(void *arg0, sptr arg1);
void Trail_PlaceNodes(Swoosh *e);
void Trail_DrawTextured(void *pool, Swoosh *trail, s32 unused);
void Trail_DrawUntextured(CSwooshSystem *pool, Swoosh *actor, s32 unused);

void Trail_Draw(sptr arg0, Trail *arg1, s32 arg2) {
    if (arg1->flags & 8) {
        Trail_PlaceNodes(arg1);
    }
    if (arg1->flags & 4) {
        Trail_ComputeFrames(arg1, arg2);
    }
    if (arg1->nodes.count >= 2) {
        if (arg1->layer >= 0) {
            Trail_DrawTextured((void *) arg0, arg1, arg2);
            return;
        }
        Trail_DrawUntextured((CSwooshSystem *) arg0, arg1, arg2);
    }
}
