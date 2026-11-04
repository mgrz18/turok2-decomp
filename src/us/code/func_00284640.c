#include "common.h"

/* Cutscene/cue playback context (D_800C1BB0); only the field read here.
 * The fuller layout is in func_00283CD4.c. */
typedef struct CueContext {
    /* 0x00 */ u8 pad00[0x38];
    /* 0x38 */ s32 active;
} CueContext;

extern CueContext *D_800C1BB0;

s32 func_00284640(void) {
    return D_800C1BB0->active;
}
