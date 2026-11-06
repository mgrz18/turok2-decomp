#include "common.h"
#include "m2c_macros.h"

extern s32 D_80130960;
extern s32 D_801309A8;

void CharNoteMenu_InitDelete(void) {
    D_80130960 = 0;
    D_801309A8 = 0;
}
