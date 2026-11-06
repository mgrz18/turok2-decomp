#include "common.h"
#include "structs/menu.h"

extern s32 D_800B4B84;
extern s32 D_800B4B8C;
extern s32 D_801309B8;

s32 GameNoteMenu_UpdatePrompt(MenuItem *arg0) {
    if (D_801309B8 != 0) {
        arg0->table = &D_800B4B84;
    } else {
        arg0->table = &D_800B4B8C;
    }
    return 0;
}
