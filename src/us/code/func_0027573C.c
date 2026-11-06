/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025D928_de_8.c, func_8025E04C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/game.h"

extern int Sound__PlayIndex2D(int arg0);
extern s32 CAudioWorld__GetForce2D(struct ContextRecAt2BC0 *arg0);
extern void CAudioWorld__SetForce2D(struct ContextRecAt2BC0 *arg0, s32 arg1);
extern s32 CAudioWorld__PlaySoundIndex(void *, s32, CVector3, s32);
extern int CAudioWorld__GetForce2D(struct ContextRecAt2BC0 *arg0);
extern void CAudioWorld__SetForce2D(struct ContextRecAt2BC0 *arg0, int arg1);
extern int CAudioWorld__PlaySoundIndex(void *arg0, int arg1, CVector3 arg2, int arg3);
int Sound__PlayIndex2D(int arg0) {
    extern signed char gAudioWorld;
    int temp_s0;
    int temp_s1;
    if (gEngineApp_m_Settings.unk1C8 != 0) {
        return -1;
    }
    {
        CVector3 vec;
        register float zero = (float)0;
        vec.z = zero;
        vec.y = zero;
        vec.x = zero;
        temp_s1 = CAudioWorld__GetForce2D(&gAudioWorld);
        CAudioWorld__SetForce2D(&gAudioWorld, 1);
        temp_s0 = CAudioWorld__PlaySoundIndex(&gAudioWorld, arg0, vec, 0);
        CAudioWorld__SetForce2D(&gAudioWorld, temp_s1);
    }
    return temp_s0;
}
