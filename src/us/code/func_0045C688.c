/* WRONG CUT: the real function starts at 0x0045C688 (size 0x118 = 280): splat
 * cut its first two words (lui/lw D_80130990) as DamagedPakMenu_SelectRetry_2. */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"

/* Start the selected level, then open the next menu page (on the player's own
 * menu when there is a player). */

extern s32 D_80130990;
extern s32 D_800C2028;
extern s32 D_801309BC;
extern s32 D_80130940;
extern u8 D_80119E2C[];
extern u8 D_0043BE14[];
extern u8 D_0043BDF0[];

s32 PakMenu_CheckPak_2(Menu *event, s32 mode, s32 arg2);
void MenuPool_OpenMenu_2(void *menu, void *page, void *player, void *args, s32 flags);

static __inline__ s32 currentMode(Menu *event) {
    s32 mode;

    if (D_80130990 != 0) {
        mode = D_800C2028;
    } else {
        mode = event->controller->port;
    }
    return mode;
}

s32 DamagedPakMenu_SelectRetry_2(void *arg0, Menu *event) {
    s32 mode = currentMode(event);
    void *menu;

    D_801309BC = 1;
    if (PakMenu_CheckPak_2(event, mode, 1) != 0) {
        if (event->player != NULL) {
            menu = (u8 *)event->player->m_pCamera + 0x53C;
        } else {
            menu = D_80119E2C;
        }
        if (event->context == D_0043BE14 || event->context == D_0043BDF0) {
            D_80130940 = 0;
        }
        MenuPool_OpenMenu_2(menu, event->context, event->player, event->controller, 0);
        return 1;
    } else {
        if (event->context == D_0043BE14 || event->context == D_0043BDF0) {
            D_80130940 = 0;
        }
        MenuPool_OpenMenu_2(D_80119E2C, event->context, event->player, event->controller, 0);
        return 1;
    }
}
