#include "common.h"
#include "structs/menu.h"

extern s32 D_800B4B84;
extern s32 D_800B4B8C;
extern s32 D_800B4B94;
extern s32 sCharNoteLoad;
extern s32 D_801309A8;

s32 CharNoteMenu_UpdatePrompt(MenuItem *arg0) {
    if (sCharNoteLoad != 0) {
        arg0->table = &D_800B4B84;
    } else if (D_801309A8 != 0) {
        arg0->table = &D_800B4B94;
    } else {
        arg0->table = &D_800B4B8C;
    }
    return 0;
}
