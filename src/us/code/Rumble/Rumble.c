/* Boundary fixed in versions/cuts.us.txt: splat starts this function at 0x26E758 with two stray words
 * (`sw $v0,0x10($s0); j .L0023587C`); the real function is Rumble_GetLevelAt
 * (Controller_UpdateRumble calls it there). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/RENDER/8028469C_17.CPP, func_802856E0_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"

/* A sound/effect emitter attenuated by distance. */
typedef struct Emitter {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ struct Emitter *next;
    /* 0x08 */ u8 pad8[4];
    /* 0x0C */ CVector3 pos;
    /* 0x18 */ f32 radius;
    /* 0x1C */ f32 gain;
    /* 0x20 */ u8 curveA[0xC];
    /* 0x2C */ u8 curveB[0xC];
} Emitter;
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ Emitter *emitters;
    /* 0x18 */ u8 pad18[0x10];
    /* 0x28 */ f32 base;
} EmitterList;
u32 __osDisableInt(void);
void __osRestoreInt(u32 mask);
extern s32 Demo_GetPlayerCount(void);
extern s32 Cinema_PathPlaying(void);
extern f32 func_00298470(f32);          /* sqrtf */
extern f32 RumbleCurve_GetValue(void *curve);
#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) < (b) ? (b) : (a))

static __inline__ f32 distance(Emitter *e, CVector3 a) {
    return func_00298470((a.x - e->pos.x) * (a.x - e->pos.x) + (a.y - e->pos.y) * (a.y - e->pos.y)
                         + (a.z - e->pos.z) * (a.z - e->pos.z));
}

/* Summed gain of a list's emitters heard from a point, clamped to [0, 1]. */
f32 Rumble_GetLevelAt(EmitterList *list, CVector3 pos) {
    Emitter *e;
    s32 mask;
    f32 total;
    f32 dist;
    f32 falloff;
    f32 a;

    total = list->base;
    mask = __osDisableInt();
    if (Demo_GetPlayerCount() || Cinema_PathPlaying()) {
        total = 0.0f;
    } else {
        for (e = list->emitters; e != NULL; e = e->next) {
            dist = distance(e, pos);
            if (dist == 0.0f || e->radius == 0.0f) {
                falloff = 1.0f;
            } else {
                falloff = 0.0f;
                if (dist < e->radius) {
                    falloff = 1.0f - dist / e->radius;
                }
            }
            falloff *= e->gain;
            a = RumbleCurve_GetValue(e->curveA);
            total += falloff * a * RumbleCurve_GetValue(e->curveB);
        }
    }
    __osRestoreInt(mask);
    return MAX(MIN(total, 1.0f), 0.0f);
}

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802846C8_de_17.c, func_802858B0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "structs/world.h"
#include "globals/scene.h"
#include "protos/scene.h"

void RumbleCurve_Init(void *slot, void *curve);
void Rumble_Init(NodeCode8028469C *arg0, Clip *arg1, CVector3 arg2, f32 arg5, f32 arg6, void **arg7);

void Rumble_Init(NodeCode8028469C *arg0, Clip *arg1, CVector3 arg2,
                   f32 arg5, f32 arg6, void **arg7) {
    arg0->key = arg1;
    arg0->position = arg2;
    arg0->value0 = arg5;
    arg0->value1 = arg6;
    arg0->owner = arg7;
    if (arg7 != 0) {
        *arg7 = arg0;
    }
    RumbleCurve_Init(&arg0->amplitudeHandle,
                  CScene__GetCurve(&gScene, arg1->mode));
    RumbleCurve_Init(&arg0->lifetimeHandle,
                  CScene__GetCurve(&gScene, arg1->frames));
}

/* Only the field read here: where to clear a word. */
typedef struct {
    /* 0x00 */ u8 pad0[0x38];
    /* 0x38 */ s32 *flag;
} FlagOwner;

void Rumble_ClearLink(FlagOwner *arg0) {
    s32 *temp_v0;

    temp_v0 = arg0->flag;
    if (temp_v0 != NULL) {
        *temp_v0 = 0;
    }
}

#include "structs/rumble.h"

/* Sets the pointer an effect clears when it ends (Rumble_Start's owner). Round 23: the member;

 * the old N64 view wrote an s32 at 0x38, amplitude.unk4 on the host. */
void Rumble_SetLink(RumbleEffect *effect, RumbleEffect **owner) {
    effect->owner = owner;
}

#include "m2c_macros.h"

s32 RumbleCurve_Advance(RumbleCurve *curve);

s32 Rumble_IsFinished(u8 *arg0) {
    s32 r = 0;

    if (RumbleCurve_Advance(arg0 + 0x20) != 0 || RumbleCurve_Advance(arg0 + 0x2C) != 0) {
        r = 1;
    }
    return r;
}
f32 Rumble_GetLevelOne(f32 *arg0, u32 arg1, u32 arg2, u32 arg3);
extern f32 RumbleCurve_GetValue(void *arg0);

f32 Rumble_GetLevelOne(f32 *arg0, u32 arg1, u32 arg2, u32 arg3) {
    f32 magnitude;
    f32 dx;
    f32 dy;
    f32 dz;
    f32 limit;
    f32 factor;
    f32 first;
    dx = *(f32 *)&arg1 - arg0[3];
    dy = *(f32 *)&arg2 - arg0[4];
    dz = *(f32 *)&arg3 - arg0[5];
    magnitude = func_00298470((dx * dx) + (dy * dy) + (dz * dz));
    if ((magnitude == 0.0f) || ((limit = arg0[6]) == 0.0f)) {
        factor = 1.0f;
    } else {
        factor = 0.0f;
        if (magnitude < limit) {
            factor = 1.0f - (magnitude / limit);
        }
    }
    factor *= arg0[7];
    first = RumbleCurve_GetValue(&arg0[8]);
    return factor * first * RumbleCurve_GetValue(&arg0[11]);
}

#include "protos/list.h"
#ifdef TARGET_PORT
#include "structs/controller.h"
_Static_assert(sizeof(((ControllerProfile *) 0)->rumblePool) == __builtin_offsetof(RumblePool, baseLevel), "rumblePool");
_Static_assert(__builtin_offsetof(ControllerProfile, rumbleBase) - __builtin_offsetof(ControllerProfile, rumblePool) == __builtin_offsetof(RumblePool, baseLevel), "rumbleBase");
_Static_assert(sizeof(((ControllerProfile *) 0)->rumbleNodes[0]) == sizeof(RumbleEffect), "rumbleNodes");
_Static_assert(__builtin_offsetof(ControllerProfile, rumbleNodes) - __builtin_offsetof(ControllerProfile, rumblePool) >= sizeof(RumblePool), "rumble pool size");
#endif

/* Puts every one of the count effects on the pool's free list. */
void RumblePool_Init(RumblePool *pool, RumbleEffect *effects, s32 count) {
    u32 mask;
    s32 i;

    mask = __osDisableInt();
    CList__Construct(&pool->free, 0, sizeof(void *));
    CList__Construct(&pool->active, 0, sizeof(void *));
    i = 0;
    if (count > 0) {
        do {
            CList__AddTail(&pool->free, effects);
            i++;
            effects++;
        } while (i < count);
    }
    pool->baseLevel = 0;
    __osRestoreInt(mask);
}

#ifdef TARGET_PORT
/* Host body (round 7 v2): the pool and its effects as RumblePool / RumbleEffect
 * (structs/rumble.h); the N64 views put the free list's head at 0x0, the active list at 0x14
 * and the curves at 0x20/0x2C of the effect (pointers before them on the host). */
#include "structs/rumble.h"

RumbleEffect *Rumble_Start(RumblePool *pool, Clip *key, CVector3 position, f32 value0, f32 value1, RumbleEffect **owner) {
    s32 mask;
    RumbleEffect *node;

    mask = __osDisableInt();
    node = (RumbleEffect *) pool->free.m_pHead;
    if (node != 0) {
        CList__Remove(&pool->free, node);
        CList__AddTail(&pool->active, node);
        node->clip = key;
        node->position = position;
        node->radius = value0;
        node->scale = value1;
        node->owner = owner;
        if (owner != 0) {
            *owner = node;
        }
        RumbleCurve_Init(&node->amplitude, CScene__GetCurve(&gScene, key->mode));
        RumbleCurve_Init(&node->lifetime, CScene__GetCurve(&gScene, key->frames));
    }
    __osRestoreInt(mask);
    return node;
}
#else
struct Node_func_80285B64_de;
struct Node_func_80285B64_de {
    char pad0[8];
    Clip *key;
    CVector3 position;
    f32 value0;
    f32 value1;
    char pad20[0x18];
    struct Node_func_80285B64_de **owner;
};
struct Node_func_80285B64_de;
typedef struct Node_func_80285B64_de Node_func_80285B64_de;
struct Node_func_80285B64_de;
struct Pool_func_80285B64_de;
struct Pool_func_80285B64_de {
    struct Node_func_80285B64_de *free;
    char pad4[0x10];
    struct Node_func_80285B64_de *active;
};
struct Pool_func_80285B64_de;
typedef struct Pool_func_80285B64_de Pool_func_80285B64_de;
static __inline__ void bind(Node_func_80285B64_de *node, s32 offset, s32 id) {
    RumbleCurve_Init((char *)node + offset, CScene__GetCurve(&gScene, id));
}
static __inline__ void init(Node_func_80285B64_de *node, Clip *key, CVector3 position, f32 value0, f32 value1, Node_func_80285B64_de **owner) {
    node->key = key;
    node->position = position;
    node->value0 = value0;
    node->value1 = value1;
    node->owner = owner;
    if (owner != 0) {
        *owner = node;
    }
    bind(node, 0x20, key->mode);
    bind(node, 0x2C, key->frames);
}
Node_func_80285B64_de *Rumble_Start(Pool_func_80285B64_de *pool, Clip *key, CVector3 position, f32 value0, f32 value1, Node_func_80285B64_de **owner) {
    s32 mask;
    Node_func_80285B64_de *node;
    mask = __osDisableInt();
    node = pool->free;
    if (node != 0) {
        CList__Remove(pool, node);
        CList__AddTail(&pool->active, node);
        init(node, key, position, value0, value1, owner);
    }
    __osRestoreInt(mask);
    return node;
}
#endif

/* Returns the effects whose amplitude or lifetime curve has run out to the
 * free list (Rumble_IsFinished, inlined). */
void RumblePool_FreeFinished(RumblePool *pool) {
    u32 mask = __osDisableInt();
    RumbleEffect *effect;
    RumbleEffect *next;
    s32 finished;

    for (effect = pool->active.m_pHead; effect != NULL; effect = next) {
        next = effect->next;
        finished = 0;
        if (RumbleCurve_Advance(&effect->amplitude) || RumbleCurve_Advance(&effect->lifetime)) finished = 1;
        if (finished) {
            if (effect->owner != NULL) *effect->owner = NULL;
            CList__Remove(&pool->active, effect);
            CList__AddTail(&pool->free, effect);
        }
    }
    __osRestoreInt(mask);
}

/* Returns every active effect to the free list. */
void RumblePool_FreeAll(RumblePool *pool) {
    RumbleEffect *effect;
    RumbleEffect *next;
    u32 mask;

    mask = __osDisableInt();
    effect = pool->active.m_pHead;
    if (effect != NULL) {
        do {
            next = effect->next;
            CList__Remove(&pool->active, effect);
            CList__AddTail(&pool->free, effect);
            effect = next;
        } while (effect != NULL);
    }
    pool->baseLevel = 0;
    __osRestoreInt(mask);
}
