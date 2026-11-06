#include "common.h"

s32 GetMemSize(void); /* osGetMemSize */
void Video_ApplyResolution(void);

extern s32 gResolution; /* current video mode */
extern s32 gAppliedResolution; /* applied video mode */
extern s32 gVideoClearFrames;

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
