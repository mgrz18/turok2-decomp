#include "common.h"
#include "structs/game_object.h"

extern char D_800A5AC4[];

void **CCache__Request(s32, s32, s32, s32, s32, s32, void *, s32); /* resource load */
void CCache__Unlock(s32, void **);                                 /* resource free */
void **CGameObjectInstance__LoadGeometry(void *obj);
s32 CISet__GetBlock(void *, s32);
s32 CGameObjectInstance__BuildBoneMatrices(void *, void *, s32);

s32 CGameObjectInstance__LoadBoneMatrices(CGameObjectInstance *obj, s32 arg1) {
    s32 result;
    void **res;
    void **sub;
    void *first;

    result = 0;
    if (!(obj->m_dwFlags & 0x40000)) {
        return result;
    }
    res = CCache__Request(0, (s32)obj->m_rpObject, obj->m_rpObjectHeader, 0, 0, 0, D_800A5AC4, arg1);
    if (res != NULL) {
        sub = CGameObjectInstance__LoadGeometry(obj);
        if (sub != NULL) {
            first = *sub;
            result = CGameObjectInstance__BuildBoneMatrices(obj, first, CISet__GetBlock(*res, 0));
            CCache__Unlock(0, sub);
        }
        CCache__Unlock(0, res);
    }
    return result;
}
