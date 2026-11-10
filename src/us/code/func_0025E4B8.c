#include "common.h"

#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define CMtxF__RotateX CMtxF__RotateX_i
#endif
s32 CMtxF__RotateX(s32, s32);                    /* extern */
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define CMtxF__RotateZ CMtxF__RotateZ_i
#endif
s32 CMtxF__RotateZ(sptr, s32);                    /* extern */

typedef struct {
    /* 0x000 */ u8 pad0[0x164];
    /* 0x164 */ s32 unk164;
    /* 0x168 */ s32 unk168;
} Owner0025E4B8;

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s8 min;
    /* 0x19 */ s8 max;
} Range0025E4B8;

typedef struct {
    /* 0x00 */ Owner0025E4B8 *owner;
    /* 0x04 */ u8 pad4[0x14 - 0x4];
    /* 0x14 */ Range0025E4B8 *range;
    /* 0x18 */ u8 pad18[0x90 - 0x18];
    /* 0x90 */ s32 value;
} Effect0025E4B8;

#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/pose_host.h"
/* Host body: the pose context (structs/pose_host.h) and the instance by member (N64
 * 0x164/0x168: m_AI.m_HeadTurn/m_HeadTilt) and the float CMtxF__RotateX/Z directly; the
 * matrix pointer-wide. */
#undef CMtxF__RotateX
#undef CMtxF__RotateZ
void CMtxF__RotateX_f(void *m, f32 a) __asm__("CMtxF__RotateX"); /* the float definitions */
void CMtxF__RotateZ_f(void *m, f32 a) __asm__("CMtxF__RotateZ");
void EnemyAI_HeadNodeCallback(sptr arg0, T2PoseContext *ctx) {
    Range0025E4B8 *range = (Range0025E4B8 *) ctx->info;
    s32 bone = ctx->bone;

    if ((bone >= range->min) && (range->max >= bone)) {
        CMtxF__RotateZ_f((void *) arg0, ctx->obj->m_AI.m_HeadTurn);
        CMtxF__RotateX_f((void *) arg0, ctx->obj->m_AI.m_HeadTilt);
    }
}
#else
void EnemyAI_HeadNodeCallback(s32 arg0, Effect0025E4B8 *arg1) {
    s32 temp_v1;
    Range0025E4B8 *temp_a0;

    temp_a0 = arg1->range;
    temp_v1 = arg1->value;
    if ((temp_v1 >= temp_a0->min) && (temp_a0->max >= temp_v1)) {
        CMtxF__RotateZ(arg0, arg1->owner->unk164);
        CMtxF__RotateX(arg0, arg1->owner->unk168);
    }
}
#endif
