#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

extern s32 D_800B49C4;
extern s32 D_800B49D0;

s32 GameNoteMenu_UpdateTitle(MenuItem *arg0) {
    if (sGameNoteLoad != 0) {
        arg0->table = &D_800B49C4;
    } else {
        arg0->table = &D_800B49D0;
    }
    return 0;
}
