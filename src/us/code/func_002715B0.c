#include "common.h"

s32 SndVoicePool_StopByOwner(s32);                         /* extern */
s32 SndQueue_CancelOwner(sptr, s32);                /* extern */

void CAudioWorld__StopSoundsByOwner(sptr arg0, s32 arg1) {
    SndVoicePool_StopByOwner(arg0 + 0x1DB8);
    SndQueue_CancelOwner(arg0 + 0x138, arg1);
}
