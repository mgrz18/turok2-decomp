/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SYSTEM/802624A0_10.CPP, func_80262E88_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/list.h"
#include "protos/list.h"

struct SceneEffectPool;
/* Turok 2 effect record: 0x298 bytes (Rage Wars 0x2F8), most fields 0x2C-0x60 earlier. */
typedef struct Effect_func_80262E88_de Effect_func_80262E88_de;
struct Effect_func_80262E88_de {
    char pad0[0xBC];
    f32 min[3];         /* 0xBC (0xE8 in Rage Wars) */
    f32 max[3];         /* 0xC8 */
    s32 flags;          /* 0xD4 (0x100) */
    char padD8[0x144 - 0xD8];
    s32 busy;           /* 0x144 (0x174) */
    char pad148[0x28C - 0x148];
    struct Effect_func_80262E88_de *next; /* 0x28C (0x2EC) */
    s32 *ref;           /* 0x290 (0x2F0) */
    char pad294[4];
};
struct SceneEffectPool {
    Effect_func_80262E88_de effects[0x20];
    CList freeEffects;   /* 0x5300 */
    CList activeEffects; /* 0x5314 */
};
extern f32 D_800F5A18;
extern void CScene__FileInstance(s32, Effect_func_80262E88_de *);
static __inline__ s32 overlaps(Effect_func_80262E88_de *effect, f32 *max) {
    if (max[0] > effect->min[0] && max[-3] < effect->max[0] && max[2] > effect->min[2] &&
        max[-1] < effect->max[2] && max[1] > effect->min[1] && max[-2] < effect->max[1]) {
        return 1;
    }
    return 0;
}
void CInstancePool__UpdateVisibility(struct SceneEffectPool *scene, s32 arg1) {
    Effect_func_80262E88_de *effect;
    Effect_func_80262E88_de *next;
    f32 *max;
    effect = scene->activeEffects.m_pHead;
    if (effect == 0) {
        return;
    }
    do {
        max = (&D_800F5A18 + 1);
    next_effect:
        next = effect->next;
        if (overlaps(effect, max)) {
            CScene__FileInstance(arg1, effect);
            if (effect->ref != 0 && effect->busy == 0) {
                *effect->ref -= 1;
                effect->ref = 0;
            }
        } else {
            effect->busy = 0;
            effect->flags &= ~0x200;
            if (effect->ref != 0) {
                *effect->ref -= 1;
            }
            CList__Remove(&scene->activeEffects, effect);
            CList__AddTail(&scene->freeEffects, effect);
        }
        effect = next;
    } while (0);
    if (effect != 0) {
        goto next_effect;
    }
}
