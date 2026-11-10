/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B8F48_de_7.c, func_802B43A4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "structs/libaudio.h"

typedef s32 (*FuncPtr)(void *);
extern void alFilterNew(void *arg0, void *a, void *b, s32 c);
#ifdef TARGET_PORT
/* the heap and the blocks are host pointers (64-bit: the s32 versions truncated them) */
extern void *alHeapDBAlloc(s32, s32, void *, s32, s32);
typedef sptr LoadNewHeap;
#else
extern s32 alHeapDBAlloc(s32, s32, void *, s32, s32);
typedef s32 LoadNewHeap;
#endif
extern void alAdpcmPull(void);
extern s32 alLoadParam(void *filter, s32 paramID, void *param);
void alLoadNew(void *arg0, FuncPtr arg1, LoadNewHeap arg2) {
    alFilterNew(arg0, (void *) alAdpcmPull, alLoadParam, 0);
    ((ALLoadFilter *)arg0)->state = alHeapDBAlloc(0, 0, (void *)arg2, 1, 0x20);
    ((ALLoadFilter *)arg0)->lstate = alHeapDBAlloc(0, 0, (void *)arg2, 1, 0x20);
    ((ALLoadFilter *)arg0)->dma = arg1(&((ALLoadFilter *)arg0)->dmaState);
    ((ALLoadFilter *)arg0)->lastsam = 0;
    ((ALLoadFilter *)arg0)->first = 1;
    ((ALLoadFilter *)arg0)->memin = 0;
}
