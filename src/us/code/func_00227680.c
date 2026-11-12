#include "common.h"
#include "structs/vector.h"
#include "globals/scene.h"
#include "globals/engine.h"

#ifdef TARGET_PORT
typedef struct Pickup {
/* the view at the host offsets of CPooledLevelActor (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Pickup(m) __builtin_offsetof(CPooledLevelActor, m)
    union {
        struct { u8 _hp_pos[T2_HOFF_Pickup(m_Actor.header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_def[T2_HOFF_Pickup(m_Actor.header.m_pIntelligence)]; void *def; }; /* N64 0x14 */
        struct { u8 _hp_vel[T2_HOFF_Pickup(m_Actor.m_vVelocity)]; CVector3 vel; }; /* N64 0x18 */
        struct { u8 _hp_boundsMin[T2_HOFF_Pickup(m_Actor.m_Bounds.m_vMin)]; CVector3 boundsMin; }; /* N64 0x14C */
        struct { u8 _hp_boundsMax[T2_HOFF_Pickup(m_Actor.m_Bounds.m_vMax)]; CVector3 boundsMax; }; /* N64 0x158 */
        struct { u8 _hp_timer[T2_HOFF_Pickup(m_Timer)]; f32 timer; }; /* N64 0x19C */
    };
} Pickup;
#else
typedef struct Pickup {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[0x4];
    /* 0x014 */ void *def;
    /* 0x018 */ CVector3 vel;
    /* 0x024 */ u8 pad24[0x14C - 0x24];
    /* 0x14C */ CVector3 boundsMin;
    /* 0x158 */ CVector3 boundsMax;
    /* 0x164 */ u8 pad164[0x19C - 0x164];
    /* 0x19C */ f32 timer;
} Pickup;
#endif

extern u8 D_800F1EF0[];

void CLevelActor__Update(Pickup *obj);
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define Collision_MoveInstance Collision_MoveInstance_f3
#endif
void Collision_MoveInstance(Pickup *obj, f32 x, f32 y, f32 z, void *arg4);

/* Counts the timer down, moves the object (CLevelActor__Update), stops it unless its definition is
 * gLevelActorIntelligence, places it at its position and refreshes its bounding box, 81.92 each way. */
void CPooledLevelActor__Update(Pickup *obj) {
    CVector3 prev;

    if (obj->timer > 0.0f) {
        obj->timer -= gFrameIncrement;
        prev = obj->pos;
    }
    CLevelActor__Update(obj);
    if (obj->def != &gLevelActorIntelligence) {
        obj->vel.x = 0.0f;
        obj->vel.y = 0.0f;
        obj->vel.z = 0.0f;
    }
    Collision_MoveInstance(obj, obj->pos.x, obj->pos.y, obj->pos.z, D_800F1EF0);
    obj->boundsMin.x = obj->pos.x - 81.92f;
    obj->boundsMin.y = obj->pos.y - 81.92f;
    obj->boundsMin.z = obj->pos.z - 81.92f;
    obj->boundsMax.x = obj->pos.x + 81.92f;
    obj->boundsMax.y = obj->pos.y + 81.92f;
    obj->boundsMax.z = obj->pos.z + 81.92f;
}
