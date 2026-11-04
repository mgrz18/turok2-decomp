#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

/* AI state: turn toward the target; after 100 steps, or when the target is within 460.8 and less than 20
 * degrees off, go to anim state 0x16. */

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ void *actor;
    /* 0x08 */ f32 angle;
    /* 0x0C */ u8 padC[0x40 - 0xC];
    /* 0x40 */ f32 dist;
} Target;

typedef struct {
    /* 0x00 */ Target a;
    /* 0x44 */ Target b;
} Targets;

extern void func_00246108(void *obj, void *ai, Targets *t, s32 f1, s32 f2);
extern void func_00245BAC(void *obj, void *ai, f32 turnRate, f32 angle);
extern void func_00243414(void *obj, void *ai, s32 state);

void func_0040B80C(void *obj, void *ai) {
    Targets t;
    MotherBossState *state;

    func_00246108(obj, ai, &t, 1, 1);
    func_00245BAC(obj, ai, 0.261799425f, t.b.angle);
    state = &D_8012FB70;
    if (++state->counter > 100 ||
        (t.b.actor != NULL && t.a.actor != NULL && t.b.dist < 460.8f &&
         (t.a.angle < 0.0f ? -t.a.angle <= 0.34906590f : t.a.angle <= 0.34906590f))) {
        func_00243414(obj, ai, 0x16);
    }
}
