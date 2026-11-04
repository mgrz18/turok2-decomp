/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_00443F40.c, func_00443F40.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"

extern s32 func_0025340C(void *a, void *b);
extern void func_00285A68(s32 a);
extern s32 D_800C2020;
extern s32 D_8011ACE4[];
void func_0041CF68(Menu *a) {
    s32 n;
    D_800C2020 = 0;
    n = 0;
    if (a->player) n = func_0025340C(&D_80119870.m_PlayerList, a->player);
    func_00285A68(n);
    a->items[14].flags |= 0x1800000;
    a->items[15].flags |= 0x1800000;
    a->items[11].flags &= ~0x1800000;
    a->items[12].flags &= ~0x1800000;
    a->items[13].flags &= ~0x1800000;
    if (D_8011ACE4[0]) a->items[7].flags |= 0x1000000; else a->items[7].flags &= ~0x1000000;
}
