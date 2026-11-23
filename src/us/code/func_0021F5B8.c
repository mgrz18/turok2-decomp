#include "common.h"
#include "globals/strings.h"
#include "structs/game_object.h"
#include "protos/iset.h"
#include "protos/cache.h"



s32 func_0021F5B8(CGameObjectInstance *obj, s32 arg1) {
    s32 temp_s0;
    sptr temp_v0;

    if (obj->m_dwFlags & 0x40000) {
        temp_v0 = CCache__Request(0, ((sptr) obj->m_rpObject), ((s32) obj->m_rpObjectHeader), 4, 0, 0, &sCGameObjectInstanceCacheDesc_AnimObjectInfo, 1);
        if (temp_v0 != 0) {
            temp_s0 = ((s32 *)(CISet__GetBlock(*(s32 *)temp_v0, 1) + (arg1 << 2)))[2];
            CCache__Unlock(0, temp_v0);
            return temp_s0;
        }
    }
    return -1;
}
