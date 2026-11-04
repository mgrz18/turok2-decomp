#include "common.h"
#include "structs/menu.h"

s32 func_0042E06C(void *player);                         /* extern */

s32 func_0045593C(s32 menu, Menu *c) {
    func_0042E06C(c->player);
    return 1;
}
