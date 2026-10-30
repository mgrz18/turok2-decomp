/* WRONG CUT: REAL func_0042C3BC size 0x5C (splat cut its last word into func_0042C414). */
#include "common.h"

extern s32 D_8011AADC[];

extern void func_0042C418(void *menu);
extern void func_0042C46C(void *menu);

typedef struct MenuState {
    /* 0x00000 */ u8 pad0[0x18FE0];
    /* 0x18FE0 */ s32 unk18FE0;
    /* 0x18FE4 */ s32 unk18FE4;
    /* 0x18FE8 */ s32 unk18FE8;
} MenuState;

void func_0042C3BC(MenuState *menu) {
    func_0042C418(menu);
    func_0042C46C(menu);
    menu->unk18FE0 = 0;
    menu->unk18FE4 = 34;
    menu->unk18FE8 = D_8011AADC[0];
}
