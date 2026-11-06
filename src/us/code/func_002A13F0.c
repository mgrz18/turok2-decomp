/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/IO/80200500_2.CPP, func_80200610_de, with Turok 2's layouts.
 * (A non-matching draft there, which also fills the 16 memory ranges; Turok 2 only builds the TLB part.)
 * -msplit-addresses like the neighbouring debugger I/O code (HostIo_Delay); the do {} while (0) blocks
 * steer register allocation. */
/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */
#include "common.h"
#include "structs/memory.h"

extern u32 sHostIoSavedTlb[32][4];     /* saved TLB entries: PageMask, EntryHi, EntryLo0, EntryLo1 */
extern u32 sHostIoTlbPageMasks[];          /* page masks 4K, 16K, ... 16M; 0x80000000 ends the table */

extern u32 func_002A31FC(void);   /* reads the TLB, returns EntryHi */

/* Rebuilds the table of virtual ranges the 32 TLB entries map. An entry whose EntryLo pages are valid
   (bit 1) maps 4K << 2n bytes per page; a non-global page of another ASID does not count. */
void HostIo_ScanTlb(void) {
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
    e = sHostIoSavedTlb[0];
    for (i = 16; i < 48; i++) {
        sHostIoMemoryRanges[i].m_First = -1;
        sHostIoMemoryRanges[i].m_Flags = 0;
        do { pageMask = *e++; } while (0);
        hi = *e++;
        lo0 = *e++;
        lo1 = *e++;
        if ((lo0 | lo1) & 2) {
            pageMask &= 0x1FFE000;
            mask = sHostIoTlbPageMasks;
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
                sHostIoMemoryRanges[i].m_First = start;
                sHostIoMemoryRanges[i].m_Last = start + size - 1;
                sHostIoMemoryRanges[i].m_Flags = 3;
            }
        }
    }
}
