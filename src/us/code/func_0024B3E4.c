/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/TEAM/80217388_4.CPP, func_80217928_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/vector.h"
#include "structs/weapon.h"

extern f32 D_800AE2D8;              /* slot scale */

f32 func_00211774(f32 value, f32 target, f32 rate);
void func_0020F79C(f32 *m);
void func_00210790(f32 *m, f32 angle);
void func_0020F85C(f32 *m, CVector3 *in, CVector3 *out);
void func_0024AF68(CBarrel *menu, CBarrelIcon *items, s32 count, s32 arg3);

/* Lays out the weapon menu's slots around the wheel and draws them. */
void func_0024B3E4(CBarrel *menu, s32 arg1, s32 arg2) {
    f32 m[16];
    CVector3 position;
    CVector3 offset;
    CBarrelIcon items[36];
    CBarrelIcon *item;
    CBarrelItem *slot;
    s32 count;
    s32 i;
    f32 target;

    if (menu->m_State == 0) {
        return;
    }
    item = items;
    count = 0;
    for (i = 0; i < menu->m_nCount; i++) {
        slot = &menu->m_Items[i];
        if (i == menu->m_nCursor) {
            target = D_800AE2D8 * 1.3f;
        } else {
            target = D_800AE2D8;
            slot->m_Lift = func_00211774(slot->m_Lift, 0.0f, 0.35f);
        }
        slot->m_Scale = func_00211774(slot->m_Scale, target, D_800AE2D8 * 0.1f);
        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = 0.75f;
        func_0020F79C(m);
        func_00210790(m, menu->m_Spin - slot->m_Angle);
        func_0020F85C(m, &offset, &position);
        count++;
        item->m_Item = i;
        item->m_vPos = position;
        item->m_Icon = D_800AF618[slot->m_Weapon]->icon;
        item->m_Alpha = 0;
        item->m_Size = 1.0f;
        item++;
    }
    func_0024AF68(menu, items, count, arg2);
}
