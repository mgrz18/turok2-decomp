#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"

extern s32 D_800C1EA8;     /* the speed setting, 0-15 */
extern f32 D_00438824[];   /* speed per setting */

s32 func_0041648C(Menu *menu, s32 value, s32 step, s32 minimum, s32 maximum, s32 wrap);
s32 func_00285670(s32 controller);
s32 func_002855E8(s32 controller);

#define MIN(a, b) ((b) < (a) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

/* Adjusts the game speed setting and stores its multiplier, clamped to [0.5, 2]. */
s32 func_00451804(void *item, Menu *menu) {
    f32 speed;

    D_800C1EA8 = func_0041648C(menu, D_800C1EA8, 1, 0, 15, 0);
    if (func_00285670(menu->controller) || func_002855E8(menu->controller)) {
        func_0041648C(menu, D_800C1EA8, 1, 0, 15, 0);
    }
    speed = D_00438824[D_800C1EA8] + 1.0f;
    speed = MIN(MAX(speed, 0.5f), 2.0f);
    D_8011AAD8.speed = speed;
    return 0;
}
