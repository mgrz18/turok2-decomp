/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/IO/802013D8.CPP, func_802013D8_de, with Turok 2's layouts. */

#include "common.h"

/* One entry of the debugger's memory permission map. */
typedef struct MemoryRange {
    /* 0x0 */ u32 first;
    /* 0x4 */ u32 last;
    /* 0x8 */ u32 flags;
} MemoryRange; /* i 0xC */

/* The built-in map: pairs of {last address, flags}, ranges follow each other. */
typedef struct MemoryBoundaryInit {
    /* 0x0 */ u32 last;
    /* 0x4 */ u32 flags;
} MemoryBoundaryInit; /* i 0x8 */

#define osMemSize (*(u32 *) 0x80000318)
extern MemoryBoundaryInit D_800B7600[];
extern MemoryRange D_80128130[];
extern u8 D_80128880;
extern u8 D_80128882;
extern s32 func_002A12F0(void);
extern void func_002A3100(void);
extern void func_002A1350(u32 address, u32 value);
extern void func_002A1324(s32 saved);

/* Reset the debugger mailbox and build the memory permission map for the installed RAM i. */
void func_002A2104(void) {
    s32 saved;
    u32 i; /* also holds the RAM size first: a separate variable changes the registers */
    u32 *src;
    u32 first;

    saved = func_002A12F0();
    func_002A3100();
    func_002A1350(0xB1FFFFF0, 0);
    func_002A1350(0xB1FFFFFC, 0);
    D_80128880 = 1;
    i = osMemSize;
    D_80128882 = 0xFF;
    if (i > 0x3FFFFF && (i & 0x1FFFF) == 0 && i <= 0x2000000) {
        i--;
        D_800B7600[1].last = i + 0x80000000;
        D_800B7600[3].last = i + 0xA0000000;
    }

    src = (u32 *) D_800B7600;
    first = 0x80000000;
    while (*src < first) {
        src += 2;
    }
    i = 0;
    do {
        D_80128130[i].first = first;
        D_80128130[i].last = *src++;
        D_80128130[i].flags = *src++;
        first = D_80128130[i].last + 1;
        i++;
    } while (first != 0);
    for (; i < 16; i++) {
        D_80128130[i].first = -1;
        D_80128130[i].flags = 0;
    }
    func_002A1324(saved);
}
