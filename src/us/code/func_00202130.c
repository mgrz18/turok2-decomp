/* WRONG CUT: splat named this D_00202128 (0x202128, 200 bytes): its first 8 bytes are junk fill
 * ("Style" text), the real function starts at the 16-aligned 0x202130 (184 bytes; CEngineApp__Main calls it),
 * followed by 8 bytes of fill (a nop and a junk word) before CHeap__AllocAt.
 * Heap set-up at boot: clear everything from the end of the image (D_8014A000) to the top of RDRAM
 * (minus 512 KB when the expansion pak is present), hand it to the heap, then start the subsystems. */

#include "common.h"

s32 GetMemSize(void);                                  /* osGetMemSize */
void *memset(void *dst, s32 value, u32 size);      /* memset */
void CHeap__Construct(void *heap, void *base, u32 size);
void CDecompressor__Construct(void *p, s32 n);
void CLoader__Construct(void *p, s32 n);
void VM_Initialize(s32 a, s32 b);
void CCache__Construct(s32 a, s32 b);

extern u8 D_8014A000[];   /* end of the loaded image */
extern u8 gHeap[];   /* the main heap */
extern u8 gDecompressor[];
extern u8 gLoader[];

void InitializeSystemResources(void) {
    u8 *base;
    u32 top;
    u32 size;

    top = 0x80400000;
    base = D_8014A000;
    if (GetMemSize() != 0x400000) {
        top = (GetMemSize() | 0x80000000) - 0x80000;
    }
    size = top - (u32) base;
    memset(base, 0, size);
    CHeap__Construct(gHeap, base, size);
    CDecompressor__Construct(gDecompressor, 3);
    CLoader__Construct(gLoader, 4);
    VM_Initialize(0, 0);
    CCache__Construct(0, 5);
}
