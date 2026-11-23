#include "common.h"
#include "globals/audio.h"
#include "globals/audiomgr.h"
#include "protos/ultra.h"


s32 alUnlink(void *);                      /* alUnlink */
s32 alLink(void *, void *);              /* alLink */

/* Waits for this frame's audio DMAs and frees the buffers not used lately. */
void __clearAudioDMA(void) {
    s32 sp10;
    u32 var_s0;
    AMDMABuffer *temp_s1;
    AMDMABuffer *var_s0_2;

    var_s0 = 0;
    sp10 = 0;
    if (nextDMA != 0) {
        do {
            osRecvMesg(&gAudioDmaMessageQ, &sp10, 1);
            var_s0 += 1;
        } while (var_s0 < (u32) nextDMA);
    }
    var_s0_2 = gAudioDmaState.firstUsed;
    if (var_s0_2 != NULL) {
        do {
            temp_s1 = (AMDMABuffer *) var_s0_2->node.next;
            if ((u32) (var_s0_2->lastFrame + 1) < (u32) audFrameCt) {
                if (gAudioDmaState.firstUsed == var_s0_2) {
                    gAudioDmaState.firstUsed = temp_s1;
                }
                alUnlink(var_s0_2);
                if (gAudioDmaState.firstFree == NULL) {
                    gAudioDmaState.firstFree = var_s0_2;
                    var_s0_2->node.next = NULL;
                    var_s0_2->node.prev = 0;
                    goto block_11;
                }
                alLink(var_s0_2, gAudioDmaState.firstFree);
                var_s0_2 = temp_s1;
            } else {
block_11:
                var_s0_2 = temp_s1;
            }
        } while (var_s0_2 != NULL);
    }
    nextDMA = 0;
    audFrameCt += 1;
}
