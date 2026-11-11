#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "globals/engine.h"

/* Per-part callback of the module's actors: in part 0, fades the actor's two blend values in with the module
 * object's anim state (9: first value up to 1; 0x13: after a delay, second value up to 3 then back down to 1;
 * from 30 on: second value 1); the other parts apply one of the values to the part. */

#ifdef TARGET_PORT
#include "structs/pose_host.h"

/* the host layouts (round 9 x2): the boss's m_AI.m_Mode (N64 0x174) and the bone-posing context
 * (structs/pose_host.h: the actor at 0, the bone at N64 0x90). The N64 views read other bytes on
 * the host: the parts never grew in after phase 0 nor pulsed in the phase 1 cinema. */
typedef struct {
    u8 pad0[__builtin_offsetof(CGameObjectInstance, m_AI.m_Mode)];
    s8 animState;
} ModuleObj;

typedef struct {
    CGameObjectInstance *actor;
    u8 pad4[__builtin_offsetof(T2PoseContext, bone) - sizeof(CGameObjectInstance *)];
    u32 part;
} PartInfo;
_Static_assert(__builtin_offsetof(PartInfo, part) == __builtin_offsetof(T2PoseContext, bone), "PartInfo");
#else
typedef struct {
    /* 0x000 */ u8 pad0[0x174];
    /* 0x174 */ s8 animState;
} ModuleObj;

typedef struct {
    /* 0x00 */ CGameObjectInstance *actor;
    /* 0x04 */ u8 pad4[0x90 - 0x4];
    /* 0x90 */ u32 part;
} PartInfo;
#endif

extern void CMtxF__ScaleAxes(void *arg0, f32 r, f32 g, f32 b);

void MotherBoss_PartEffects(void *arg0, PartInfo *info) {
    CGameObjectInstance *actor = info->actor;
    ModuleObj *obj;
    f32 v;
    f32 t;
    f32 dt;

    switch (info->part) {
        case 0:
            obj = gMotherBoss.obj;
            if (obj->animState == 9) {
                v = actor->m_AI.m_Mother.m_GrowScale + gFrameIncrement * 0.01f;
                if (v > 1.0f) {
                    v = 1.0f;
                }
                actor->m_AI.m_Mother.m_GrowScale = v;
            }
            if (obj->animState == 0x13) {
                t = sMotherBossFadeTimer;
                if (t < 8.8f) {
                    sMotherBossFadeTimer = t + gFrameIncrement * (1.0f / 15);
                } else if (sMotherBossCinemaStarted == 0) {
                    dt = gFrameIncrement;
                    actor->m_AI.m_Mother.m_PulseScale += dt * 0.4f;
                    if (actor->m_AI.m_Mother.m_PulseScale > 3.0f) {
                        sMotherBossCinemaStarted = 1;
                    }
                    sMotherBossFadeTimer = t + dt * (1.0f / 15);
                } else {
                    v = actor->m_AI.m_Mother.m_PulseScale - gFrameIncrement * 0.12f;
                    if (v < 1.0f) {
                        v = 1.0f;
                    }
                    actor->m_AI.m_Mother.m_PulseScale = v;
                }
            }
            if (obj->animState >= 30) {
                actor->m_AI.m_Mother.m_PulseScale = 1.0f;
            }
            break;
        case 21:
        case 26:
        case 31:
        case 36:
        case 46:
        case 51:
        case 56:
        case 61:
            CMtxF__ScaleAxes(arg0, actor->m_AI.m_Mother.m_GrowScale, actor->m_AI.m_Mother.m_GrowScale, actor->m_AI.m_Mother.m_GrowScale);
            break;
        case 89:
        case 93:
        case 97:
        case 101:
        case 105:
        case 109:
            CMtxF__ScaleAxes(arg0, actor->m_AI.m_Mother.m_PulseScale, actor->m_AI.m_Mother.m_PulseScale, actor->m_AI.m_Mother.m_PulseScale);
            break;
    }
}
