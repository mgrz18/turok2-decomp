/* the real function is 0x004242EC-0x00424508 (size 0x21C, 135 words); splat cut its last two words (jr ra; addiu sp) off as func_00424500.
 * All 135 words match (checked over the real extent). First-module copy of src/us/code/Video_NextResolution_2.c. */
#include "common.h"
#include "structs/frame.h"

s32 GetMemSize(void); /* osGetMemSize */

#define MAX(a, b) ((a) >= (b) ? (a) : (b))

extern s32 osTvType; /* osTvType */
extern s32 gScreenWidth;
extern s32 gScreenHeight;
extern s32 gResolution; /* current mode */
extern s32 gAppliedResolution; /* previous mode */
extern s32 gVideoModeCounts[2]; /* mode count per TV region */

/* Steps to the next video mode of the TV region that fits in memory. */
void Video_NextResolution(void) {
    s32 region = 0;
    s32 *mode;
    s32 limit;

    switch (osTvType) {
        case 0:
        case 2:
            region = 1;
            break;
        case 1:
            break;
    }
    mode = (s32 *)&gVideoModes[region][gResolution];
    if (GetMemSize() != 0x400000) {
        limit = 0x2A300;
    } else {
        limit = 0xF660;
    }
    gResolution = (gResolution + 1) % gVideoModeCounts[region];
    while (mode[0] * mode[1] > limit) {
        gResolution = (gResolution + 1) % gVideoModeCounts[region];
    }
    gScreenWidth = MAX(gVideoModes[region][gResolution].width, gVideoModes[region][gAppliedResolution].width);
    gScreenHeight = MAX(gVideoModes[region][gResolution].height, gVideoModes[region][gAppliedResolution].height);
}
