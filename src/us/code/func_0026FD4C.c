#include "common.h"

extern s32 gAudioDmaBuffers;
typedef struct {
    /* 0x0 */ u8 initialized;
    /* 0x1 */ u8 pad1[3];
    /* 0x4 */ s32 unk4;
    /* 0x8 */ s32 *data;
} Unk800F2790;
extern Unk800F2790 gAudioDmaState;
extern s32 __amDMA;

s32 *__amDmaNew(Unk800F2790 **arg0) {
    if (gAudioDmaState.initialized == 0) {
        gAudioDmaState.data = &gAudioDmaBuffers;
        gAudioDmaState.unk4 = 0;
        gAudioDmaState.initialized = 1U;
    }
    *arg0 = &gAudioDmaState;
    return &__amDMA;
}
