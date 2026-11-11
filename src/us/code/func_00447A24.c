#include "common.h"
#include "m2c_macros.h"
#include "protos/ai.h"
#ifdef TARGET_PORT
#include "structs/game_object.h"
#endif


void BlindOne_PhaseVulnerableInit_2(s32 *arg0) {
    sptr temp_a0;

#ifdef TARGET_PORT
    /* arg0 holds the boss actor pointer-wide; its CAI at the host offset (N64 +0x140) */
    temp_a0 = *(sptr *) arg0;
    CAI__SetMode(temp_a0, (sptr) &((CGameObjectInstance *) temp_a0)->m_AI, 0);
#else
    temp_a0 = *arg0;
    CAI__SetMode(temp_a0, temp_a0 + 0x140, 0);
#endif
}
