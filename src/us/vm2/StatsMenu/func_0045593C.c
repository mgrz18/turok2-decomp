#include "common.h"
#include "structs/menu.h"

s32 CPlayer__StartGame_2(void *player);                         /* extern */

s32 StatsMenu_ContinueYes_2(s32 menu, Menu *c) {
    CPlayer__StartGame_2(c->player);
    return 1;
}
