#include "common.h"
#include "globals/scene.h"
#include "protos/scene.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 targetId;
} Link;


#ifdef TARGET_PORT
#include "structs/game_object.h"
/* host: the actor's current region is a pointer at 0x10 of its header */
sptr GameObject_GetRegionInfo(CInstanceHeader *arg0) {
    struct CGameRegion *r = arg0->m_pCurrentRegion;
    return r != NULL ? (sptr) CScene__GetRegionInfo(&gScene, (u16 *) r) : 0;
}
#else
sptr GameObject_GetRegionInfo(Link *arg0) {
    sptr temp_a1;
    sptr var_v0;

    temp_a1 = arg0->targetId;
    var_v0 = 0;
    if (temp_a1 != 0) {
        var_v0 = CScene__GetRegionInfo(&gScene, temp_a1);
    }
    return var_v0;
}
#endif
