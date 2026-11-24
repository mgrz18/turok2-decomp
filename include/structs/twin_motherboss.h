/* Local types that vm1/MotherBoss and its vm2 copy both define (one layout): one definition
 * for both copies (tools/struct_census.py). */
#ifndef STRUCTS_TWIN_MOTHERBOSS_H
#define STRUCTS_TWIN_MOTHERBOSS_H

#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"
#include "structs/collision.h"
#include "structs/vector.h"
#include "structs/level_actor.h"
#include "structs/player.h"
#include "structs/pick.h"
#include "structs/game.h"
#include "structs/light.h"
#include "structs/model.h"

typedef struct {
    /* 0x00 */ CVector3 pos;
    /* 0x0C */ s32 type;
} AnimEvent;
typedef struct {
    s16 a;
    s16 b;
    u8 c[4];
} __attribute__((aligned(4))) Part;
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
typedef struct {
    /* 0x00 */ s16 upper;
    /* 0x02 */ s16 lower;
    /* 0x04 */ u8 extra[4];
} __attribute__((aligned(4))) ActionParams;

#endif
