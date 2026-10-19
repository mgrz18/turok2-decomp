/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BAE40_de_14.c, func_802BAE40_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern int D_8000030C;
extern void func_0029B1F0();
extern int func_0029B4B0(unsigned int devAddr, unsigned int * data);
extern int func_0029B540(unsigned int devAddr, unsigned int data);
extern s32 func_0029B410(s32 arg0, s32 *arg1);
extern void osMapTLBRdb();
extern u32 D_80122998;
extern Quad_func_802A1BE0_de D_0029BFE0[];
extern u64 D_80122990;
extern s32 D_800B8900;
extern unsigned char D_8000031C[];
extern void func_0029E2E0(unsigned int sr);
extern unsigned int func_0029E220(unsigned int csr);
extern unsigned int func_0029DFE0();
extern s32 D_80000300;
extern void func_0029E340(void *addr, s32 size);
extern void func_0029E0D0(void *addr, s32 size);
extern void *func_00266C5C(void *dst, s32 value, u32 size);
void func_0029B1F0(void)
{
    u32 pifdata;
    u32 clock = 0;
    u64 *rate;
    D_80122998 = 1;
    func_0029E2E0(
func_0029DFE0
() | 0x20000000);
    func_0029E220(0x01000800);
    while (func_0029B4B0(0x1FC007FC, &pifdata)) {
        ;
    }
    while (func_0029B540(0x1FC007FC, pifdata | 8)) {
        ;
    }
    *(Quad_func_802A1BE0_de *)0x80000000 = *D_0029BFE0;
    *(Quad_func_802A1BE0_de *)0x80000080 = *D_0029BFE0;
    *(Quad_func_802A1BE0_de *)0x80000100 = *D_0029BFE0;
    *(Quad_func_802A1BE0_de *)0x80000180 = *D_0029BFE0;
    func_0029E340((void *)0x80000000, 0x80000180 - 0x80000000 + sizeof(Quad_func_802A1BE0_de));
    func_0029E0D0((void *)0x80000000, 0x80000180 - 0x80000000 + sizeof(Quad_func_802A1BE0_de));
    osMapTLBRdb();
    func_0029B410(4, &clock);
    clock &= ~0xf;
    if (clock != 0) {
        D_80122990 = clock;
    } else {
        D_80122990 = 62500000;
    }
    rate = &D_80122990;
    *rate = *rate * 3 / 4;
    if (D_8000030C == 0) {
        func_00266C5C(D_8000031C, 0, 0x40);
    }
    if (D_80000300 == 0) {
        D_800B8900 = 0x2F5B2D2;
    } else if (D_80000300 == 2) {
        D_800B8900 = 0x2E6025C;
    } else {
        D_800B8900 = 0x2E6D354;
    }
}
int func_0029B4B0(u32 arg0, u32 *arg1);
int func_0029B540(u32 arg0, u32 arg1);
