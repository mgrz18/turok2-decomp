#include "common.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "protos/ai.h"

typedef struct {
    f32 x, y, z;
} Vec3_00402FA0;

#ifdef TARGET_PORT
#include "structs/game_object.h"
/* the views at the host offsets (tools/port/hostview.py; round 10 y2): the actor's position,
 * velocity and current animation (frame, index), the CAI's sight target and
 * m_AnimRepeatedCount. The N64 views read other bytes on the host: the push never ended (the
 * repeat count) and never pushed. */
#define T2_HOFF_Obj00402FA0(m) __builtin_offsetof(CGameObjectInstance, m)
#define T2_HOFF_Item00402FA0(m) __builtin_offsetof(CAI, m)
typedef struct {
    union {
        struct { u8 _hp_pos[T2_HOFF_Obj00402FA0(header.m_vPos)]; Vec3_00402FA0 pos; }; /* N64 0x4 */
        struct { u8 _hp_vel[T2_HOFF_Obj00402FA0(m_vVelocity)]; Vec3_00402FA0 vel; }; /* N64 0x18 */
        struct { u8 _hp_time[T2_HOFF_Obj00402FA0(m_CurrentAnimation.m_AnimationFrame)]; f32 time; }; /* N64 0xD8 */
        struct { u8 _hp_anim[T2_HOFF_Obj00402FA0(m_CurrentAnimation.m_nAnimIndex)]; s16 anim; }; /* N64 0xDC */
    };
} Obj00402FA0;
#else
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Vec3_00402FA0 pos;
    /* 0x10 */ u8 pad0[0x18 - 0x10];
    /* 0x18 */ Vec3_00402FA0 vel;
    /* 0x24 */ u8 pad1[0xD8 - 0x24];
    /* 0xD8 */ f32 time;
    /* 0xDC */ s16 anim;
} Obj00402FA0;
#endif

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ Vec3_00402FA0 pos;
} Target00402FA0;

#ifdef TARGET_PORT
typedef struct {
    union {
        struct { u8 _hp_target[T2_HOFF_Item00402FA0(m_pSightTarget)]; Target00402FA0 *target; }; /* N64 0x80 */
        struct { u8 _hp_count[T2_HOFF_Item00402FA0(m_AnimRepeatedCount)]; s8 count; }; /* N64 0xC7 */
    };
} Item00402FA0;
#else
typedef struct {
    /* 0x00 */ u8 pad0[0x80];
    /* 0x80 */ Target00402FA0 *target;
    /* 0x84 */ u8 pad1[0xC7 - 0x84];
    /* 0xC7 */ s8 count;
} Item00402FA0;
#endif

sptr CGameObjectInstance__PickRandomAnim(Obj00402FA0 *obj, sptr a1, sptr a2);
void CVector3__Subtract(Vec3_00402FA0 *out, Vec3_00402FA0 *from, Vec3_00402FA0 *to);

void MantidQueen_UpdatePush(Obj00402FA0 *obj, Item00402FA0 *item) {
    Vec3_00402FA0 dir;
    sptr anim;
    WaveBossState *s = &gMantidQueen;

    anim = CGameObjectInstance__PickRandomAnim(obj, 0x1004, -1);
    if (obj->anim == anim) {
        if (s->pushed == 0 && obj->time >= 10.0f) {
            s->pushed = 1;
            if (item->target != NULL) {
                CVector3__Subtract(&dir, &item->target->pos, &obj->pos);
                obj->vel.x += dir.x * 0.75f;
                obj->vel.z += dir.z * 0.75f;
            }
        }
        if (obj->time >= 20.0f) {
            obj->vel.x = 0.0f;
            obj->vel.z = 0.0f;
        }
    }
    if (item->count != 0) {
        CAI__SetMode(obj, item, 8);
    }
}
