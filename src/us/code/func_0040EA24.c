#include "common.h"
#include "m2c_macros.h"
#include "protos/ai.h"


#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/boss.h"
/* host body (round 10 y2): gBlindOne.boss by member and its CAI (N64 + 0x140): the boss pointer
 * read as an s32 crashed CAI__SetMode as the vulnerable phase began */
void BlindOne_PhaseVulnerableInit(BlindOneState *arg0) {
    CAI__SetMode(arg0->boss, &arg0->boss->m_AI, 0);
}
#else
void BlindOne_PhaseVulnerableInit(s32 *arg0) {
    sptr temp_a0;

    temp_a0 = *arg0;
    CAI__SetMode(temp_a0, temp_a0 + 0x140, 0);
}
#endif
