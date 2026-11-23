#include "common.h"
#include "structs/sound.h"

sptr CAudioWorld__GetPitchEnvelope(CAudioWorld *arg0, s32 arg1) {
    return arg0->table2B74 + (arg1 * 0xC);
}
