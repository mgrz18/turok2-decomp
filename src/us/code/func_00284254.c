#include "common.h"

/* Cutscene/cue playback context (D_800C1BB0); only the field read here.
 * The fuller layout is in func_00283CD4.c. */
typedef struct CueContext {
    /* 0x00 */ u8 pad00[0x38];
    /* 0x38 */ s32 active;
} CueContext;

void func_00410CE8();                               /* extern */
extern CueContext *D_800C1BB0;

void func_00284254(void) {
    if (D_800C1BB0->active != 0) {
        func_00410CE8();
    }
}
