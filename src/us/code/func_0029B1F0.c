/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BAE40_de_14.c, func_802BAE40_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern int D_8000030C;
extern void osInitialize();
extern int __osSiRawReadIo(unsigned int devAddr, unsigned int * data);
extern int __osSiRawWriteIo(unsigned int devAddr, unsigned int data);
extern s32 osPiRawReadIo(s32 arg0, s32 *arg1);
extern void osMapTLBRdb();
extern u32 D_80122998;
extern Quad_func_802A1BE0_de __osException[];
extern u64 D_80122990;
extern s32 D_800B8900;
extern unsigned char D_8000031C[];
extern void __osSetSR(unsigned int sr);
extern unsigned int __osSetFpcCsr(unsigned int csr);
extern unsigned int __osGetSR();
extern s32 D_80000300;
extern void osWritebackDCache(void *addr, s32 size);
extern void osInvalICache(void *addr, s32 size);
extern void *memset(void *dst, s32 value, u32 size);
void osInitialize(void)
{
    u32 pifdata;
    u32 clock = 0;
    u64 *rate;
    D_80122998 = 1;
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
    *(Quad_func_802A1BE0_de *)0x80000000 = *__osException;
    *(Quad_func_802A1BE0_de *)0x80000080 = *__osException;
    *(Quad_func_802A1BE0_de *)0x80000100 = *__osException;
    *(Quad_func_802A1BE0_de *)0x80000180 = *__osException;
    osWritebackDCache((void *)0x80000000, 0x80000180 - 0x80000000 + sizeof(Quad_func_802A1BE0_de));
    osInvalICache((void *)0x80000000, 0x80000180 - 0x80000000 + sizeof(Quad_func_802A1BE0_de));
    osMapTLBRdb();
    osPiRawReadIo(4, &clock);
    clock &= ~0xf;
    if (clock != 0) {
        D_80122990 = clock;
    } else {
        D_80122990 = 62500000;
    }
    rate = &D_80122990;
    *rate = *rate * 3 / 4;
    if (D_8000030C == 0) {
        memset(D_8000031C, 0, 0x40);
    }
    if (D_80000300 == 0) {
        D_800B8900 = 0x2F5B2D2;
    } else if (D_80000300 == 2) {
        D_800B8900 = 0x2E6025C;
    } else {
        D_800B8900 = 0x2E6D354;
    }
}
int __osSiRawReadIo(u32 arg0, u32 *arg1);
int __osSiRawWriteIo(u32 arg0, u32 arg1);
