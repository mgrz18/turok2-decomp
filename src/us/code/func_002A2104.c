/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/IO/802013D8.CPP, func_802013D8_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/memory.h"
#include "globals/hostio.h"

/* The built-in map: pairs of {last address, flags}, ranges follow each other. */
typedef struct MemoryBoundaryInit {
    /* 0x0 */ u32 last;
    /* 0x4 */ u32 flags;
} MemoryBoundaryInit; /* i 0x8 */

#define osMemSize (*(u32 *) 0x80000318) /* a constant address, not the symbol: extern u32 osMemSize schedules differently (round G2) */
extern MemoryBoundaryInit gHostIoMemoryBounds[];
extern s32 HostIo_DisableInt(void);
extern void func_002A3100(void);
extern void HostIo_WriteWord(u32 address, u32 value);
extern void HostIo_RestoreInt(s32 saved);

/* Reset the debugger mailbox and build the memory permission map for the installed RAM i. */
void HostIo_Init(void) {
    s32 saved;
    u32 i; /* also holds the RAM size first: a separate variable changes the registers */
    u32 *src;
    u32 first;

    saved = HostIo_DisableInt();
    func_002A3100();
    HostIo_WriteWord(0xB1FFFFF0, 0);
    HostIo_WriteWord(0xB1FFFFFC, 0);
    sHostIoStopped = 1;
    i = osMemSize;
    sHostIoResumeAllowed = 0xFF;
    if (i > 0x3FFFFF && (i & 0x1FFFF) == 0 && i <= 0x2000000) {
        i--;
        gHostIoMemoryBounds[1].last = i + 0x80000000;
        gHostIoMemoryBounds[3].last = i + 0xA0000000;
    }

    src = (u32 *) gHostIoMemoryBounds;
    first = 0x80000000;
    while (*src < first) {
        src += 2;
    }
    i = 0;
    do {
        sHostIoMemoryRanges[i].m_First = first;
        sHostIoMemoryRanges[i].m_Last = *src++;
        sHostIoMemoryRanges[i].m_Flags = *src++;
        first = sHostIoMemoryRanges[i].m_Last + 1;
        i++;
    } while (first != 0);
    for (; i < 16; i++) {
        sHostIoMemoryRanges[i].m_First = -1;
        sHostIoMemoryRanges[i].m_Flags = 0;
    }
    HostIo_RestoreInt(saved);
}
