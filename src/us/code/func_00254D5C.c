/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/TEAM/80217388_4.CPP, func_80217928_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/vector.h"
#include "structs/weapon.h"
#include "globals/player.h"


f32 Math_Approach(f32 value, f32 target, f32 rate);
void CMtxF__Identity(f32 *m);
void CMtxF__RotateZ(f32 *m, f32 angle);
void CMtxF__TransformPoint(f32 *m, CVector3 *in, CVector3 *out);
void CBarrel__DrawIcons(CBarrel *menu, CBarrelIcon *items, s32 count, sptr arg3);

/* Lays out the eight slots of the weapon wheel, dropping in as it opens,
 * and draws them. */
void CBarrel__DrawEight(CBarrel *menu, s32 arg1, s32 arg2) {
    f32 m[16];
    CVector3 position;
    CBarrelIcon items[36];
    CVector3 offset;
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
    offset.x = 0.0f;
    offset.y = -0.6f - (0.4f - menu->m_Open * 0.4f);
    offset.z = 0.0f;
    for (i = 0; i < 8; i++) {
        slot = &menu->m_Items[i];
        if (slot->m_Weapon == -1) {
            continue;
        }
        if (i == menu->m_nCursor) {
            target = gBarrelSlotScale * 1.3f;
        } else {
            target = gBarrelSlotScale;
        }
        count++;
        slot->m_Scale = Math_Approach(slot->m_Scale, target, gBarrelSlotScale * 0.1f);
        CMtxF__Identity(m);
        CMtxF__RotateZ(m, slot->m_Angle);
        CMtxF__TransformPoint(m, &offset, &position);
        item->m_Item = i;
        item->m_vPos = position;
        item->m_Icon = gWeaponInfoTable[slot->m_Weapon]->icon;
        item->m_Alpha = 1.0f;
        item->m_Size = 1.0f;
        item++;
    }
    CBarrel__DrawIcons(menu, items, count, arg2);
}
