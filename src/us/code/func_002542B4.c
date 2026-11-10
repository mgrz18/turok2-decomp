#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"
#include "protos/iset.h"
#include "protos/cache.h"

extern u8 D_800A7094[];
extern void CGameObjectInstance__UpdateBounds(CGameObjectInstance *, void *);
extern void Shadow_DrawBlob(CGameObjectInstance *, CVector3 *, s32);

void Actor_ReloadAnimInfo(CGameObjectInstance *actor, s32 arg1) {
    sptr *res;
    CPlayer *owner;
    CVector3 dir;

    if (actor->m_dwFlags & 0x40000) {
        res = CCache__Request(0, actor->m_rpObject, actor->m_rpObjectHeader, 4, 0, 0,
                            D_800A7094, 1);
        if (res != NULL) {
            owner = (CPlayer *)actor->m_AI.m_pParent;
            CGameObjectInstance__UpdateBounds(actor, CISet__GetBlock(*res, 0));
            if (arg1) {
                dir.x = owner->m_pCamera->m_vPos.x - actor->header.m_vPos.x;
                dir.y = 0.0f;
                dir.z = owner->m_pCamera->m_vPos.z - actor->header.m_vPos.z;
                Shadow_DrawBlob(actor, &dir, arg1);
            }
            CCache__Unlock(0, res);
        }
    }
}
