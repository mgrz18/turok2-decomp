/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025D928_de_8.c, func_8025DF34_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/audio.h"

extern s32 CAudioWorld__PlaySound(void *, s32, CVector3, s32, s32);
extern s32 CAudioWorld__GetForce2D(struct ContextRecAt2BC0 *arg0);
extern void CAudioWorld__SetForce2D(struct ContextRecAt2BC0 *arg0, s32 arg1);
extern int CAudioWorld__GetForce2D(struct ContextRecAt2BC0 *arg0);
extern void CAudioWorld__SetForce2D(struct ContextRecAt2BC0 *arg0, int arg1);
extern int gEngineApp_m_Settings_unk1C8;
s32 Sound__Play2D(s32 arg0) {
    s32 temp_s0;
    s32 temp_s1;
    if (gEngineApp_m_Settings_unk1C8 != 0) {
        return -1;
    }
    {
        CVector3 vec;
        register f32 zero = 0.0f;
        vec.z = zero;
        vec.y = zero;
        vec.x = zero;
        temp_s1 = CAudioWorld__GetForce2D(&gAudioWorld);
        CAudioWorld__SetForce2D(&gAudioWorld, 1);
        temp_s0 = CAudioWorld__PlaySound(&gAudioWorld, (s16)arg0, vec, 0, -1);
        CAudioWorld__SetForce2D(&gAudioWorld, temp_s1);
    }
    return temp_s0;
}
