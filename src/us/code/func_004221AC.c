#include "common.h"
#include "m2c_macros.h"
#include "globals/pak.h"

extern s32 D_801309A8;

void CharNoteMenu_InitDelete(void) {
    sCharNoteLoad = 0;
    D_801309A8 = 0;
}
