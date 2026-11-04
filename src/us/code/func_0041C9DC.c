#include "common.h"
#include "structs/menu.h"

extern void func_0042E14C(void *player);

s32 func_0041C9DC(s32 menu, Menu *c) {
    func_0042E14C(c->player);
    return 1;
}
