#ifndef STRUCTS_POSE_HOST_H
#define STRUCTS_POSE_HOST_H

/* TARGET_PORT only: the bone-posing context CGameObjectInstance__BuildBoneMatrices
 * (func_0021CDAC.c) fills and CGameObjectInstance__PoseBone (func_0021B95C.c) passes to
 * the instance's m_AI.m_pfEffects callback, at its HOST layout. On the N64 every view of
 * it is a local struct at fixed offsets (obj 0x00, out 0x04, info 0x14, bone 0x90...);
 * on the host the pointers are 8 bytes, so the callbacks that read it
 * (EnemyAI_HeadNodeCallback...) use this twin. func_0021B95C.c checks it against
 * PoseBone's own view (_Static_assert). */
#ifdef TARGET_PORT

#include "common.h"

typedef struct T2PoseKeys {
    void *joints;
    void *frames;
    void *posTable;
    void *rotTable;
    s32 rotFrame0;
    s32 rotFrame1;
    s32 posFrame0;
    s32 posFrame1;
    f32 blend;
} T2PoseKeys;

typedef struct T2PoseContext {
    struct CGameObjectInstance *obj; /* N64 0x00 */
    u32 *out;                        /* N64 0x04: the bone matrices (Mtx) */
    f32 (*bones)[4][4];              /* N64 0x08 */
    void *anims;                     /* N64 0x0C */
    void *file;                      /* N64 0x10 */
    sptr info;                       /* N64 0x14: "anim object info" block 0 (AnimObjInfo) */
    void *channel;                   /* N64 0x18 */
    void *blendChannel;              /* N64 0x1C */
    T2PoseKeys keys;                 /* N64 0x20 */
    T2PoseKeys blendKeys;            /* N64 0x44 */
    s32 visible;                     /* N64 0x68 */
    s32 blend;
    s32 moving;
    s32 f74;
    s32 f78;
    s32 f7C;
    s32 f80;
    s32 f84;
    s32 f88;
    u8 *boneData;                    /* N64 0x8C */
    s32 bone;                        /* N64 0x90 */
} T2PoseContext;

#endif /* TARGET_PORT */
#endif
