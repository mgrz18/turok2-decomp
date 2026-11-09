#include "common.h"
#include "structs/player.h"
#include "protos/game.h"

/* A looping sound: only its sound handle here. */
struct CLoopingSoundData {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 m_nHandle;
};

void Sound__StopSoundId(s32);                            /* extern */
void LoopSounds_Stop(s32, struct CLoopingSoundData *); /* extern */

/* Stops the player's weapon firing looping sound. */
void CPlayer__StopWeaponFiringSound(CPlayer *arg0) {
    struct CLoopingSoundData *temp_v0;

    temp_v0 = arg0->m_pLoopingSound_WeaponFiring;
    if (temp_v0 != NULL) {
        Sound__StopSoundId(temp_v0->m_nHandle);
    }
    LoopSounds_Stop(LoopSounds_Get(), arg0->m_pLoopingSound_WeaponFiring);
    arg0->m_pLoopingSound_WeaponFiring = NULL;
}
