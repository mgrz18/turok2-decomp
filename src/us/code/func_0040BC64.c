#include "common.h"

/* AI update for the module's actor: runs the AI step and the current anim's callback; in phase 0, a target
 * closer than 1228.8 makes the D_8010FEB4 actors drop their health; then updates the module (func_00409784). */

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ void (*callback)(void *obj, void *ai);
} Anim;

typedef struct {
    /* 0x00 */ u8 pad0[0x30];
    /* 0x30 */ Anim *anim;
} CAI;

typedef struct {
    /* 0x000 */ u8 pad0[0x65C];
    /* 0x65C */ s32 phase;
} State;

extern State D_8012FB70;
extern void func_0025AD90(void *obj, CAI *ai, f32 pitchOffset);
extern f32 func_0040BD60(void *obj, void **found);
extern void func_0040BD10(void);
extern void func_00409784(State *state);

void func_0040BC64(void *obj, CAI *ai) {
    State *state = &D_8012FB70;
    void *found;
    f32 dist;

    func_0025AD90(obj, ai, 0.0f);
    if (ai->anim != NULL && ai->anim->callback != NULL) {
        ai->anim->callback(obj, ai);
    }
    if (state->phase == 0) {
        dist = func_0040BD60(obj, &found);
        if (found != NULL && dist < 1228.7999f) {
            func_0040BD10();
        }
    }
    func_00409784(&D_8012FB70);
}
