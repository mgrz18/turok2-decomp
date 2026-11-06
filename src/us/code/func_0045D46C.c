/* Second-module copy of Video_UpdateResolution (first module; this one at link address 0x0042446C), calling this module's own functions at their link addresses. */
#include "common.h"

s32 GetMemSize(void); /* osGetMemSize */
void Video_ApplyResolution_2(void);

extern s32 gResolution; /* current video mode */
extern s32 gAppliedResolution; /* applied video mode */
extern s32 gVideoClearFrames;

void Video_UpdateResolution_2(void) {
    if (gResolution == -1) {
        if (GetMemSize() != 0x400000) {
            gResolution = 1;
        } else {
            gResolution = 0;
        }
    }
    if (gVideoClearFrames < 5 && gAppliedResolution != gResolution) {
        Video_ApplyResolution_2();
        gAppliedResolution = gResolution;
    }
}
