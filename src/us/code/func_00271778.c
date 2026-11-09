#include "common.h"
#include "structs/sound.h"

sptr CAudioWorld__GetVolumeEnvelope(CAudioWorld *arg0, s32 arg1) {
    return arg0->table2B70 + (arg1 * 0xC);
}
