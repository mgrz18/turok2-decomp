/* Second-module copy of PakConfirmMenu_SelectNo (first module). NOTE: the ROM code reads jtbl_800C1450, whose entries are the first module copy's case addresses (link base 0x420F34, 0xE0 above this copy's 0x420E54): the switch here builds its own table, which will not equal the ROM's. */
#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"

s32 PakMenu_CheckPak_2(void *event, s32 port, s32 arg2);
char *PakMenu_GetReturnMenu_2(void *event);
void MenuPool_OpenMenu_2(char *dst, char *fmt, CPlayer *player, s32 arg3, char *arg4);

extern char D_0043BDF0[];
extern char D_0043BE14[];
extern char D_80119E2C[];
extern s32 D_800C2028;
extern s32 D_8011F148;
extern s32 D_80130940;
extern s32 D_80130990;
extern u32 D_801309B0; /* menu state */
extern s32 D_801309BC;

static __inline__ s32 GetPort(Menu *event) {
    if (D_80130990 != 0) {
        return D_800C2028;
    }
    return event->controller->port;
}

static __inline__ void ShowEvent(Menu *event) {
    if (PakMenu_CheckPak_2(event, GetPort(event), 1) != 0) {
        D_801309BC = 1;
        MenuPool_OpenMenu_2(D_80119E2C, event->context, event->player, (s32) event->controller, NULL);
    } else {
        MenuPool_OpenMenu_2(D_80119E2C, event->context, event->player, (s32) event->controller, NULL);
    }
}

s32 PakConfirmMenu_SelectNo_2(s32 unused, Menu *event) {
    char *text;
    char *dst;

    switch (D_801309B0) {
        case 4:
            ShowEvent(event);
            break;
        case 3:
            ShowEvent(event);
            break;
        case 0:
            ShowEvent(event);
            break;
        case 1:
        case 2:
            D_801309BC = 1;
            D_80130940 = 0;
            if (D_80130990 != 0) {
                D_8011F148 = 1;
                return 1;
            }
            text = PakMenu_GetReturnMenu_2(event);
            if (text == NULL) {
                text = event->context;
            }
            if (event->context == D_0043BE14 || event->context == D_0043BDF0) {
                D_80130940 = 0;
            }
            if (event->player != NULL) {
                dst = (char *)event->player->m_pCamera + 0x53C;
            } else {
                dst = D_80119E2C;
            }
            MenuPool_OpenMenu_2(dst, text, event->player, (s32) event->controller, NULL);
            break;
    }
    return 1;
}
