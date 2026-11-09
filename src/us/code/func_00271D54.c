#include "common.h"
#include "structs/sound.h"
#include "protos/iset.h"


void CAudioWorld__GetVariantCue(CAudioWorld *arg0, s16 arg1) {
    CISet__GetBlock(CISet__GetBlock(arg0->variants, arg1), 0);
}
