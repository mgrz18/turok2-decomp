#include "common.h"

/* A 0x10-byte entry of D_00439774: the template first. */
typedef struct {
    /* 0x0 */ s32 *tmpl;
    /* 0x4 */ u8 pad4[0xC];
} TemplateEntry;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 *tmpl;
} MenuItem;

extern TemplateEntry D_00439774[];
extern s32 D_800C1144;
extern u32 D_80130920;

s32 func_0041AEB0(MenuItem *arg0) {
    if ((u32) D_80130920 < 0xCU) {
        arg0->tmpl = D_00439774[D_80130920].tmpl;
    } else {
        arg0->tmpl = &D_800C1144;
    }
    return 0;
}
