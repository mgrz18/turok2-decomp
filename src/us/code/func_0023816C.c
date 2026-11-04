#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"


extern void func_0020EF8C(CVector3 *out, f32 t, CVector3 *a, CVector3 *b);
extern f32 func_002671B4(CPlayer *player);
extern f32 D_800A65B0;

s32 func_0023816C(CGameObjectInstance *actor, CVector3 *pos) {
    s32 ret = 0;
    CPlayer *player;
    u16 state;
    CVector3 a;
    CVector3 b;

    if (actor->header.m_Type == 1 && (actor->m_dwFlags & 0x300000)) {
        player = ((CPlayer *)actor->m_AI.m_pParent);
        if (actor == &player->m_WeaponActor) {
            state = player->m_Data.m_CurrentWeapon;
            if (state < 2 || (s16) state == 0x22) {
                a = *pos;
                b = player->m_CharacterActor.header.m_vPos;
                func_0020EF8C(pos, 0.75f, &a, &b);
                pos->y += func_002671B4(player) * D_800A65B0;
                ret = 1;
            }
        }
    }
    return ret;
}
