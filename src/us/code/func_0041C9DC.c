#include "common.h"
#include "structs/menu.h"

extern void CPlayer__StartGame(void *player);

s32 StatsMenu_ContinueYes(s32 menu, Menu *c) {
    CPlayer__StartGame(c->player);
    return 1;
}
