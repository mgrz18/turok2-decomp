#include "common.h"
#include "m2c_macros.h"

extern s32 sCharNoteLoad;
extern s32 D_801309A8;

void CharNoteMenu_InitLoad_2(void) {
    sCharNoteLoad = 1;
    D_801309A8 = 0;
}
