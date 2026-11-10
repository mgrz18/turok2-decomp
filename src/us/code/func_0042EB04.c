/* CPlayer code: CPlayer__StopLoopingSounds, 0x0042EB04-0x0042EB4C (splat had it inside the data D_0042D50C). */
#include "common.h"
#include "structs/player.h"
#include "protos/game.h"

extern void LoopSounds_Stop(void *pool, sptr sound);

/* Stops the player's two looping sounds. */
void CPlayer__StopLoopingSounds(CPlayer *p) {
    LoopSounds_Stop(LoopSounds_Get(), p->m_pLoopingSound_WeaponBackground);
    p->m_pLoopingSound_WeaponBackground = 0;
    LoopSounds_Stop(LoopSounds_Get(), p->m_pLoopingSound_WeaponFiring);
    p->m_pLoopingSound_WeaponFiring = 0;
}
