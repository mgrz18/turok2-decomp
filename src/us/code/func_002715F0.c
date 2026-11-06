#include "common.h"
#include "structs/sound.h"

void SndVoicePool_StopByFlags(SndVoicePool *pool);                         /* extern */
void SndQueue_CancelFlags(SndQueue *queue, s32);                /* extern */

void CAudioWorld__StopSoundsByFlags(CAudioWorld *mgr, s32 arg1) {
    SndVoicePool_StopByFlags(&mgr->pool);
    SndQueue_CancelFlags(&mgr->queue, arg1);
}
