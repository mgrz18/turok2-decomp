#include "common.h"
#include "structs/game_object.h"

typedef void (*AICallback)(void *actor, void *ai);

extern u8 D_800B1DE0[];
void func_002627B8(void *actor, void *ai);
void func_00262BE0(void *actor, void *ai);
void func_00262A1C(void *actor, void *ai);
void func_00262B58(void *actor, void *ai);

/* AI init: installs this actor kind's table and callbacks. */
void func_002626A8(void *actor, CAI *ai) {
    /* FAKEMATCH: the ROM builds func_00262B58's address in $a0 (the dead
     * actor argument) while $v0 still holds func_00262A1C's; no natural
     * source shape tried (temporaries, inline setters, reusing the
     * parameter, store order, permuter) gives a second register other
     * than $v1, so the value is pinned to $a0. */
    register AICallback unk108 asm("$4");

    ai->m_pModeTable = D_800B1DE0;
    ai->m_pfStep = func_002627B8;
    ai->m_pfHurt = func_00262BE0;
    ai->m_pfCalculateOrientationMatrix = func_00262A1C;
    unk108 = func_00262B58;
    ai->m_pfEffects = unk108;
}
