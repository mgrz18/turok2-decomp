#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s16 type;
    /* 0x06 */ u8 pad6[0xE];
    /* 0x14 */ u8 *text;
} MenuItemTmpl;

extern u8 *func_00413230(void);
extern f32 func_004163C0(u8 *text, f32 sx, f32 sy);

/* Width of the item's text at the given scale. */
f32 func_00416BAC(MenuItemTmpl *a, f32 scale) {
    u8 *s;

    if (a->type == 5) {
        s = func_00413230();
    } else {
        s = a->text;
    }
    return func_004163C0(s, scale, scale);
}
