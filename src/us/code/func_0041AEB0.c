#include "common.h"
#include "structs/menu.h"

/* A 0x10-byte entry of D_00439774: the template first. */
typedef struct {
    /* 0x0 */ s32 *tmpl;
    /* 0x4 */ u8 pad4[0xC];
} TemplateEntry;

extern TemplateEntry D_00439774[];
extern s32 D_800C1144;
extern u32 D_80130920;

s32 EnterCheatMenu_ResultLabel(MenuItem *arg0) {
    if ((u32) D_80130920 < 0xCU) {
        arg0->table = D_00439774[D_80130920].tmpl;
    } else {
        arg0->table = &D_800C1144;
    }
    return 0;
}
