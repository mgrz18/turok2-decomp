#include "common.h"

typedef struct {
    /* 0x00 */ s32 type;
    /* 0x04 */ f32 value;
} Setting;

void CShakeAxis__Set(Setting *arg0, f32 arg1, s32 arg2) {
    arg0->type = arg2;
    arg0->value = (f32) (arg1 * 0.1f);
}
