/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025D928_de_8.c, func_8025DFE8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 CAudioWorld__PlaySoundIndex(void *, s32, CVector3, s32);
extern int CAudioWorld__PlaySoundIndex(void *arg0, int arg1, CVector3 arg2, int arg3);
extern int D_8011ACA0;
s32 Sound__PlayIndex(s32 arg0, CVector3 arg1, s32 arg2) {
    extern s32 gAudioWorld;
    if (D_8011ACA0 != 0) {
        return -1;
    }
    return CAudioWorld__PlaySoundIndex(&gAudioWorld, arg0, arg1, arg2);
}
