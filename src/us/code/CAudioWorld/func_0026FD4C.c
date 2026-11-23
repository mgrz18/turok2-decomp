#include "common.h"
#include "globals/audiomgr.h"
u32 __amDMA(s32 offset, s32 length);

s32 *__amDmaNew(AMDMAState **arg0) {
    if (gAudioDmaState.initialized == 0) {
        gAudioDmaState.firstFree = gAudioDmaBuffers;
        gAudioDmaState.firstUsed = NULL;
        gAudioDmaState.initialized = 1U;
    }
    *arg0 = &gAudioDmaState;
    return &__amDMA;
}
