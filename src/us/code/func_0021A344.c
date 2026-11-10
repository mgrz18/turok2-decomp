#include "common.h"
#include "structs/vector.h"
#ifdef TARGET_PORT
#include "structs/game_object.h"
#endif
#include "globals/scene.h"
#include "protos/iset.h"
#include "protos/cache.h"
#include "protos/scene.h"

/* Advance an actor's animation (CGameObjectInstance__AdvanceAnimation) and move it by the root motion
 * that produced: the new position is taken back out, turned by the actor's
 * rotation, collided along (CGameObjectInstance__MoveWithCollision), then the animation is applied. */

typedef struct Actor Actor;

#ifdef TARGET_PORT
struct Actor {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Actor(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_pos[T2_HOFF_Actor(header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_region[T2_HOFF_Actor(header.m_pIntelligence)]; s32 *region; }; /* N64 0x14 */
        struct { u8 _hp_unk58[T2_HOFF_Actor(m_mfOrient)]; u8 unk58[0x40]; }; /* N64 0x58 */
        struct { u8 _hp_unkA4[T2_HOFF_Actor(m_rpObject)]; sptr unkA4; }; /* N64 0xA4: a ROM address */
        struct { u8 _hp_unkB0[T2_HOFF_Actor(m_rpObjectHeader)]; s32 unkB0; }; /* N64 0xB0 */
        struct { u8 _hp_unkBC[T2_HOFF_Actor(m_vMin)]; u8 unkBC[0x18]; }; /* N64 0xBC */
        struct { u8 _hp_flags[T2_HOFF_Actor(m_dwFlags)]; u32 flags; }; /* N64 0xD4 */
        struct { u8 _hp_ai[T2_HOFF_Actor(m_AI)]; u8 ai[0x54]; }; /* N64 0x140 */
        struct { u8 _hp_unk194[T2_HOFF_Actor(m_AI.m_vDesiredPos)]; u8 unk194[0xC]; }; /* N64 0x194 */
        struct { u8 _hp_init[T2_HOFF_Actor(m_AI.m_pfCalculateOrientationMatrix)]; void (*init)(Actor *, void *); }; /* N64 0x244 */
    };
};
#else
struct Actor {
    u8 pad0[0x4];
    CVector3 pos;               /* 0x004 */
    u8 pad10[0x4];
    s32 *region;            /* 0x014 */
    u8 pad18[0x40];
    u8 unk58[0x4C];         /* 0x058 */
    s32 unkA4;              /* 0x0A4 */
    u8 padA8[0x8];
    s32 unkB0;              /* 0x0B0 */
    u8 padB4[0x8];
    u8 unkBC[0x18];         /* 0x0BC */
    u32 flags;              /* 0x0D4 */
    u8 padD8[0x68];
    u8 ai[0x54];            /* 0x140 */
    u8 unk194[0xB0];        /* 0x194 */
    void (*init)(Actor *, void *);  /* 0x244 */
};
#endif

extern u8 D_800A5AC4[];     /* "anim object info" */

void CMtxF__TransformBox(void *, void *, void *);
void CVector3__Add(CVector3 *, CVector3 *, void *);
void CVector3__Subtract(CVector3 *, CVector3 *, CVector3 *);
void func_0021A500(Actor *);
void CGameObjectInstance__MoveWithCollision(Actor *, CVector3 *);
void CGameObjectInstance__AdvanceAnimation(Actor *);

void CGameObjectInstance__UpdateAnimation(Actor *actor) {
    CVector3 oldPos;
    CVector3 newPos;
    sptr *set;
    void *data;
    s32 *game;
    u32 flags;
    u32 moving;

    flags = actor->flags;
    if (!(flags & 0x40000)) {
        return;
    }
    game = &gScene;
    if (*game != 4 || (flags & 0x2000)) {
        moving = flags & 0x300000;
        oldPos = actor->pos;
        CGameObjectInstance__AdvanceAnimation(actor);
        newPos = actor->pos;
        actor->pos = oldPos;
        if (!moving && *actor->region != 0) {
            CVector3__Subtract(&newPos, &newPos, &oldPos);
            CVector3__Add(&newPos, &newPos, actor->unk194);
            CGameObjectInstance__MoveWithCollision(actor, &newPos);
            if (*actor->region == 1) {
                CScene__CheckAutoActivation(game, actor, &oldPos, &actor->pos);
            }
        }
        func_0021A500(actor);
        set = CCache__Request(0, actor->unkA4, actor->unkB0, 0, 0, 0, D_800A5AC4, 1);
        if (set != NULL) {
            data = CISet__GetBlock(*set, 0);
            actor->init(actor, actor->ai);
            CMtxF__TransformBox(actor->unk58, data, actor->unkBC);
            CCache__Unlock(0, set);
        }
    }
}
