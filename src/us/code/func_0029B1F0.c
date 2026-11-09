/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BAE40_de_14.c, func_802BAE40_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/libultra.h"
#include "protos/ultra.h"

extern void osInitialize();
extern int __osSiRawReadIo(unsigned int devAddr, unsigned int * data);
extern int __osSiRawWriteIo(unsigned int devAddr, unsigned int data);
extern s32 osPiRawReadIo(s32 arg0, s32 *arg1);
extern void osMapTLBRdb();
extern void __osSetSR(unsigned int sr);
extern unsigned int __osSetFpcCsr(unsigned int csr);
extern unsigned int __osGetSR();
extern void *memset(void *dst, s32 value, uptr size);
void osInitialize(void)
{
    u32 pifdata;
    u32 clock = 0;
    u64 *rate;
    __osInitialized = 1;
    __osSetSR(
__osGetSR
() | 0x20000000);
    __osSetFpcCsr(0x01000800);
    while (__osSiRawReadIo(0x1FC007FC, &pifdata)) {
        ;
    }
    while (__osSiRawWriteIo(0x1FC007FC, pifdata | 8)) {
        ;
    }
    *(__osExceptionVector *)0x80000000 = *__osException;
    *(__osExceptionVector *)0x80000080 = *__osException;
    *(__osExceptionVector *)0x80000100 = *__osException;
    *(__osExceptionVector *)0x80000180 = *__osException;
    osWritebackDCache((void *)0x80000000, 0x80000180 - 0x80000000 + sizeof(__osExceptionVector));
    osInvalICache((void *)0x80000000, 0x80000180 - 0x80000000 + sizeof(__osExceptionVector));
    osMapTLBRdb();
    osPiRawReadIo(4, &clock);
    clock &= ~0xf;
    if (clock != 0) {
        osClockRate = clock;
    } else {
        osClockRate = 62500000;
    }
    rate = &osClockRate;
    *rate = *rate * 3 / 4;
    if (osResetType == 0) {
        memset(osAppNMIBuffer, 0, 0x40);
    }
    if (osTvType == 0) {
        osViClock = 0x2F5B2D2;
    } else if (osTvType == 2) {
        osViClock = 0x2E6025C;
    } else {
        osViClock = 0x2E6D354;
    }
}
int __osSiRawReadIo(u32 arg0, u32 *arg1);
int __osSiRawWriteIo(u32 arg0, u32 arg1);
