#include "common.h"
#include "structs/game_object.h"

/* AI state: turn towards the target with the 10000 animation, the turn rate
 * depending on the stance; release the grabbed actor near the end of it. */

typedef struct {
    /* 0x00 */ u8 pad0[0xD8];
    /* 0xD8 */ f32 animFrame;
    /* 0xDC */ s16 anim;
    /* 0xDE */ s16 padDE;
    /* 0xE0 */ s16 animLength;
} Self;

s32 func_00219F74(void *self, s32 anim, s32 arg2);
void func_00246108(void *self, CAI *ai, CAITargets *out, s32 f1, s32 f2);
void func_00245BAC(void *self, CAI *ai, f32 angle, f32 arg3);
void func_00243414(void *self, void *ai, s32 mode);

void func_0025F3A4(Self *self, CAI *ai) {
    CAITargets t;
    u8 *held = (u8 *) ai->m_Enemy.m_pGrabbed;
    f32 rate;
    s32 anim;

    anim = func_00219F74(self, 10000, -1);
    if (self->anim == anim) {
        func_00246108(self, ai, &t, 0, 1);
        rate = 0.26179942f;
        if (ai->m_ModeBefore == 1 || ai->m_ModeBefore == 2) {
            rate = 1.0471977f;
        }
        func_00245BAC(self, ai, rate * 1.5f, t.m_Sight.m_Angle);
        if (held != NULL && self->animLength * 0.7f <= self->animFrame) {
            func_00243414(held, held + 0x140, 2);
            ai->m_Enemy.m_pGrabbed = NULL;
        }
    }
    if (ai->m_AnimRepeatedCount) {
        func_00243414(self, ai, 10);
    }
}
