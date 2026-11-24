#include "common.h"
#include "protos/enemy_ai.h"

#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s8 min;
    /* 0x19 */ s8 max;
} Range0025E4B8;

typedef struct {
    /* 0x00 */ CGameObjectInstance *owner;
    /* 0x04 */ u8 pad4[0x14 - 0x4];
    /* 0x14 */ Range0025E4B8 *range;
    /* 0x18 */ u8 pad18[0x90 - 0x18];
    /* 0x90 */ s32 value;
} Effect0025E4B8;

#ifdef TARGET_PORT
#include "structs/pose_host.h"
/* Host body: the port's pose context (structs/pose_host.h) in place of the N64 one; the matrix
 * pointer-wide. */
void EnemyAI_HeadNodeCallback(sptr arg0, T2PoseContext *ctx) {
    Range0025E4B8 *range = (Range0025E4B8 *) ctx->info;
    s32 bone = ctx->bone;

    if ((bone >= range->min) && (range->max >= bone)) {
        CMtxF__RotateZ((void *) arg0, ctx->obj->m_AI.m_HeadTurn);
        CMtxF__RotateX((void *) arg0, ctx->obj->m_AI.m_HeadTilt);
    }
}
#else
void EnemyAI_HeadNodeCallback(s32 arg0, Effect0025E4B8 *arg1) {
    s32 temp_v1;
    Range0025E4B8 *temp_a0;

    temp_a0 = arg1->range;
    temp_v1 = arg1->value;
    if ((temp_v1 >= temp_a0->min) && (temp_a0->max >= temp_v1)) {
        CMtxF__RotateZ((void *) arg0, arg1->owner->m_AI.m_HeadTurn);
        CMtxF__RotateX((void *) arg0, arg1->owner->m_AI.m_HeadTilt);
    }
}
#endif
