/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/MISC/8043DF84_2.CPP, func_8043E6A4_de, with Turok 2's layouts. */
#include "common.h"

extern s32 func_0041648C(void *menu, s32 value, s32 step, s32 minimum, s32 maximum, s32 wrap);
extern s32 func_00285670(s32);
extern s32 func_002855E8(s32);

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ s32 controller;
} MenuItem;

extern s32 D_800C1EA8;      /* selected entry, 0..15 */
typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 scale[16];  /* per entry */
} ScaleTable;

extern ScaleTable D_00438900;
extern f32 D_8011AAF4;      /* resulting scale */

/* Steps the selection (twice when the controller reports busy) and sets the scale
 * from the table plus one, clamped to [0.5, 2.0]. */
s32 func_004188A4(void *arg0, MenuItem *item) {
    f32 scale;

    D_800C1EA8 = func_0041648C(item, D_800C1EA8, 1, 0, 15, 0);
    if (func_00285670(item->controller) != 0 || func_002855E8(item->controller) != 0) {
        func_0041648C(item, D_800C1EA8, 1, 0, 15, 0);
    }
    scale = D_00438900.scale[D_800C1EA8] + 1.0f;
    if (!(scale < 0.5f)) {
        if (!(scale > 2.0f)) {
            if (scale < 0.5f) {
                scale = 0.5f;
            }
        } else {
            scale = 2.0f;
        }
    } else {
        scale = 0.5f;
    }
    D_8011AAF4 = scale;
    return 0;
}
