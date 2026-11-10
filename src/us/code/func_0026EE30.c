#include "common.h"

#ifdef TARGET_PORT
#include "structs/rumble.h"
/* Host body: the curve is a pointer (RumbleCurve.curve), pointer-wide on the host. */
void RumbleCurve_Init(RumbleCurve *arg0, void *arg1) {
    arg0->curve = arg1;
    arg0->unk4 = 0;
    arg0->unk8 = 0;
}
#else
/* Three words: a value and two counters this clears. */
typedef struct {
    /* 0x00 */ s32 value;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
} Triple;

void RumbleCurve_Init(Triple *arg0, sptr arg1) {
    arg0->value = arg1;
    arg0->unk4 = 0;
    arg0->unk8 = 0;
}
#endif
