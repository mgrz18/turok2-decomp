#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"


extern void CVector3__Lerp(CVector3 *out, f32 t, CVector3 *a, CVector3 *b);
extern f32 GameObject_GetCollisionHeight(CPlayer *player);

s32 Blast_GetSourcePos(CGameObjectInstance *actor, CVector3 *pos) {
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
                CVector3__Lerp(pos, 0.75f, &a, &b);
                pos->y += GameObject_GetCollisionHeight(player) * 0.5f;
                ret = 1;
            }
        }
    }
    return ret;
}
