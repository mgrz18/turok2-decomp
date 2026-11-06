/* WRONG CUT: REAL CScene__InitPersistentData size 0x5C (splat cut its last word into func_0042C414). */
#include "common.h"

extern s32 D_8011AADC[];

extern void CScene__LoadBitSections(void *menu);
extern void CScene__LoadBitSections2(void *menu);

typedef struct MenuState {
    /* 0x00000 */ u8 pad0[0x18FE0];
    /* 0x18FE0 */ s32 unk18FE0;
    /* 0x18FE4 */ s32 unk18FE4;
    /* 0x18FE8 */ s32 unk18FE8;
} MenuState;

void CScene__InitPersistentData(MenuState *menu) {
    CScene__LoadBitSections(menu);
    CScene__LoadBitSections2(menu);
    menu->unk18FE0 = 0;
    menu->unk18FE4 = 34;
    menu->unk18FE8 = D_8011AADC[0];
}
