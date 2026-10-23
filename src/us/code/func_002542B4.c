#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"

extern u8 D_800A7094[];
extern s32 *func_0020367C(s32, void *, u32, s32, s32, s32, void *, s32);
extern void *func_002017D4(s32 table, s32 index);
extern void func_0021E7F8(CGameObjectInstance *, void *);
extern void func_0021E03C(CGameObjectInstance *, CVector3 *, s32);
extern void func_002051F4(s32, void *);

void func_002542B4(CGameObjectInstance *actor, s32 arg1) {
    s32 *res;
    CPlayer *owner;
    CVector3 dir;

    if (actor->m_dwFlags & 0x40000) {
        res = func_0020367C(0, actor->m_rpObject, actor->m_rpObjectHeader, 4, 0, 0,
                            D_800A7094, 1);
        if (res != NULL) {
            owner = (CPlayer *)actor->m_AI.m_pParent;
            func_0021E7F8(actor, func_002017D4(*res, 0));
            if (arg1) {
                dir.x = owner->m_pCamera->m_vPos.x - actor->header.m_vPos.x;
                dir.y = 0.0f;
                dir.z = owner->m_pCamera->m_vPos.z - actor->header.m_vPos.z;
                func_0021E03C(actor, &dir, arg1);
            }
            func_002051F4(0, res);
        }
    }
}
