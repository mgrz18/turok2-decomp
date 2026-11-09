#include "common.h"
#include "structs/player.h"
#include "structs/menu.h"

extern char D_800B41B0[];
extern char D_800B41C0[];
extern char D_800B41D0[];
extern char D_800B41E0[];
extern char D_800B41F0[];
extern char D_800B4200[];
extern s32 MenuItem_TextLength(MenuItem *a);
#ifdef TARGET_PORT
/* variadic on the host: arm64 Apple passes variadic arguments on the stack (docs/port/MACOS.md) */
extern void sprintf(char *dst, const char *fmt, ...);
#else
extern void sprintf(char *dst, char *fmt, s32 value);
#endif

/* Label of the handicap item, with the handicap value printed into it. */
s32 PlayerSetupMenu_HandicapLabel(MenuItem *a, Menu *c) {
    s32 handicap = c->player->m_pOptions->m_Handicap;

    if (handicap == 0) {
        a->text = D_800B41F0;
    } else if (handicap == 10) {
        a->text = D_800B4200;
    } else {
        switch (c->controller->port) {
        case 0:
        default:
            a->text = D_800B41B0;
            break;
        case 1:
            a->text = D_800B41C0;
            break;
        case 2:
            a->text = D_800B41D0;
            break;
        case 3:
            a->text = D_800B41E0;
            break;
        }
        sprintf(a->text + (MenuItem_TextLength(a) - 2), "%02d", handicap);
    }
    return 0;
}
