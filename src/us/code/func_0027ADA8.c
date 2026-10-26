#include "common.h"

void func_00279028(u8);
void func_00277090(void *, s32);
extern u8 D_8011AAEA;

void func_0027ADA8(void *arg0) {
    u8 pad[0x100];

    func_00279028(D_8011AAEA);
    if (*(s32 *)((u8 *)arg0 + 0x40) != 0) {
        func_00277090(arg0, 1);
    }
    if (*(s32 *)((u8 *)arg0 + 0x88) != 0) {
        func_00277090(arg0, 2);
    }
}
