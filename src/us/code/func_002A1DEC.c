/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/IO/io_hostmemory.CPP, func_802010D8_de, with Turok 2's layouts. */

#include "common.h"

/* One entry of the debugger's memory permission map. */
typedef struct MemoryRange {
    /* 0x0 */ u32 first;
    /* 0x4 */ u32 last;
    /* 0x8 */ u32 flags;
} MemoryRange; /* size 0xC */

extern MemoryRange D_80128130[48];
extern u8 D_8F128560[];
extern u32 func_002A1384(u32 address);
extern void func_002A13F0(void);
extern void func_002A1CB4(u32 address, s32 value, u32 count);
extern void func_002A1D7C(u32 dst, u32 src, u32 count);
extern void func_002A1A3C(u32 address, u32 source, u32 count);
extern void func_002A1844(u32 port, u32 address, u32 count);
extern void func_002A15B8(u32 port, u32 address, u32 count);

/* Split the debugger's memory transfer at permission boundaries. */
void func_002A1DEC(u32 mode) {
    u32 address, remaining, chunk, flags;
    u32 port = 0xB1FE0000;
    u32 i;

    address = func_002A1384(0xB1FFFFF4);
    remaining = func_002A1384(0xB1FFFFF8);
    if (address - 0x80000000 > 0x3FFFFFFF) {
        func_002A13F0();
    }
    if (address > 0xEFFFFFFF) {
        address += (u32) D_8F128560;
    }
    while (remaining != 0) {
        i = 0;
        while (address < D_80128130[i].first || address > D_80128130[i].last) {
            i++;
        }
        if (i >= 48) {
            flags = 0;
            chunk = 0x1000 - (address & 0xFFF);
        } else {
            flags = D_80128130[i].flags;
            chunk = (D_80128130[i].last + 1) - address;
        }
        if (remaining < chunk) {
            chunk = remaining;
        }
        if ((flags & mode) == 0) {
            if (mode == 1) {
                func_002A1CB4(port, 0, chunk);
            }
        } else if (flags & 4) {
            if (mode == 1) {
                func_002A1D7C(port, address, chunk);
            } else {
                func_002A1D7C(address, port, chunk);
            }
        } else if (mode == 1) {
            func_002A1A3C(port, address, chunk);
        } else if (flags & 8) {
            func_002A1844(port, address, chunk);
        } else {
            func_002A15B8(port, address, chunk);
        }
        port += chunk;
        remaining -= chunk;
        address += chunk;
    }
}
