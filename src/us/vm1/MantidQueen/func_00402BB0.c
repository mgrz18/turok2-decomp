#include "common.h"
#include "globals/onscreen.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/ai.h"

typedef struct {
    f32 x, y, z;
} Vec3_00402BB0;

#ifdef TARGET_PORT
#include "structs/game_object.h"
/* the views at the host offsets (round 9 x2): the instances' position and region pointer
 * (header.m_vPos, header.m_pCurrentRegion: the region was kept in an s32) and the CAI's
 * m_vDesiredPos (N64 0x54) */
typedef struct {
    union {
        struct { u8 _hp_pos[__builtin_offsetof(CGameObjectInstance, header.m_vPos)]; Vec3_00402BB0 pos; };
    };
} Marker00402BB0;

typedef struct {
    union {
        struct { u8 _hp_pos[__builtin_offsetof(CGameObjectInstance, header.m_vPos)]; Vec3_00402BB0 pos; };
        struct { u8 _hp_region[__builtin_offsetof(CGameObjectInstance, header.m_pCurrentRegion)]; sptr region; };
    };
} Obj00402BB0;

typedef struct {
    union {
        struct { u8 _hp_pos[__builtin_offsetof(CAI, m_vDesiredPos)]; Vec3_00402BB0 pos; };
    };
} Item00402BB0;
#else
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Vec3_00402BB0 pos;
} Marker00402BB0;

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Vec3_00402BB0 pos;
    /* 0x10 */ s32 region;
} Obj00402BB0;

typedef struct {
    /* 0x00 */ u8 pad0[0x54];
    /* 0x54 */ Vec3_00402BB0 pos;
} Item00402BB0;
#endif

s32 Cinema_IsIdle(void);
void COnScreen__SetBoss(void *a0, Obj00402BB0 *obj, void *a2);

void MantidQueen_UpdateIntro(Obj00402BB0 *obj, Item00402BB0 *item) {
    Marker00402BB0 *marker;

    if (Cinema_IsIdle()) {
        if (CScene__FindInstances(&gScene, 0x451, 9, 0x1D4D, &marker, 1)) {
            item->pos = marker->pos;
            obj->pos = marker->pos;
            obj->region = CScene__FindRegionAt(&gScene, &obj->pos);
        }
        CAI__SetMode(obj, item, 8);
        COnScreen__SetBoss(&gEngineApp_m_OnScreen, obj, "Queen");
    }
}
