#include "common.h"
#include "structs/game_object.h"

/* CAI reaction (like func_0025CAC8 case 11, but setting the delay): raises the
 * intensity by 20 (at most 255) and sets the reaction delay to 75..60 frames. */
void func_0025FCB8(void *actor, CAI *ai) {
    s32 v = ai->m_Aggression;
    f32 level;

    v += 20;
    if (v >= 0x100) {
        v = 0xFF;
    }
    ai->m_Aggression = v;
    level = ai->m_Aggression;
    level *= (1.0f / 255);
    level = level * -15.0f + 75.0f;
    ai->m_ConditionTimer = level;
}
