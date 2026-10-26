/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_0043F404.c, func_0043F404, with Turok 2's layouts. */
/* A menu slider: step its bound variable (any numeric type) with left/right. */

#include "common.h"

struct MenuItem;

typedef struct MenuBinding {
    /* 0x00 */ char *name;
    /* 0x04 */ s32 type;            /* 0 f32, 1/2/5 s32, 3 s8, 4 s16, 6 u8, 7 u16, 8 u32 */
    /* 0x08 */ f32 min;
    /* 0x0C */ f32 max;
    /* 0x10 */ f32 step;
    /* 0x14 */ f32 unk14;
    /* 0x18 */ void *(*storage)(struct MenuItem *item);
} MenuBinding;

typedef struct MenuItem {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ MenuBinding *binding;
} MenuItem;

typedef struct Menu {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ s32 controller;
} Menu;

extern s32 func_00285628(s32 controller);   /* left pressed */
extern s32 func_00285670(s32 controller);   /* right pressed */

static __inline__ f32 bindingGet(MenuItem *item) {
    MenuBinding *b = item->binding;
    void *p = b->storage(item);

    switch (b->type) {
        default: return 0.0f;
        case 0: return *(f32 *)p;
        case 3: return *(s8 *)p;
        case 4: return *(s16 *)p;
        case 1: case 2: case 5: return *(s32 *)p;
        case 6: return *(u8 *)p;
        case 7: return *(u16 *)p;
        case 8: return *(u32 *)p;
    }
}

static __inline__ void bindingSet(MenuItem *item, f32 value) {
    MenuBinding *b = item->binding;
    void *p = b->storage(item);

    switch (b->type) {
        case 0: *(f32 *)p = value; return;
        case 3: *(s8 *)p = (s32)value; return;
        case 4: *(s16 *)p = (s32)value; return;
        case 1: case 2: case 5: *(s32 *)p = (s32)value; return;
        case 6: *(u8 *)p = (u32)value; return;
        case 7: *(u16 *)p = (u32)value; return;
        case 8: *(u32 *)p = (u32)value; return;
    }
}

s32 func_004133A4(MenuItem *item, Menu *menu) {
    MenuBinding *b = item->binding;
    f32 value;

    value = bindingGet(item);
    if (func_00285628(menu->controller)) {
        value -= b->step;
        if (value < b->min) {
            value = b->min;
        }
    } else if (func_00285670(menu->controller)) {
        value += b->step;
        if (b->max < value) {
            value = b->max;
        }
    }
    bindingSet(item, value);
    return 0;
}
