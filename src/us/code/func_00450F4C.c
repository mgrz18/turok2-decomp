#include "common.h"
#include "structs/player.h"
#include "structs/game.h"
#include "structs/menu.h"

s32 func_004160F0(MenuItem *item); /* length of the item's text line */
s32 func_0029E3E0(char *buf, const char *fmt, ...); /* sprintf */

typedef struct MenuPlayer {
    /* 0x000 */ u8 pad0[0x518];
    /* 0x518 */ CPlayerOptions *options;
} MenuPlayer;

extern char D_800B41B0[];
extern char D_800B41C0[];
extern char D_800B41D0[];
extern char D_800B41E0[];
extern char D_800B41F0[];
extern char D_800B4200[];

/* Shows the player's handicap: two special values, otherwise a per-port line with the number. */
s32 func_00450F4C(MenuItem *item, Menu *ctx) {
    s32 handicap = ((MenuPlayer *) ctx->player)->options->m_Handicap;

    if (handicap == 0) {
        item->text = D_800B41F0;
    } else if (handicap == 10) {
        item->text = D_800B4200;
    } else {
        switch (ctx->controller->port) {
        case 0:
        default:
            item->text = D_800B41B0;
            break;
        case 1:
            item->text = D_800B41C0;
            break;
        case 2:
            item->text = D_800B41D0;
            break;
        case 3:
            item->text = D_800B41E0;
            break;
        }
        func_0029E3E0(&item->text[func_004160F0(item) - 2], "%02d", handicap);
    }
    return 0;
}
