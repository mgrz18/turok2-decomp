#include "common.h"

s32 SndVoicePool_ClearFollow(s32);                         /* extern */

void CAudioWorld__DetachAllVoices(sptr arg0) {
    SndVoicePool_ClearFollow(arg0 + 0x1DB8);
}
