#include "common.h"

extern s32 gLoadedFont;

f32 COnScreen__GetFontAdvance(void) {
    switch (gLoadedFont) {
        case 5:
            return 6.0f;
        case 4:
            return 8.0f;
        case 1:
            return 12.0f;
        case 0:
            return 24.0f;
        case 3:
            return 16.0f;
        default:
            return 1.0f;
    }
}
