#include "common.h"
#include "structs/camera.h"
#include "structs/sound.h"
#include "protos/list.h"

/* The free list at 0, the used list at 0x14. */
typedef struct {
    /* 0x00 */ CList m_Free;
    /* 0x14 */ CList m_Used;
} SoundSlotPool;

s32 Sound__Stop(s32);                         /* extern */

void LoopSounds_StopAll(SoundSlotPool *arg0) {
    SndSource *temp_s0;
    SndSource *var_s1;

    var_s1 = (SndSource *) arg0->m_Used.m_pHead;
    if (var_s1 != NULL) {
        do {
            temp_s0 = var_s1->next;
            Sound__Stop(var_s1->handle);
            var_s1->sound.w = -1;
            var_s1->handle = -1;
            CList__Remove(&arg0->m_Used, var_s1);
            CList__AddTail(arg0, var_s1);
            var_s1 = temp_s0;
        } while (var_s1 != NULL);
    }
}
