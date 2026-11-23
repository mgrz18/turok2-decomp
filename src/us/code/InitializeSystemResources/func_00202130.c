/* WRONG CUT: splat named this D_00202128 (0x202128, 200 bytes): its first 8 bytes are junk fill
 * ("Style" text), the real function starts at the 16-aligned 0x202130 (184 bytes; CEngineApp__Main calls it),
 * followed by 8 bytes of fill (a nop and a junk word) before CHeap__AllocAt.
 * Heap set-up at boot: clear everything from the end of the image (D_8014A000) to the top of RDRAM
 * (minus 512 KB when the expansion pak is present), hand it to the heap, then start the subsystems. */

#include "common.h"
#include "globals/sysrc.h"
#include "protos/cache.h"

s32 GetMemSize(void);                                  /* osGetMemSize */
void *memset(void *dst, s32 value, uptr size);      /* memset */
void CHeap__Construct(CHeap *heap, void *base, uptr size);
void CDecompressor__Construct(CDecompressor *p, s32 n);
void CLoader__Construct(CLoader *p, s32 n);
void VM_Initialize(s32 a, s32 b);

extern u8 D_8014A000[];   /* end of the loaded image */

void InitializeSystemResources(void) {
    u8 *base;
    u32 top;
    uptr size;

    top = 0x80400000;
    base = D_8014A000;
    if (GetMemSize() != 0x400000) {
        top = (GetMemSize() | 0x80000000) - 0x80000;
    }
    size = top - (uptr) base;
    memset(base, 0, size);
    CHeap__Construct(&gHeap, base, size);
    CDecompressor__Construct(&gDecompressor, 3);
    CLoader__Construct(&gLoader, 4);
    VM_Initialize(0, 0);
    CCache__Construct(0, 5);
}
