/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025C524_de_2.c, func_8025CA24_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/audio.h"

extern s32 CList__AddTail(CList *list, void *node);
extern void Sound__Stop(s32 arg0);
s32 LoopSounds_Stop(void *arg0, void *arg1) {
    extern void CList__Remove(void *, s32);
    void *var_s0;
    if (arg1 == 0) {
        return 0;
    }
    var_s0 = ((SoundVoicePool *)arg0)->playing.m_pHead;
    if (var_s0 != 0) {
        do {
            if (var_s0 == arg1) {
                Sound__Stop(((SoundSourceNode *)var_s0)->value);
                sLoopSoundsLastStopped = ((SoundSourceNode *)var_s0)->value;
                CList__Remove(&((SoundVoicePool *)arg0)->playing, (s32)var_s0);
                CList__AddTail(arg0, var_s0);
                return 1;
            }
            var_s0 = ((SoundSourceNode *)var_s0)->next;
        } while (var_s0 != 0);
    }
    return 0;
}
