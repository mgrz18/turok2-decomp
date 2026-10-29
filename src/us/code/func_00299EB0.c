/* From decompals/ultralib (libultra 2.0I): src/io/pirawdma.c, osPiRawStartDma.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */

typedef unsigned long u32;
typedef long s32;
typedef volatile unsigned long vu32;
extern void *D_80000308;
extern u32 func_0029BF80(void *);
s32 osPiRawStartDma(s32, u32 , void *, u32 );
#ident "$Revision: 1.17 $"
s32 osPiRawStartDma(s32 direction, u32 devAddr, void* dramAddr, u32 size) {
    register u32 stat;
    { stat = (*(vu32*)((u32)((0x04600000 + 0x10))|0xA0000000)); while (stat & ((1 << 1) | (1 << 0))) stat = (*(vu32*)((u32)((0x04600000 + 0x10))|0xA0000000)); } (void)0;
    (*(vu32*)((u32)((0x04600000 + 0x00))|0xA0000000)=(u32)(func_0029BF80(dramAddr)));
    (*(vu32*)((u32)((0x04600000 + 0x04))|0xA0000000)=(u32)(((u32)((u32)D_80000308 | devAddr)&0x1FFFFFFF)));
    switch (direction) {
        case 0:
            (*(vu32*)((u32)((0x04600000 + 0x0C))|0xA0000000)=(u32)(size - 1));
            break;
        case 1:
            (*(vu32*)((u32)((0x04600000 + 0x08))|0xA0000000)=(u32)(size - 1));
            break;
        default:
            return -1;
    }
    return 0;
}
