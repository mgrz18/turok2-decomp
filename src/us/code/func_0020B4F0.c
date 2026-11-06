#include "common.h"

void *CISet__GetBlock(s32, s32);                  /* extern */
s32 Model_BlendMorphFrames(s32, f32, void *);            /* extern */
extern s32 gFrameCount;

typedef struct {
    /* 0x00 */ s32 id;
    /* 0x04 */ f32 value;
} Param;

void Model_AdvanceMorph(s32 *arg0, f32 arg1) {
    f32 temp_f0;
    s32 temp_s0;
    Param *temp_v0;

    temp_s0 = *arg0;
    temp_v0 = CISet__GetBlock(temp_s0, 3);
    if (temp_v0->id != gFrameCount) {
        temp_f0 = temp_v0->value + arg1;
        temp_v0->value = temp_f0;
        Model_BlendMorphFrames(temp_s0, temp_f0, temp_v0);
    }
}
