#include "common.h"
#include "structs/menu.h"
#include "globals/menu.h"

/* A 0x10-byte entry of sCheatCodes_unk4: the template first. */
typedef struct {
    /* 0x0 */ s32 *tmpl;
    /* 0x4 */ u8 pad4[0xC];
} TemplateEntry;

extern TemplateEntry sCheatCodes_unk4[];
extern s32 D_800C1144;

s32 EnterCheatMenu_ResultLabel(MenuItem *arg0) {
    if ((u32) sEnterCheatResult < 0xCU) {
        arg0->table = sCheatCodes_unk4[sEnterCheatResult].tmpl;
    } else {
        arg0->table = &D_800C1144;
    }
    return 0;
}
