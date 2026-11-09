#include "common.h"
#include "m2c_macros.h"
#include "protos/ai.h"


void BlindOne_PhaseVulnerableInit(s32 *arg0) {
    s32 temp_a0;

    temp_a0 = *arg0;
    CAI__SetMode(temp_a0, temp_a0 + 0x140, 0);
}
