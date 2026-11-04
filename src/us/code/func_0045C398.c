/* WRONG CUT: the real function starts at 0x0045C398 (size 0xE0 = 224): splat
 * cut its first two words (lui/lw D_8013099C) as func_0045C398. */
#include "common.h"
#include "structs/menu.h"

/* Load the selected level once: returns whether it has been loaded. */

extern s32 D_8013099C;
extern s32 D_80130990;
extern s32 D_800C2028;

s32 func_0028591C(s32 mode);
s32 func_0041DCB0(Menu *event, s32 mode, s32 arg2);

static __inline__ s32 currentMode(Menu *event) {
    s32 mode;

    if (D_80130990 != 0) {
        mode = D_800C2028;
    } else {
        mode = event->controller->port;
    }
    return mode;
}

s32 func_0045C398(Menu *event) {
    if (D_8013099C == 0) {
        if (currentMode(event) != -1) {
            if (func_0028591C(currentMode(event)) != 0) {
                if (func_0041DCB0(event, currentMode(event), 0) != 0) {
                    D_8013099C = 1;
                }
            }
        }
    }
    return D_8013099C;
}
