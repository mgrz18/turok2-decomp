#include "common.h"
#include "structs/boss.h"
#include "globals/boss.h"
#include "globals/scene.h"
#include "protos/scene.h"
#include "protos/ai.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x234];
    /* 0x234 */ void *onEvent;
    /* 0x238 */ u8 pad1[0x248 - 0x238];
    /* 0x248 */ void *onUpdate;
} Actor00402ADC;

void MantidQueen_Init(void *state, sptr a1);
s32 MantidQueen_Draw(sptr a0, sptr a1, sptr a2);
void EnemyAI_HeadNodeCallback(void);

void MantidQueen_EnterStart(sptr a0, sptr a1) {
    Actor00402ADC *found[2];
    Actor00402ADC *actor;
    s32 count;
    s32 i;

    count = CScene__FindInstances(&gScene, 0x451, 9, -1, found, 2);
    for (i = 0; i < count; i++) {
        actor = found[i];
        actor->onEvent = MantidQueen_Draw;
        actor->onUpdate = EnemyAI_HeadNodeCallback;
    }
    MantidQueen_Init(&gMantidQueen, a0);
    CAI__SetMode(a0, a1, 1);
}
