/* WRONG CUT: the real function starts at 0x0045C398 (size 0xE0 = 224): splat
 * cut its first two words (lui/lw sDamagedPakDone) as DamagedPakMenu_Poll_2. */
#include "common.h"
#include "structs/menu.h"

/* Load the selected level once: returns whether it has been loaded. */

extern s32 sDamagedPakDone;
extern s32 gPakMenuAnyPak;
extern s32 gPakMenuPort;

s32 Controller_PakChanged(s32 mode);
s32 PakMenu_CheckPak_2(Menu *event, s32 mode, s32 arg2);

static __inline__ s32 currentMode(Menu *event) {
    s32 mode;

    if (gPakMenuAnyPak != 0) {
        mode = gPakMenuPort;
    } else {
        mode = event->controller->port;
    }
    return mode;
}

s32 DamagedPakMenu_Poll_2(Menu *event) {
    if (sDamagedPakDone == 0) {
        if (currentMode(event) != -1) {
            if (Controller_PakChanged(currentMode(event)) != 0) {
                if (PakMenu_CheckPak_2(event, currentMode(event), 0) != 0) {
                    sDamagedPakDone = 1;
                }
            }
        }
    }
    return sDamagedPakDone;
}
