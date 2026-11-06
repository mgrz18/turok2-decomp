#include "common.h"
#include "structs/sound.h"

void SndVoicePool_StopSound(SndVoicePool *pool);                         /* extern */
void SndQueue_CancelSoundId(SndQueue *queue, s32);                /* extern */

void CAudioWorld__StopSound(CAudioWorld *mgr, s32 arg1) {
    SndVoicePool_StopSound(&mgr->pool);
    SndQueue_CancelSoundId(&mgr->queue, arg1);
}
