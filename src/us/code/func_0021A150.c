#include "common.h"
#include "protos/iset.h"
#include "protos/cache.h"

#ifdef TARGET_PORT
/* The port: the same through CGameObjectInstance (structs/game_object.h); the Actor view below
 * has the N64 offsets (the pointers before 0xA4 and in CAI make the host ones larger). */
#include "structs/game_object.h"
#include "structs/cache.h"

extern u8 D_800A5AC4[];     /* "anim object info" */

void CMtxF__TransformBox(void *, void *, void *);
void CAnimationState__Reset(void *);
s32 CGameObjectInstance__PickRandomAnim(CGameObjectInstance *, sptr, sptr);

void CGameObjectInstance__SetupAnimation(CGameObjectInstance *actor, s32 *a1, s32 a2, s32 anim) {
    CMemEntry *model;
    CMemEntry *set;
    s32 id;
    void *tree;
    void *setTree;
    s32 size;
    void *data;

    actor->m_dwFlags &= ~0x40000;
    id = CISet__GetROMAddress((sptr)a1, a2, actor->header.m_nObjType);
    model = CCache__Request(0, id, 0x18, 0, 0, 0, "anim object index", 1);
    if (model != NULL) {
        tree = model->data;
        actor->m_rpGeometryHeader = (void *)(sptr)CISet__GetROMAddressAndSize(tree, id, 1, &size);
        actor->m_rpGeometryHeaderSize = size;
        actor->m_rpObject = (void *)(sptr)CISet__GetROMAddressAndSize(tree, id, 0, &size);
        actor->m_rpObjectHeader = size;
        actor->m_rpAnims = (void *)(sptr)CISet__GetROMAddress((sptr)tree, id, 2);
        set = CCache__Request(0, (s32)(sptr)actor->m_rpObject, actor->m_rpObjectHeader, 0, 0, 0, (char *)D_800A5AC4, 1);
        if (set != NULL) {
            setTree = set->data;
            data = CISet__GetBlock(setTree, 0);
            ((void (*)(CGameObjectInstance *, void *))actor->m_AI.m_pfCalculateOrientationMatrix)(actor, &actor->m_AI);
            CMtxF__TransformBox(&actor->m_mfOrient, data, &actor->m_vMin);
            CAnimationState__Reset(&actor->m_CurrentAnimation);
            CAnimationState__Reset(&actor->m_BlendAnimation);
            actor->m_nAnims = ((u16 *) CISet__GetBlock((void *)(sptr)setTree, 1))[3];
            actor->m_dwFlags |= 0x40000;
            if (anim > 0) {
                anim = CGameObjectInstance__PickRandomAnim(actor, anim, -1);
            } else {
                anim = -anim;
            }
            if (anim == -1) {
                anim = 0;
            }
            actor->m_CurrentAnimation.m_nRequestedAnimIndex = anim;
            actor->m_CurrentAnimation.m_bCycleCompleted = 0;
            if (actor->m_CurrentAnimation.m_nAnimIndex != anim) {
                actor->m_CurrentAnimation.m_bAnimActive = 1;
            }
            actor->m_AI.m_ModeLastFrame = -1;
            actor->m_AI.m_AnimIndex = anim;
            CCache__Unlock(0, set);
        }
        CCache__Unlock(0, model);
    }
}
#else

/* Bind an actor to its model and animation set, then choose its first
 * animation: `anim` > 0 is an action to pick at random (CGameObjectInstance__PickRandomAnim),
 * otherwise -anim is the entry itself. */

typedef struct Actor Actor;

struct Actor {
    u8 pad0[0x2];
    u16 unk2;               /* 0x002 */
    u8 pad4[0x54];
    u8 unk58[0x4C];         /* 0x058 */
    s32 unkA4;              /* 0x0A4 */
    s32 unkA8;              /* 0x0A8 */
    s32 unkAC;              /* 0x0AC */
    s32 unkB0;              /* 0x0B0 */
    s32 unkB4;              /* 0x0B4 */
    u8 padB8[0x2];
    u16 setId;              /* 0x0BA */
    u8 unkBC[0x18];         /* 0x0BC */
    u32 flags;              /* 0x0D4 */
    u8 unkD8[0x4];          /* 0x0D8 */
    s16 curAnim;            /* 0x0DC */
    s16 nextAnim;           /* 0x0DE */
    u8 padE0[0x2];
    u8 unkE2;               /* 0x0E2 */
    u8 unkE3;               /* 0x0E3 */
    u8 padE4[0x8];
    u8 unkEC[0x54];         /* 0x0EC */
    u8 ai[0x35];            /* 0x140 */
    s8 unk175;              /* 0x175 */
    u8 pad176[0x90];
    s8 unk206;              /* 0x206 */
    u8 pad207[0x3D];
    void (*init)(Actor *, void *);  /* 0x244 */
};

extern u8 D_800A5AC4[];     /* "anim object info" */

void CMtxF__TransformBox(void *, void *, void *);
void CAnimationState__Reset(void *);
sptr CGameObjectInstance__PickRandomAnim(Actor *, sptr, sptr);

void CGameObjectInstance__SetupAnimation(Actor *actor, sptr a1, s32 a2, sptr anim) {
    s32 *model;
    s32 *set;
    s32 id;
    sptr tree;
    sptr setTree;
    void *data;

    actor->flags &= ~0x40000;
    id = CISet__GetROMAddress(a1, a2, actor->unk2);
    model = CCache__Request(0, id, 0x18, 0, 0, 0, (sptr) "anim object index", 1);
    if (model != NULL) {
        tree = *model;
        actor->unkAC = CISet__GetROMAddressAndSize(tree, id, 1, &actor->unkB4);
        actor->unkA4 = CISet__GetROMAddressAndSize(tree, id, 0, &actor->unkB0);
        actor->unkA8 = CISet__GetROMAddress(tree, id, 2);
        set = CCache__Request(0, actor->unkA4, actor->unkB0, 0, 0, 0, D_800A5AC4, 1);
        if (set != NULL) {
            setTree = *set;
            data = CISet__GetBlock(setTree, 0);
            actor->init(actor, actor->ai);
            CMtxF__TransformBox(actor->unk58, data, actor->unkBC);
            CAnimationState__Reset(actor->unkD8);
            CAnimationState__Reset(actor->unkEC);
            actor->setId = ((u16 *) CISet__GetBlock(setTree, 1))[3];
            actor->flags |= 0x40000;
            if (anim > 0) {
                anim = CGameObjectInstance__PickRandomAnim(actor, anim, -1);
            } else {
                anim = -anim;
            }
            if (anim == -1) {
                anim = 0;
            }
            actor->nextAnim = anim;
            actor->unkE2 = 0;
            if (actor->curAnim != anim) {
                actor->unkE3 = 1;
            }
            actor->unk175 = -1;
            actor->unk206 = anim;
            CCache__Unlock(0, set);
        }
        CCache__Unlock(0, model);
    }
}
#endif
