#include "common.h"
#include "structs/menu.h"

void CPlayer__StartGame_2(void *player);

s32 ContinueMenu_Yes_2(void *item, Menu *menu) {
    CPlayer__StartGame_2(menu->player);
    return 1;
}
