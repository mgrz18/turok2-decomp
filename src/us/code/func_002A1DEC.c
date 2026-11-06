/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/IO/io_hostmemory.CPP, func_802010D8_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/memory.h"

extern u8 D_8F128560[];
extern u32 HostIo_ReadWord(u32 address);
extern void HostIo_ScanTlb(void);
extern void HostIo_FillBlock(u32 address, s32 value, u32 count);
extern void HostIo_CopyCartBytes(u32 dst, u32 src, u32 count);
extern void HostIo_WriteBlock(u32 address, u32 source, u32 count);
extern void HostIo_ReadBlockRmw(u32 port, u32 address, u32 count);
extern void HostIo_ReadBlock(u32 port, u32 address, u32 count);

/* Split the debugger's memory transfer at permission boundaries. */
void HostIo_MemoryTransfer(u32 mode) {
    u32 address, remaining, chunk, flags;
    u32 port = 0xB1FE0000;
    u32 i;

    address = HostIo_ReadWord(0xB1FFFFF4);
    remaining = HostIo_ReadWord(0xB1FFFFF8);
    if (address - 0x80000000 > 0x3FFFFFFF) {
        HostIo_ScanTlb();
    }
    if (address > 0xEFFFFFFF) {
        address += (u32) D_8F128560;
    }
    while (remaining != 0) {
        i = 0;
        while (address < D_80128130[i].m_First || address > D_80128130[i].m_Last) {
            i++;
        }
        if (i >= 48) {
            flags = 0;
            chunk = 0x1000 - (address & 0xFFF);
        } else {
            flags = D_80128130[i].m_Flags;
            chunk = (D_80128130[i].m_Last + 1) - address;
        }
        if (remaining < chunk) {
            chunk = remaining;
        }
        if ((flags & mode) == 0) {
            if (mode == 1) {
                HostIo_FillBlock(port, 0, chunk);
            }
        } else if (flags & 4) {
            if (mode == 1) {
                HostIo_CopyCartBytes(port, address, chunk);
            } else {
                HostIo_CopyCartBytes(address, port, chunk);
            }
        } else if (mode == 1) {
            HostIo_WriteBlock(port, address, chunk);
        } else if (flags & 8) {
            HostIo_ReadBlockRmw(port, address, chunk);
        } else {
            HostIo_ReadBlock(port, address, chunk);
        }
        port += chunk;
        remaining -= chunk;
        address += chunk;
    }
}
