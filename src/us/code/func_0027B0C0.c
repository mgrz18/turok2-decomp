#include "common.h"

typedef struct {
    /* 0x00 */ f32 x;
    /* 0x04 */ f32 y;
} FontScale;

extern FontScale D_801101A8;

void COnScreen__SetFontScale(f32 arg0, f32 arg1) {
    D_801101A8.x = arg0;
    D_801101A8.y = arg1;
}
