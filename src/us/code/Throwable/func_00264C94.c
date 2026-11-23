#include "common.h"
#include "structs/game_object.h"
#include "globals/ai.h"

s32 func_00264994(s32, s32);
#ifdef TARGET_PORT
void Throwable_CalcOrientation(void *obj, void *ai);
#endif
s32 Throwable_Collision(sptr, sptr);
s32 Throwable_Step(sptr, sptr);

void Throwable_Setup(sptr arg0, CAI *ai) {
    ai->m_pModeTable = gThrowableModeTable;
    ai->m_pfStep = Throwable_Step;
    ai->m_pfCollision = Throwable_Collision;
#ifdef TARGET_PORT
    /* func_00264994 + 0xC is Throwable_CalcOrientation (func_002649A0.c: a wrong splat cut); the
     * host address + 0xC is inside data (STABILITY.md S14: SetupAnimation called a data address). */
    ai->m_pfCalculateOrientationMatrix = Throwable_CalcOrientation;
#else
    ai->m_pfCalculateOrientationMatrix = (void *) (func_00264994 + 0xC);
#endif
}
