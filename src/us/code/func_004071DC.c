#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/ai.h"

/* Just the AI callbacks this sets. */
#ifdef TARGET_PORT
/* the CAI's draw and effects callbacks at their host offsets (tools/port/hostview.py; round 10
 * y2): the N64 0x234/0x248 wrote the two pointers into other members of the 0x453 actors (the
 * Primagen's parts never got their draw/pose callbacks) */
#define T2_HOFF_Callbacks(m) __builtin_offsetof(CGameObjectInstance, m)
typedef struct {
    union {
        struct { u8 _hp_onDraw[T2_HOFF_Callbacks(m_AI.m_pfDraw)]; void *onDraw; }; /* N64 0x234 */
        struct { u8 _hp_onUpdate[T2_HOFF_Callbacks(m_AI.m_pfEffects)]; void *onUpdate; }; /* N64 0x248 */
    };
} Callbacks;
#else
typedef struct {
    /* 0x000 */ u8 pad000[0x234];
    /* 0x234 */ void *onDraw;
    /* 0x238 */ u8 pad238[0x10];
    /* 0x248 */ void *onUpdate;
} Callbacks;
#endif

extern void Primagen_Draw();
extern void Primagen_Effects();


void Primagen_UpdateMode0(CGameObjectInstance *obj, CAI *ai) {
    Callbacks *found[10];
    Callbacks *item;
    s32 n;
    s32 i;

    n = CScene__FindInstances(&gScene, 0x453, 9, -1, found, 10);
    for (i = 0; i < n; i++) {
        item = found[i];
        item->onDraw = Primagen_Draw;
        item->onUpdate = Primagen_Effects;
    }
    CAI__SetMode(obj, ai, 0x1C);
}
