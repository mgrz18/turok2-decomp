/* WRONG CUT: splat's func_0041321C (0x41321C-0x4133A4) is the shared return of func_004131B0 (3 words), a padding nop,
 * one junk word (0x41322C) and this function, which really starts at 0x413230 (0x174 bytes). */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/ovl_a/func_0043F290.c, func_0043F290, with Turok 2's layouts. */
#include "common.h"

struct MenuItem;

typedef struct MenuBinding {
    /* 0x00 */ char *name;
    /* 0x04 */ s32 type; /* 0 f32, 1/2/5 s32, 3 s8, 4 s16, 6 u8, 7 u16, 8 u32 */
    /* 0x08 */ f32 min;
    /* 0x0C */ f32 max;
    /* 0x10 */ f32 step;
    /* 0x14 */ f32 scale;
    /* 0x18 */ void *(*storage)(struct MenuItem *item);
} MenuBinding;

typedef struct MenuItem {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ MenuBinding *binding;
} MenuItem;

extern s32 func_0029E3E0(char *buf, const char *fmt, ...); /* sprintf */
extern char D_801308A0[];
extern char D_80130860[];

/* Reads the item's bound variable as a float, whatever its type. */
static __inline__ f32 MenuItem_GetValue(MenuItem *item) {
    MenuBinding *binding = item->binding;
    void *p = binding->storage(item);

    switch (binding->type) {
        default:
            return 0.0f;
        case 0:
            return *(f32 *) p;
        case 3:
            return (f32) *(s8 *) p;
        case 4:
            return (f32) *(s16 *) p;
        case 1:
        case 2:
        case 5:
            return (f32) *(s32 *) p;
        case 6:
            return (f32) *(u8 *) p;
        case 7:
            return (f32) *(u16 *) p;
        case 8:
            return (f32) *(u32 *) p;
    }
}

/* Formats the item's value (scaled) after its name. */
char *func_00413230(MenuItem *item) {
    MenuBinding *binding = item->binding;
    f32 value = MenuItem_GetValue(item);

    value = value / binding->scale;
    func_0029E3E0(D_801308A0, "%3.2f", (f64) value);
    if (binding->name) {
        func_0029E3E0(D_80130860, "%s %s", binding->name, D_801308A0);
    } else {
        func_0029E3E0(D_80130860, "%s", D_801308A0);
    }
    return D_80130860;
}
