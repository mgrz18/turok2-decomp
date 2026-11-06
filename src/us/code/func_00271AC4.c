/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80258800_de.c, func_80258F10_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern int SpeechPlayer_Play(void *arg0, int arg1, float arg2, int arg3);
int CAudioWorld__PlaySpeech(ContextRecAt2BC0 *arg0, int arg1) {
    float var_f0 = (1.0f);
    float temp_f1 = arg0->blue;
    if (!(var_f0 < temp_f1)) {
        var_f0 = temp_f1;
    }
    return SpeechPlayer_Play(&arg0->dialog[0], arg1, var_f0, 0x40);
}
