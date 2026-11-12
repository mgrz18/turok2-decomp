/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SOUND/8022C894_4.CPP, func_8022DB04_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#ifdef TARGET_PORT
#include "common.h"
#include "structs/player.h"
#else
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
#endif
extern float func_002974C0(float x);
extern float func_002982F0(float x);
struct Object126;
typedef struct Object126 Object126;
struct Actor126;
typedef struct Actor126 Actor126;
struct Actor126;
struct Actor126 {         /* Turok 2 offsets (Rage Wars in comments) */
    u8 pad0[0xD4];
    s32 flags;            /* 0x0D4 (0x100) */
    u8 padD8[0xCC];
    f32 value1A4;         /* 0x1A4 (0x1D4) */
    u8 pad1A8[0x862];
    s16 stateA0A;         /* 0xA0A (0x652) */
    u8 padA0C[0xC0];
    f32 offsetACC;        /* 0xACC (0x728) */
};
struct Object126;
struct Object126 {
    u8 pad0[0x18];
    f32 x;                /* 0x18 (0x1C) */
    u8 pad1C[4];
    f32 y;                /* 0x20 (0x24) */
    u8 pad24[0x2C];
    f32 angle;            /* 0x50 (0x6C): yaw */
};
#ifndef TARGET_PORT
extern void PlayerShock_Enter(Actor126 *actor, Object126 *arg1);
#endif
#ifdef TARGET_PORT
/* The port (round 11 C26): the player and the body by member. The N64 views (Actor126: 0xD4 the
 * flags, 0x1A4 m_AI.m_ConditionTimer, 0xA0A m_ModeBefore, 0xACC m_vHeadRotOffset.y; Object126:
 * 0x18/0x20 the velocity's x/z, 0x50 m_RotY) all missed on the host: the shock never set its
 * timer (PlayerShock_Update reads it by member) nor the 0x800000 flag. */
void PlayerShock_Enter(CPlayer *actor, CGameObjectInstance *object) {
    f32 object_angle;
    f32 angle;
    f32 sine;
    f32 cosine;

    actor->m_CharacterActor.m_dwFlags |= 0x800000;
    actor->m_CharacterActor.m_AI.m_ConditionTimer = 52.5f;
    if (actor->m_ModeBefore == 14) {
        object_angle = object->m_RotY;
        angle = 6.2831855f;
        sine = func_002982F0(object_angle + angle);
        cosine = func_002974C0(object->m_RotY + angle);
        sine *= 384.0f;
        cosine *= 384.0f;
        object->m_vVelocity.x += sine;
        object->m_vVelocity.z += cosine;
        object->m_RotY += actor->m_vHeadRotOffset.y;
        actor->m_vHeadRotOffset.y = 0.0f;
    }
}
#else
void PlayerShock_Enter(Actor126 *actor, Object126 *arg1) {
    Object126 *object;
    f32 angle;
    f32 object_angle;
    f32 sine;
    f32 cosine;
    actor->flags |= 0x800000;
    actor->value1A4 = 52.5f;
    if (actor->stateA0A == 14) {
        object = arg1;
        object_angle = object->angle;
        angle = 6.2831855f;
        sine = func_002982F0(object_angle + angle);
        cosine = func_002974C0(object->angle + angle);
        sine *= 384.0f;
        cosine *= 384.0f;
        object->x += sine;
        object->y += cosine;
        object->angle += actor->offsetACC;
        actor->offsetACC = 0.0f;
    }
}
#endif
