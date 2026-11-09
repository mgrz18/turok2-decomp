#include "common.h"
#include "globals/engine.h"
#include "protos/list.h"

s32 LevelEvent_Fire(s32);


/* A timed list entry: counts down and is freed at zero. */
typedef struct TimedNode {
    /* 0x00 */ struct TimedNode *prev;
    /* 0x04 */ struct TimedNode *next;
    /* 0x08 */ u8 *item;               /* item[0xE] & 2: timed */
    /* 0x0C */ f32 timer;
} TimedNode;

typedef struct {
    /* 0x0000 */ u8 pad0[0x1180];
    /* 0x1180 */ TimedNode *activeHead;  /* CList at 0x1180 */
    /* 0x1184 */ u8 pad1184[0x1194 - 0x1184];
    /* 0x1194 */ TimedNode *freeHead;    /* CList at 0x1194 */
} TimedPool;

void CScene__UpdateDelayedEvents(TimedPool *arg0) {
    f32 temp_f1;
    s32 var_s1;
    s32 var_v1;
    u8 *temp_a0;
    TimedNode *temp_s2;
    TimedNode *var_s0;

    var_s0 = arg0->activeHead;
    var_s1 = 0;
    if (var_s0 != NULL) {
        do {
            temp_a0 = var_s0->item;
            temp_s2 = var_s0->next;
            var_v1 = var_s1;
            if (temp_a0[0xE] & 2) {
                temp_f1 = var_s0->timer - gFrameIncrement;
                var_s0->timer = temp_f1;
                if (temp_f1 <= 0.0f) {
                    var_v1 = 1;
                    var_s1 = 1;
                }
            }
            if (var_v1 != 0) {
                LevelEvent_Fire((sptr) temp_a0);
            }
            if (var_s1 != 0) {
                CList__Remove(&arg0->activeHead, var_s0);
                CList__AddHead(&arg0->freeHead, var_s0);
            }
            var_s0 = temp_s2;
            var_s1 = 0;
        } while (var_s0 != NULL);
    }
}
