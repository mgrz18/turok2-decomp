#include "common.h"

typedef struct {
    s32 x0;
    s32 x4;
    s32 x8;
    s32 xC;
} Quad; /* passed by value: starts in $a3, the rest on the stack */

extern void Sound__PlaySpeech(s32);

void ImpactEvent_PlaySound(s32 arg0, s32 arg1, s32 arg2, Quad q) {
    Sound__PlaySpeech(q.xC);
}
