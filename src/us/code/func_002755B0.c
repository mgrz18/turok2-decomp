/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025D928_de_8.c, func_8025DEC0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/audio.h"

extern int gEngineApp_m_Settings_unk1C8;
extern s32 CAudioWorld__PlaySoundPitched(struct ContextRecAt2BC0 *arg0, s32 arg1, CVector3 arg2,
                         s32 arg3, s32 arg4, f32 arg5);
s32 Sound__PlayPitched(s32 arg0, CVector3 arg1, s32 arg4, s32 arg5, f32 arg6) {
    if (gEngineApp_m_Settings_unk1C8 != 0) {
        return -1;
    }
    return CAudioWorld__PlaySoundPitched(&gAudioWorld, (s16)arg0, arg1, arg4, arg5, arg6);
}
