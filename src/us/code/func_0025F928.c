#include "common.h"
#include "structs/game_object.h"

/* CAI callback: raises the 0xC8 level by 10 (at most 255) and sets the timer
 * from it, 0..255 -> 240..120. The float temporaries (240 passed through two
 * variables) are what gives the ROM its alternating $f0/$f1 allocation. */
void func_0025F928(void *self, CAI *ai) {
    s32 level = ai->m_Aggression;
    f32 t;
    f32 scale;
    f32 v;

    scale = 1.0f / 255.0f;
    level += 10;
    if (level >= 0x100) {
        level = 0xFF;
    }
    ai->m_Aggression = level;
    v = ai->m_Aggression * scale;
    t = v;
    t *= -120.0f;
    v = 240.0f;
    scale = v;
    v = t + scale;
    ai->m_ConditionTimer = v;
}
