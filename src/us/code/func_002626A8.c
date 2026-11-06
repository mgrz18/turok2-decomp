#include "common.h"
#include "structs/game_object.h"
#include "globals/ai.h"

typedef void (*AICallback)(void *actor, void *ai);

void Turret_Step(void *actor, void *ai);
void Turret_Hurt(void *actor, void *ai);
void Turret_CalcOrientation(void *actor, void *ai);
void Turret_Effects(void *actor, void *ai);

/* AI init: installs this actor kind's table and callbacks. */
void Turret_Setup(void *actor, CAI *ai) {
    /* FAKEMATCH: the ROM builds Turret_Effects's address in $a0 (the dead
     * actor argument) while $v0 still holds Turret_CalcOrientation's; no natural
     * source shape tried (temporaries, inline setters, reusing the
     * parameter, store order, permuter) gives a second register other
     * than $v1, so the value is pinned to $a0. */
    register AICallback unk108 asm("$4");

    ai->m_pModeTable = gTurretModeTable;
    ai->m_pfStep = Turret_Step;
    ai->m_pfHurt = Turret_Hurt;
    ai->m_pfCalculateOrientationMatrix = Turret_CalcOrientation;
    unk108 = Turret_Effects;
    ai->m_pfEffects = unk108;
}
