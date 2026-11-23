/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00285B34.c, func_80285B64_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/vector.h"
#include "globals/scene.h"
#include "protos/list.h"
#include "protos/scene.h"

#ifdef TARGET_PORT
/* Host body (round 7 v2): the pool and its effects as RumblePool / RumbleEffect
 * (structs/rumble.h); the N64 views put the free list's head at 0x0, the active list at 0x14
 * and the curves at 0x20/0x2C of the effect (pointers before them on the host). */
#include "structs/rumble.h"
typedef struct Clip {
    s16 mode;
    s16 frames;
} Clip;
extern s32 __osDisableInt(void);
extern void __osRestoreInt(s32 mask);
extern void RumbleCurve_Init(RumbleCurve *slot, void *curve);

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
struct Clip;
struct Clip {
    s16 mode;
    s16 frames;
};
struct Clip;
typedef struct Clip Clip;
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
extern s32 __osDisableInt(void);
extern void __osRestoreInt(s32 mask);
extern void RumbleCurve_Init(void *slot, sptr handle);
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
