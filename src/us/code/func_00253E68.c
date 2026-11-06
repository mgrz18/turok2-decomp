#include "common.h"
#include "structs/player.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 id;
} LoopSound;

s32 Sound__StopSoundId(s32);                         /* extern */
s32 LoopSounds_Stop(s32, void *);                 /* extern */
s32 LoopSounds_Get();                                /* extern */

void CPlayer__StopWeaponBackgroundSound(CPlayer *arg0) {
    LoopSound *temp_v0;

    temp_v0 = arg0->m_pLoopingSound_WeaponBackground;
    if (temp_v0 != NULL) {
        Sound__StopSoundId(temp_v0->id);
    }
    LoopSounds_Stop(LoopSounds_Get(), arg0->m_pLoopingSound_WeaponBackground);
    arg0->m_pLoopingSound_WeaponBackground = NULL;
}
