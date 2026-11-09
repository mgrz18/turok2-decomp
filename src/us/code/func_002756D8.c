/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025D928_de_8.c, func_8025DFE8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/game.h"
#include "globals/audio.h"

extern s32 CAudioWorld__PlaySoundIndex(void *, s32, CVector3, s32);
extern int CAudioWorld__PlaySoundIndex(void *arg0, int arg1, CVector3 arg2, int arg3);
#ifdef TARGET_PORT
int T2Port_AudioReady(void);
#endif
s32 Sound__PlayIndex(s32 arg0, CVector3 arg1, s32 arg2) {
#ifdef TARGET_PORT /* no sound bank on the host yet: audio off (port/game/t2audio.c) */
    if (!T2Port_AudioReady()) {
        return 0;
    }
#endif
    if (gEngineApp_m_Settings.unk1C8 != 0) {
        return -1;
    }
    return CAudioWorld__PlaySoundIndex(&gAudioWorld, arg0, arg1, arg2);
}
