#include "common.h"
#include "structs/sound.h"
#include "protos/iset.h"

void CAudioWorld__GetVariantLayers(CAudioWorld *m, s16 arg1) {
    CISet__GetBlock(CISet__GetBlock(m->variants, arg1), 1);
}
