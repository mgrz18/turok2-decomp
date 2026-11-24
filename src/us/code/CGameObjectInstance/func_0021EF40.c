#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"


void CMtxF__TransformPoint(void *m, void *in, void *out);
extern void CGameObjectInstance__LocalToWorld(CVector3 *out, CGameObjectInstance *actor, CVector3 v);
void CVector3__Add(void *out, void *a, void *b);
void GameObject_TraceMove(void *actor, CVector3 pos, void *outPos, void *outRegion, s32 flagA, s32 flagB);

void CGameObjectInstance__TraceMoveLocal(CGameObjectInstance *actor, CVector3 v, s32 arg4, s32 arg5) {
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
    GameObject_TraceMove(actor, out, (void *) (sptr) arg4, (void *) (sptr) arg5, 1, 1);
}
