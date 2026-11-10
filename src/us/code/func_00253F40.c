#include "common.h"
#include "structs/player.h"
#include "protos/game.h"

/* A looping sound: only its sound handle here. */
#ifdef TARGET_PORT
/* LoopSounds' node (SoundSourceNode, rw/abu_types.h): the sound id at N64 0xC follows a
 * pointer, so on the host it is at 0x14 */
struct CLoopingSoundData {
    s32 unk0;
    void *next;
    s32 value;
    s32 m_nHandle;
};
#else
struct CLoopingSoundData {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 m_nHandle;
};
#endif

void Sound__StopSoundId(s32);                            /* extern */
void LoopSounds_Stop(sptr, struct CLoopingSoundData *); /* extern */

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
