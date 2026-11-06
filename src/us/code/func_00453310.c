#include "common.h"
#include "structs/menu.h"

void func_0042E06C(void *player);

s32 ContinueMenu_Yes_2(void *item, Menu *menu) {
    func_0042E06C(menu->player);
    return 1;
}
