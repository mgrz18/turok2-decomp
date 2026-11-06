/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025D928_de_8.c, func_8025DE54_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/game.h"
#include "globals/audio.h"

extern s32 CAudioWorld__PlaySound(void *, s32, CVector3, s32, s32);
s32 Sound__Play(s16 arg0, CVector3 arg1, s32 arg4, s32 arg5) {
    if (gEngineApp_m_Settings.unk1C8 != 0) {
        return -1;
    }
    return CAudioWorld__PlaySound(&gAudioWorld, arg0, arg1, arg4, arg5);
}
