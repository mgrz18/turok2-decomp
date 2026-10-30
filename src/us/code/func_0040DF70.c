#include "common.h"

extern f32 D_800B6D28; /* frame time scale */
extern s32 D_80119890; /* current player */

typedef struct AIState40DF70 AIState40DF70;

typedef struct {
    /* 0x00 */ char pad0[0x8];
    /* 0x08 */ void (*update)(void *obj, AIState40DF70 *ai);
} AIProcs40DF70;

struct AIState40DF70 {
    /* 0x000 */ char pad0[0x4];
    /* 0x004 */ s32 active;
    /* 0x008 */ char pad8[0x28];
    /* 0x030 */ AIProcs40DF70 *procs;
    /* 0x034 */ char pad34[0x44];
    /* 0x078 */ s32 target;
    /* 0x07C */ f32 targetDist;
    /* 0x080 */ s32 target80;
    /* 0x084 */ char pad84[0x4];
    /* 0x088 */ s32 target88;
    /* 0x08C */ char pad8C[0x8C];
    /* 0x118 */ f32 timer;
};

void func_0040DF70(void *obj, AIState40DF70 *ai) {
    if (ai->active != 0 && D_80119890 != 0) {
        ai->target80 = D_80119890;
        ai->target88 = D_80119890;
        ai->target = D_80119890;
        ai->targetDist = 150.0f;
    }
    if (ai->procs != NULL && ai->procs->update != NULL) {
        ai->procs->update(obj, ai);
    }
    ai->timer = (ai->timer - D_800B6D28 < 0.0f) ? 0.0f : ai->timer - D_800B6D28;
}
