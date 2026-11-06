#include "common.h"
#include "structs/game_object.h"
#include "globals/scene.h"

/* Just the AI callbacks this sets. */
typedef struct {
    /* 0x000 */ u8 pad000[0x234];
    /* 0x234 */ void *onDraw;
    /* 0x238 */ u8 pad238[0x10];
    /* 0x248 */ void *onUpdate;
} Callbacks;

extern s32 CScene__FindInstances(void *world, s32 key0, s32 key1, s32 key2, Callbacks **out, s32 max);
extern void CAI__SetMode(CGameObjectInstance *obj, CAI *ai, s32 mode);
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
