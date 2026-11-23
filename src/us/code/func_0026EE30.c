#include "common.h"

#include "structs/rumble.h"
/* Host body: the curve is a pointer (RumbleCurve.curve), pointer-wide on the host. */
void RumbleCurve_Init(RumbleCurve *arg0, void *arg1) {
    arg0->curve = arg1;
    arg0->unk4 = 0;
    arg0->unk8 = 0;
}
