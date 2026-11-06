/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025C524_de_2.c, func_8025C95C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/audio.h"

extern s32 CList__AddTail(CList *list, void *node);
extern s32 Sound__Play(s16 arg0, CVector3 position, s32 arg4, s32 arg5);
void *LoopSounds_Start(void *arg0, s32 arg1, CVector3 *arg2, void *arg3, s32 arg4) {
    extern void CList__Remove(void *arg0, void *arg1);
    SoundSourceNode *node;
    s32 result;
    node = ((SoundVoicePool *)arg0)->available.m_pHead;
    if (node != 0) {
        CList__Remove(arg0, node);
        CList__AddTail(&((SoundVoicePool *)arg0)->playing, node);
        node->state = -1;
        node->value = -1;
        result = Sound__Play((s16)arg1, *arg2,
                                (s32)arg3, arg4);
        node->value = result;
        sLoopSoundsLastStarted = result;
        node->state = arg1;
        node->position = *arg2;
        node->owner = arg3;
    }
    return node;
}
