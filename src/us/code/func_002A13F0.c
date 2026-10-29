/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/IO/80200500_2.CPP, func_80200610_de, with Turok 2's layouts.
 * (A non-matching draft there, which also fills the 16 memory ranges; Turok 2 only builds the TLB part.)
 * -msplit-addresses like the neighbouring debugger I/O code (func_002A13BC); the do {} while (0) blocks
 * steer register allocation. */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */
#include "common.h"

/* One mapped TLB range: the virtual addresses start..end, flags 3 when valid. */
typedef struct {
    /* 0x0 */ u32 start;
    /* 0x4 */ u32 end;
    /* 0x8 */ s32 flags;
} TlbRange;

extern TlbRange D_80128130[48]; /* 16 memory ranges, then the 32 TLB ranges */
extern u32 D_801288A0[32][4];     /* saved TLB entries: PageMask, EntryHi, EntryLo0, EntryLo1 */
extern u32 D_800BC308[];          /* page masks 4K, 16K, ... 16M; 0x80000000 ends the table */

extern u32 func_002A31FC(void);   /* reads the TLB, returns EntryHi */

/* Rebuilds the table of virtual ranges the 32 TLB entries map. An entry whose EntryLo pages are valid
   (bit 1) maps 4K << 2n bytes per page; a non-global page of another ASID does not count. */
void func_002A13F0(void) {
    u32 *e;
    u32 pageMask;
    u32 hi;
    u32 lo0;
    u32 lo1;
    u32 size;
    u32 start;
    u32 asid;
    u32 *mask;
    u32 m;
    u32 i;

    asid = func_002A31FC() & 0xFF;
    e = D_801288A0[0];
    for (i = 16; i < 48; i++) {
        D_80128130[i].start = -1;
        D_80128130[i].flags = 0;
        do { pageMask = *e++; } while (0);
        hi = *e++;
        lo0 = *e++;
        lo1 = *e++;
        if ((lo0 | lo1) & 2) {
            pageMask &= 0x1FFE000;
            mask = D_800BC308;
            size = 0x1000;
        next_mask:
            m = *mask;
            if (m != pageMask) {
                do {
                    mask++;
                    if (!(m & 0x80000000)) {
                        size <<= 2;
                        goto next_mask;
                    }
                } while (0);
                size = 0;
            }
            if (size != 0) {
                if ((lo0 & lo1 & 3) != 3) {
                    if ((lo0 & 3) == 2 && (hi & 0xFF) != asid) {
                        lo0 &= ~2;
                    }
                    if ((lo1 & 3) == 2 && (hi & 0xFF) != asid) {
                        lo1 &= ~2;
                    }
                }
                m = ~0x1FFF; /* VPN2 of EntryHi (a separate temp keeps the constant out of the loop) */
                start = hi & m;
                if (lo0 & lo1 & 2) {
                    size <<= 1;
                } else if (lo1 & 2) {
                    start += size;
                }
                D_80128130[i].start = start;
                D_80128130[i].end = start + size - 1;
                D_80128130[i].flags = 3;
            }
        }
    }
}
