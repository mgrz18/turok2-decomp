/* Second VM module copy of CPlayer__StopLoopingSounds (same source, linked 0x0042EA24). */
#include "common.h"
#include "structs/player.h"
#include "protos/game.h"

extern void LoopSounds_Stop(void *pool, sptr sound);

/* Stops the player's two looping sounds. */
void CPlayer__StopLoopingSounds_2(CPlayer *p) {
    LoopSounds_Stop(LoopSounds_Get(), p->m_pLoopingSound_WeaponBackground);
    p->m_pLoopingSound_WeaponBackground = 0;
    LoopSounds_Stop(LoopSounds_Get(), p->m_pLoopingSound_WeaponFiring);
    p->m_pLoopingSound_WeaponFiring = 0;
}
