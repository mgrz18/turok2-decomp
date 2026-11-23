#include "common.h"
#include "globals/scene.h"
#include "protos/scene.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ sptr targetId; /* header.m_pCurrentRegion: a pointer */
} Link;


#include "structs/game_object.h"
/* host: the actor's current region is a pointer at 0x10 of its header */
sptr GameObject_GetRegionInfo(CInstanceHeader *arg0) {
    struct CGameRegion *r = arg0->m_pCurrentRegion;
    return r != NULL ? (sptr) CScene__GetRegionInfo(&gScene, (u16 *) r) : 0;
}
