/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/TEAM/80217388_4.CPP, func_80217928_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/vector.h"
#include "structs/weapon.h"

extern f32 D_800AE2D8;              /* slot scale */

f32 Math_Approach(f32 value, f32 target, f32 rate);
void CMtxF__Identity(f32 *m);
void CMtxF__RotateX(f32 *m, f32 angle);
void CMtxF__TransformPoint(f32 *m, CVector3 *in, CVector3 *out);
void CBarrel__DrawIcons(CBarrel *menu, CBarrelIcon *items, s32 count, s32 arg3);

/* Lays out the weapon menu's slots around the wheel and draws them. */
void CBarrel__DrawRing(CBarrel *menu, s32 arg1, s32 arg2) {
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
            slot->m_Lift = Math_Approach(slot->m_Lift, 0.0f, 0.35f);
        }
        slot->m_Scale = Math_Approach(slot->m_Scale, target, D_800AE2D8 * 0.1f);
        offset.x = 0.0f;
        offset.y = 0.0f;
        offset.z = 0.75f;
        CMtxF__Identity(m);
        CMtxF__RotateX(m, menu->m_Spin - slot->m_Angle);
        CMtxF__TransformPoint(m, &offset, &position);
        count++;
        item->m_Item = i;
        item->m_vPos = position;
        item->m_Icon = D_800AF618[slot->m_Weapon]->icon;
        item->m_Alpha = 0;
        item->m_Size = 1.0f;
        item++;
    }
    CBarrel__DrawIcons(menu, items, count, arg2);
}
