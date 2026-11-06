#include "common.h"
#include "structs/boss.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x234];
    /* 0x234 */ void *onEvent;
    /* 0x238 */ u8 pad1[0x248 - 0x238];
    /* 0x248 */ void *onUpdate;
} Actor00402ADC;

extern u8 D_800F7078[];
s32 CScene__FindInstances(void *world, s32 a1, s32 a2, s32 a3, Actor00402ADC **out, s32 max);
void MantidQueen_Init(void *state, s32 a1);
void CAI__SetMode(s32 a0, s32 a1, s32 a2);
s32 MantidQueen_Draw(s32 a0, s32 a1, s32 a2);
void EnemyAI_HeadNodeCallback(void);

void MantidQueen_EnterStart(s32 a0, s32 a1) {
    Actor00402ADC *found[2];
    Actor00402ADC *actor;
    s32 count;
    s32 i;

    count = CScene__FindInstances(D_800F7078, 0x451, 9, -1, found, 2);
    for (i = 0; i < count; i++) {
        actor = found[i];
        actor->onEvent = MantidQueen_Draw;
        actor->onUpdate = EnemyAI_HeadNodeCallback;
    }
    MantidQueen_Init(&D_8012F3A0, a0);
    CAI__SetMode(a0, a1, 1);
}
