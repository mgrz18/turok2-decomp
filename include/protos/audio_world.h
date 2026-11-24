/* What CAudioWorld (src/us/code/CAudioWorld) calls outside its own module, declared once for
 * its files (round 28, part q2). The sound-player modules' definitions still take Rage Wars
 * views of these objects; the types here are the ones CAudioWorld hands them (structs/sound.h). */

#ifndef PROTOS_AUDIO_WORLD_H
#define PROTOS_AUDIO_WORLD_H

#include "common.h"
#include "structs/vector.h"
#include "structs/sound.h"
#include "structs/camera.h"

CCamera *CCameraPool__GetNearestCamera(CCameraPool *pool, CVector3 *point);
void SndVoicePool_Update(void *pool);
void SndQueue_Update(void *queue);
void SndSeq_Update(void *seq);
void SndSeq_UpdateFade(void *seq);
void SpeechPlayer_Update(void *fx);
s32 SpeechPlayer_Play(void *fx, s32 group, f32 scale);
void SpeechPlayer_Stop(void *fx);
s32 BinaryRange(void *array, s32 count, u32 key, s32 *first, s32 *last);
s32 Random_Next(void);
void *VM_Alloc(s32 romAddr, s32 unused, s32 size);   /* defined returning s32 (the address) */

#endif
