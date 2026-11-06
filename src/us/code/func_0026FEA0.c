#include "common.h"

/* An audio DMA buffer (libultra sample code's DMABuffer). */
typedef struct DMABuffer {
    /* 0x00 */ struct DMABuffer *next;  /* ALLink */
    /* 0x04 */ struct DMABuffer *prev;
    /* 0x08 */ s32 startAddr;
    /* 0x0C */ u32 lastFrame;
} DMABuffer;

typedef struct {
    /* 0x0 */ DMABuffer *firstUsed;
    /* 0x4 */ DMABuffer *firstFree;
} DMAState;

s32 alUnlink(void *);                      /* alUnlink */
s32 alLink(void *, void *);              /* alLink */
s32 osRecvMesg(s32 *, s32 *, s32);   /* extern */
extern u32 D_800B2660;          /* audio frame */
extern u32 D_800B2664;          /* DMAs issued this frame */
extern s32 D_800F2778;          /* DMA message queue */
extern DMAState D_800F2794;

/* Waits for this frame's audio DMAs and frees the buffers not used lately. */
void __clearAudioDMA(void) {
    s32 sp10;
    u32 var_s0;
    DMABuffer *temp_s1;
    DMABuffer *var_s0_2;

    var_s0 = 0;
    sp10 = 0;
    if (D_800B2664 != 0) {
        do {
            osRecvMesg(&D_800F2778, &sp10, 1);
            var_s0 += 1;
        } while (var_s0 < (u32) D_800B2664);
    }
    var_s0_2 = D_800F2794.firstUsed;
    if (var_s0_2 != NULL) {
        do {
            temp_s1 = var_s0_2->next;
            if ((u32) (var_s0_2->lastFrame + 1) < (u32) D_800B2660) {
                if (D_800F2794.firstUsed == var_s0_2) {
                    D_800F2794.firstUsed = temp_s1;
                }
                alUnlink(var_s0_2);
                if (D_800F2794.firstFree == NULL) {
                    D_800F2794.firstFree = var_s0_2;
                    var_s0_2->next = NULL;
                    var_s0_2->prev = 0;
                    goto block_11;
                }
                alLink(var_s0_2, D_800F2794.firstFree);
                var_s0_2 = temp_s1;
            } else {
block_11:
                var_s0_2 = temp_s1;
            }
        } while (var_s0_2 != NULL);
    }
    D_800B2664 = 0;
    D_800B2660 += 1;
}
