#include "common.h"
#include "globals/scene.h"
#include "protos/scene.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ sptr targetId; /* header.m_pCurrentRegion: a pointer */
} Link;


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
