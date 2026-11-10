#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"


extern void CMtxF__TransformPoint(void *matrix, CVector3 *in, CVector3 *out);
extern void CGameObjectInstance__LocalToWorld(CVector3 *out, CGameObjectInstance *actor, CVector3 v);
extern void CVector3__Add(CVector3 *out, CVector3 *a, CVector3 *b);
extern void GameObject_TraceMove(CGameObjectInstance *actor, CVector3 pos, sptr arg4, sptr arg5, s32 arg6, s32 arg7);

void func_0021EF40(CGameObjectInstance *actor, CVector3 v, s32 arg4, s32 arg5) {
    CVector3 local;
    CVector3 out;

    if (actor->m_dwFlags & 0x300000) {
        v.x = -v.x;
        v.z = -v.z;
        CMtxF__TransformPoint(((u8 *)((CPlayer *)actor->m_AI.m_pParent)->m_pCamera) + 0x14C, &v, &out);
    } else {
        CGameObjectInstance__LocalToWorld(&local, actor, v);
        CVector3__Add(&out, &actor->header.m_vPos, &local);
    }
    GameObject_TraceMove(actor, out, arg4, arg5, 1, 1);
}
