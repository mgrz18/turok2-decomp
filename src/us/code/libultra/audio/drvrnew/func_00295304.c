/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B8F48_de_7.c, func_802B43A4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/libaudio.h"

void alFilterNew(void *f, void *handler, void *setParam, s32 type);
/* ALDMANew returns the ALDMAproc (a host function pointer: s32 truncated it on macOS) */
/* the heap and the blocks are host pointers (64-bit: the s32 versions truncated them) */
void *alHeapDBAlloc(u8 *file, s32 line, void *hp, s32 num, s32 size);
typedef sptr LoadNewHeap;
extern void alAdpcmPull(void);
extern s32 alLoadParam(void *filter, s32 paramID, void *param);
void alLoadNew(void *arg0, ALDMANew arg1, LoadNewHeap arg2) {
    alFilterNew(arg0, (void *) alAdpcmPull, alLoadParam, 0);
    ((ALLoadFilter *)arg0)->state = alHeapDBAlloc(0, 0, (void *)arg2, 1, 0x20);
    ((ALLoadFilter *)arg0)->lstate = alHeapDBAlloc(0, 0, (void *)arg2, 1, 0x20);
    ((ALLoadFilter *)arg0)->dma = arg1(&((ALLoadFilter *)arg0)->dmaState);
    ((ALLoadFilter *)arg0)->lastsam = 0;
    ((ALLoadFilter *)arg0)->first = 1;
    ((ALLoadFilter *)arg0)->memin = 0;
}
