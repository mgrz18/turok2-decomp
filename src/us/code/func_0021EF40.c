#include "common.h"
#include "structs/game_object.h"

typedef struct Player {
    u8 pad0[0x51C];
    u8 *unk51C;         /* 0x51C */
} Player;


extern void func_0020F85C(void *matrix, CVector3 *in, CVector3 *out);
extern void func_0021F00C(CVector3 *out, CGameObjectInstance *actor, CVector3 v);
extern void func_0020EEF8(CVector3 *out, CVector3 *a, CVector3 *b);
extern void func_002688C8(CGameObjectInstance *actor, CVector3 pos, s32 arg4, s32 arg5, s32 arg6, s32 arg7);

void func_0021EF40(CGameObjectInstance *actor, CVector3 v, s32 arg4, s32 arg5) {
    CVector3 local;
    CVector3 out;

    if (actor->m_dwFlags & 0x300000) {
        v.x = -v.x;
        v.z = -v.z;
        func_0020F85C(((Player *)actor->m_AI.m_pParent)->unk51C + 0x14C, &v, &out);
    } else {
        func_0021F00C(&local, actor, v);
        func_0020EEF8(&out, &actor->header.m_vPos, &local);
    }
    func_002688C8(actor, out, arg4, arg5, 1, 1);
}
