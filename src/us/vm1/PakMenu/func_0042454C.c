#include "common.h"
#include "globals/render.h"

s32 GetMemSize(void); /* osGetMemSize */
void Video_ApplyResolution(void);


void Video_UpdateResolution(void) {
    if (gResolution == -1) {
        if (GetMemSize() != 0x400000) {
            gResolution = 1;
        } else {
            gResolution = 0;
        }
    }
    if (gVideoClearFrames < 5 && gAppliedResolution != gResolution) {
        Video_ApplyResolution();
        gAppliedResolution = gResolution;
    }
}
