/* cc1-flags: -quiet -G0 -mips3 -O2 -mgas -meb -mcpu=VR4300 -mno-fix4300 -mhard-float -mfp64 -msplit-addresses */
#include "common.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80200500_de.c, func_80200568_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80200500_de.c, func_802005A0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/IO/80200568_2.CPP, func_802005DC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "globals/hostio.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/IO/80200500_2.CPP, func_80200610_de, with Turok 2's layouts.
 * (A non-matching draft there, which also fills the 16 memory ranges; Turok 2 only builds the TLB part.)
 * -msplit-addresses like the neighbouring debugger I/O code (HostIo_Delay); the do {} while (0) blocks
 * steer register allocation. */
#include "common.h"
#include "structs/memory.h"
#include "m2c_macros.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80200800_de.c, func_80200880_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* Acclaim debugger I/O: copy from cartridge space (see HostIo_ReadBlock, its no-DMA twin). */
#include "common.h"
#include "protos/ultra.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80200800_de_3.c, func_80200B14_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80200800_de_4.c, func_80200C28_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/fuzzy/func_80200D18_de.c, func_80200D18_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/IO/80200B14_2.CPP, func_80200E04_de, with Turok 2's layouts. */

#include "common.h"
#include "ultra_types.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80200800_de_6.c, func_80200F98_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00201064.c, func_00201064.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/IO/io_hostmemory.CPP, func_802010D8_de, with Turok 2's layouts. */

#include "common.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002012D8.c, func_002012D8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/IO/802013D8.CPP, func_802013D8_de, with Turok 2's layouts. */

#include "common.h"
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002014A4.c, func_002014A4, with Turok 2's layouts. */

#include "common.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002016FC.c, func_002016FC.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/CISet__GetBlockAndSize.c, CISet__GetBlockAndSize, with Turok 2's layouts. */

#include "common.h"
/* Boundary fixed in versions/cuts.us.txt: splat's HostIo_ThreadMain (0x208 bytes) is two functions: the I/O thread HostIo_ThreadMain
 * (0x1B4 bytes, an endless loop with no epilogue) and PCinit (0x54 bytes) right after it,
 * which starts at 0x2A2804 with its own `addiu $sp, $sp, -0x18` prologue. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002018A0.c, func_002018A0,
 * and src/game/func_00201A74.c, func_00201A74, with Turok 2's layouts. */

#include "common.h"
#include "structs/hostio.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00201ACC.c, func_00201ACC.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
/* Matches with --cc cc1n64.exe (SN cc1n64), like the rest of this OS-side file; not with the default cc1. */

#include "common.h"
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/CDecompressor__DecompressSync.c, CDecompressor__DecompressSync, with Turok 2's layouts. */

#include "common.h"
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00201C80.c, func_00201C80, with Turok 2's layouts. */

#include "common.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00201DB0.c, func_00201DB0.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/CLoader__LoadAsync.c, CLoader__LoadAsync.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00202018.c, func_00202018.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
/* Turok 2 builds this file with -msplit-addresses (the ROM splits %hi/%lo itself). */

#include "common.h"
/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00202198.c, func_00202198.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"

u32 HostIo_GetSR(void);
void HostIo_SetSR(u32 sr);

s32 HostIo_DisableInt(void) {
    s32 temp_v0;

    temp_v0 = HostIo_GetSR();
    HostIo_SetSR(temp_v0 & ~1);
    return temp_v0 & 1;
}

void HostIo_RestoreInt(s32 arg0) {
    HostIo_SetSR(HostIo_GetSR() | arg0);
}
void HostIo_WriteWord(s32 address, s32 value);

void HostIo_WriteWord(s32 address, s32 value) {
    if ((*(volatile u32 *)(0xA4600010U)) & 3U) {
        do {
        } while ((*(volatile u32 *)(0xA4600010U)) & 3U);
    }
    *(s32 *)address = value;
}
s32 HostIo_ReadWord(s32 arg0);

s32 HostIo_ReadWord(s32 arg0) {
    if ((*(volatile u32 *)(0xA4600010U)) & 3U) {
        do {
        } while ((*(volatile u32 *)(0xA4600010U)) & 3U);
    }
    return (*(volatile u32 *)(arg0));
}
void HostIo_Delay(s32 arg0);

void HostIo_Delay(s32 arg0) {
    for (sHostIoDelayCounter = 0; sHostIoDelayCounter < arg0; sHostIoDelayCounter++) {
    }
}

extern u32 HostIo_ReadTlb(void);   /* reads the TLB, returns EntryHi */

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

    asid = HostIo_ReadTlb() & 0xFF;
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

s32 HostIo_ReadByte(s32 arg0) {
    s32 shift;

    shift = ~arg0;
    shift &= 3;
    shift *= 8;
    return ((u32) HostIo_ReadWord(arg0 & ~3) >> shift) & 0xFF;
}

s32 HostIo_ReadByteDma(s32 arg0) {
    s32 shift;

    shift = ~arg0;
    shift &= 3;
    shift *= 8;
    return ((u32) HostIo_ReadWord(arg0 & ~3) >> shift) & 0xFF;
}

void HostIo_ReadBlock(s32 source, u8 *destination, u32 count) {
    while (count != 0 && (source & 3) != 0) {
        *destination++ = HostIo_ReadByte(source++);
        count--;
    }
    while (count >= 4) {
        u32 word = HostIo_ReadWord(source);
        *destination++ = word >> 24;
        *destination++ = word >> 16;
        *destination++ = word >> 8;
        *destination++ = word;
        source += 4;
        count -= 4;
    }
    while (count != 0) {
        *destination++ = HostIo_ReadByte(source++);
        count--;
    }
}

/* Copies count bytes of cartridge space at source to destination: bytes up to a word boundary,
   then whole words, by DMA when the PI manager runs and the rest is 16 bytes or more to a 16-byte
   aligned destination, then the trailing bytes. */
void HostIo_ReadBlockDma(u32 source, u8 *destination, u32 count) {
    s32 dma;

    dma = 0;
    if (sHostIoPiReady == 0) {
        dma = -1;
    }
    while (count != 0 && (source & 3) != 0) {
        *destination++ = HostIo_ReadByteDma(source++);
        count--;
    }
    while (count >= 4) {
        if (count >= 16 && !((uptr)destination & 0xF) && dma != -1) {
            dma = osPiStartDma(&sHostIoDmaMsg, 1, 0, source, destination, count, &sHostIoDmaQueue);
            if (dma != -1) {
                osRecvMesg(&sHostIoDmaQueue, NULL, 1);
                count = 0;
            }
        } else {
            u32 word = HostIo_ReadWord(source);
            *destination++ = word >> 24;
            *destination++ = word >> 16;
            *destination++ = word >> 8;
            *destination++ = word;
            source += 4;
            count -= 4;
        }
    }
    while (count != 0) {
        *destination++ = HostIo_ReadByteDma(source++);
        count--;
    }
}

/* Store byte arg1 at the unaligned address arg0 via a word read-modify-write. */
void HostIo_PokeByte(s32 arg0, s32 arg1) {
    s32 shift = (~arg0 & 3) * 8;
    u32 *word = (u32 *)(arg0 & ~3);
    u32 old = *word;
    *word = (old & ~(0xFF << shift)) | ((arg1 & 0xFF) << shift);
}

void HostIo_ReadBlockRmw(s32 source, s32 destination, u32 count) {
    while (count != 0 && (source & 3) != 0) {
        HostIo_PokeByte(destination++, (u8)HostIo_ReadByte(source++));
        count--;
    }
    while (count >= 4) {
        u32 word = HostIo_ReadWord(source);
        HostIo_PokeByte(destination++, word >> 24);
        HostIo_PokeByte(destination++, (word >> 16) & 0xFF);
        HostIo_PokeByte(destination++, (word >> 8) & 0xFF);
        HostIo_PokeByte(destination++, word & 0xFF);
        source += 4;
        count -= 4;
    }
    while (count != 0) {
        HostIo_PokeByte(destination++, (u8)HostIo_ReadByte(source++));
        count--;
    }
}

void HostIo_WriteByte(s32 arg0, unsigned char arg1)
{
  s32 temp_s0;
  s32 temp_s2;
  temp_s2 = ((~arg0) & 3) * 8;
  arg0 &= ~3;
  HostIo_WriteWord(arg0, (HostIo_ReadWord(arg0) & (~(0xFF << temp_s2))) | ((arg1 & 0xFF) << temp_s2));
}

void HostIo_WriteByteDma(s32 arg0, unsigned char arg1)
{
  s32 temp_s0;
  s32 temp_s2;
  temp_s2 = ((~arg0) & 3) * 8;
  arg0 &= ~3;
  HostIo_WriteWord(arg0, (HostIo_ReadWord(arg0) & (~(0xFF << temp_s2))) | ((arg1 & 0xFF) << temp_s2));
}

void HostIo_WriteBlock(s32 address, const u8 *source, u32 length)
{
  u32 word;
  while ((length != 0) && ((address & 3) != 0))
  {
    HostIo_WriteByte(address++, *(source++));
    length--;
  }
  if (length >= 4)
  {
    do
    {
      word = (((((u32) source[0]) << 24) | (((u32) source[1]) << 16)) | (((u32) source[2]) << 8)) | source[3];
      HostIo_WriteWord(address, word);
      address += 4;
      source += 4;
      length -= 4;
    }
    while (length >= 4);
  }
  while (length != 0)
  {
    HostIo_WriteByte(address++, *(source++));
    length--;
  }
}

/* Write an arbitrary byte stream to the device, using DMA when aligned
 * and falling back to byte/word writes if the DMA request is unavailable. */
void HostIo_WriteBlockDma(u32 address, u8 *source, u32 remaining) {
    s32 dmaResult = 0;

    if (sHostIoPiReady == 0) {
        dmaResult = -1;
    }
    while (remaining != 0 && (address & 3) != 0) {
        HostIo_WriteByteDma(address++, *source++);
        remaining--;
    }
    while (remaining >= 4) {
        if (remaining >= 16 && ((uptr) source & 15) == 0 && dmaResult != -1) {
            dmaResult = osPiStartDma(&sHostIoDmaMsg, 1, 1, address, source, remaining, &sHostIoDmaQueue);
            if (dmaResult != -1) {
                osRecvMesg(&sHostIoDmaQueue, NULL, 1);
                remaining = 0;
            }
        } else {
            HostIo_WriteWord(address, ((u32) source[0] << 24) | ((u32) source[1] << 16) | ((u32) source[2] << 8)
                                       | source[3]);
            address += 4;
            source += 4;
            remaining -= 4;
        }
    }
    while (remaining != 0) {
        HostIo_WriteByteDma(address++, *source++);
        remaining--;
    }
}

void HostIo_FillBlock(s32 address, s32 value, u32 count)
{
  u32 new_var;
  u32 byte;
  unsigned char new_var2;
  u32 word;
  new_var2 = value & 0xFF;
  while ((count != 0) && ((address & 3) != 0))
  {
    HostIo_WriteByte(address++, new_var2);
    count--;
  }
  if (count >= 4)
  {
    byte = new_var2;
    new_var = byte;
    word = (((byte << 24) | (byte << 16)) | (new_var << 8)) | new_var;
    do
    {
      HostIo_WriteWord(address, word);
      count -= 4;
      address += 4;
    }
    while (count >= 4);
  }
  while (count)
  {
    HostIo_WriteByte(address++, new_var2);
    count--;
  }
}

void HostIo_CopyCartBytes(u32 dst, u32 src, u32 n) {
    while (n--) {
        HostIo_WriteByte(dst++, HostIo_ReadByte(src++) & 0xFF);
    }
}

extern u8 gHostIoHighAddressBias[];   /* local: an absolute symbol, an address offset (not data) */

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
        address += (uptr) gHostIoHighAddressBias;
    }
    while (remaining != 0) {
        i = 0;
        while (address < sHostIoMemoryRanges[i].m_First || address > sHostIoMemoryRanges[i].m_Last) {
            i++;
        }
        if (i >= 48) {
            flags = 0;
            chunk = 0x1000 - (address & 0xFFF);
        } else {
            flags = sHostIoMemoryRanges[i].m_Flags;
            chunk = (sHostIoMemoryRanges[i].m_Last + 1) - address;
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

void HostIo_CopyMailboxBlock(void) {
    u32 src = HostIo_ReadWord(0xB1FFFFF4) & 0xB1FFFFFC;
    u32 len = HostIo_ReadWord(0xB1FFFFF8) & 0x1FFFFFC;
    u32 i;
    HostIo_WriteWord(0xB1FFFFFC, 0);
    while (HostIo_ReadWord(0xB0000010) == 0) {
        HostIo_Delay(500);
    }
    for (i = 0; i < (len >> 2); i++) {
        HostIo_WriteWord((src & 0xB07FFFFF) + i * 4, HostIo_ReadWord(src + i * 4));
    }
    HostIo_Delay(2000);
    HostIo_WriteWord(0xB1FFFFF4, 0);
}

#define osMemSize (*(u32 *) 0x80000318) /* a constant address, not the symbol: extern u32 osMemSize schedules differently (round G2) */

extern void HostIo_InstallVectors(void);

/* Reset the debugger mailbox and build the memory permission map for the installed RAM i. */
void HostIo_Init(void) {
    s32 saved;
    u32 i; /* also holds the RAM size first: a separate variable changes the registers */
    u32 *src;
    u32 first;

    saved = HostIo_DisableInt();
    HostIo_InstallVectors();
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

/* The debugger host's command loop: wait for commands in the mailbox at 0xB1FFFFF0 and run them
 * until told to continue. */
void HostIo_CommandLoop(void) {
    s32 cont;
    u32 cmd;

    do {
        cont = 1;
        while ((cmd = HostIo_ReadWord(0xB1FFFFF0)) == 0) {
            HostIo_Delay(1000);
        }
        if (cmd == 2) {
            HostIo_WriteWord(0xB1FFFFFC, 0);
            HostIo_Delay(1000);
        } else {
            HostIo_WriteWord(0xB1FFFFFC, 0x101);
            while (HostIo_ReadWord(0xB1FFFFF0) == cmd) {
                HostIo_Delay(500);
            }
            if (HostIo_ReadWord(0xB1FFFFF0) == 2) {
                HostIo_WriteWord(0xB1FFFFFC, 0);
            } else {
                switch (cmd) {
                case 0x10:
                    break;
                case 0x11:
                    cont = 0;
                    break;
                case 0x12:
                    sHostIoSavedSR &= ~1;
                    sHostIoStopped = 0;
                    sHostIoResumeAllowed = 0;
                    HostIo_SetSR(HostIo_GetSR() & ~1);
                    break;
                case 0x13:
                    sHostIoResumeAllowed = 0;
                    sHostIoStopped = 0;
                    break;
                case 0x14:
                    sHostIoResumeAllowed = 0xFF;
                    break;
                case 0x15:
                    sHostIoResumeAllowed = 0;
                    break;
                case 0x16:
                    HostIo_WriteWord(0xB1FE0000, (sHostIoResumeAllowed << 24) | (sHostIoStopped << 16));
                    break;
                case 0x17:
                    HostIo_MemoryTransfer(2);
                    break;
                case 0x18:
                    HostIo_MemoryTransfer(1);
                    break;
                case 0x19:
                    HostIo_CopyMailboxBlock();
                    break;
                }
                HostIo_WriteWord(0xB1FFFFFC, 0);
                while (HostIo_ReadWord(0xB1FFFFF0) == 1) {
                    HostIo_Delay(500);
                }
            }
        }
    } while (cont != 0 || sHostIoResumeAllowed == 0);
    HostIo_WriteWord(0xB1FFFFF0, 0);
}

/* Length including the terminator. */
s32 HostIo_StrSize(u8 *s) {
    u8 *p = s;

    while (*p++ != 0) {
    }
    return p - s;
}

extern u8 sHostIoThreadStackEnd[]; /* local type: globals/hostio.h declares it; this view (its type here) is what the code uses */
void HostIo_ThreadMain(void *arg);

void HostIo_StartThread(s32 pri) {
    if (sHostIoPiReady == 0) {
        sHostIoPiReady = 1;
        osCreateMesgQueue(&sHostIoDmaQueue, sHostIoDmaBuf, 1);
        osCreateMesgQueue(&sHostIoRequestQueue, sHostIoRequestBuf, 1);
        osCreateMesgQueue(&sHostIoReplyQueue, sHostIoReplyBuf, 1);
        osCreateMesgQueue(&sHostIoEventQueue, sHostIoEventBuf, 1);
        osSetEventMesg(0, &sHostIoEventQueue, (OSMesg)1);
        osCreateThread(&sHostIoThread, 0x4653, HostIo_ThreadMain, 0, sHostIoThreadStackEnd + 4, pri);
        osStartThread(&sHostIoThread);
    }
}

/* Posts a command to the host through the 0xB1FFFFxx registers and waits until it is done.
 * The port has its own (SN debug host I/O, break 1: port/cmake/game.cmake). */
#ifndef TARGET_PORT
s32 HostIo_Call(s32 cmd, s32 arg0, s32 arg1, s32 arg2) {
    HostIo_WriteWord(0xB1FFFFE4, arg0);
    HostIo_WriteWord(0xB1FFFFE8, arg1);
    HostIo_WriteWord(0xB1FFFFEC, arg2);
    HostIo_WriteWord(0xB1FFFFE0, cmd);
    do {
        HostIo_Delay(1000);
        if (HostIo_ReadWord(0xB1FFFFF0) != 0) {
            __asm__ volatile("break 1");
        }
    } while (HostIo_ReadWord(0xB1FFFFE0) != 0);
    return HostIo_ReadWord(0xB1FFFFDC);
}
#endif

/* I/O thread: serve the requests sent through sHostIoRequestQueue (send block, run command, read block, reply). */
void HostIo_ThreadMain(void *arg) {
    HostIoRequest *req;
    u32 len;

    for (;;) {
        osRecvMesg(&sHostIoRequestQueue, &req, 1);
        if (req->size != 0 && req->write == 1) {
            if (req->wC.flags & 0x80000000) {
                sHostIoPiReady = 0;
            }
            HostIo_WriteBlockDma(0xB1FF0000, req->buffer, req->size);
            if (req->wC.flags & 0x80000000) {
                sHostIoPiReady = 1;
            }
        }
        osRecvMesg(&sHostIoEventQueue, NULL, 0);
        HostIo_WriteWord(0xB1FFFFE4, req->arg0);
        HostIo_WriteWord(0xB1FFFFE8, req->arg1);
        HostIo_WriteWord(0xB1FFFFEC, req->arg2);
        HostIo_WriteWord(0xB1FFFFD8, 0x1010101);
        HostIo_WriteWord(0xB1FFFFE0, req->wC.h.cmd);
        osRecvMesg(&sHostIoEventQueue, NULL, 1);
        if (req->size != 0 && req->write == 0) {
            len = HostIo_ReadWord(0xB1FFFFDC);
            if (len != 0) {
                if (req->wC.flags & 0x80000000) {
                    sHostIoPiReady = 0;
                }
                HostIo_ReadBlockDma(0xB1FF0000, req->buffer, len);
                if (req->wC.flags & 0x80000000) {
                    sHostIoPiReady = 1;
                }
            }
        }
        if (req->reply != NULL) {
            osSendMesg(req->reply, (void *) HostIo_ReadWord(0xB1FFFFDC), 1);
        }
    }
}

s32 PCinit(void) {
    s32 saved;
    s32 result;

    saved = HostIo_DisableInt();
    sHostIoState = 1;
    result = HostIo_Call(0x101, 0, 0, 0);
    HostIo_RestoreInt(saved);
    return result;
}

sptr PCinitThreaded(void) {
    HostIoRequest req;
    OSMesg reply;
    sHostIoState = 2;
    HostIo_StartThread(0xFF);
    req.size = 0;
    req.wC.flags = 0x80000101;
    req.arg0 = 0;
    req.arg1 = 0;
    req.arg2 = 0;
    req.reply = &sHostIoReplyQueue;
    osSendMesg(&sHostIoRequestQueue, &req, 1);
    osRecvMesg(&sHostIoReplyQueue, &reply, 1);
    return (sptr)reply;
}

/* Send a string to the debugger host (command 0x202), through the I/O thread when it runs. */
s32 PCopen(u8 *src, s32 arg) {
    HostIoRequest req;
    s32 reply;
    s32 len;
    s32 saved;

    len = HostIo_StrSize(src);
    if (sHostIoState == 2) {
        req.buffer = src;
        req.size = len;
        req.write = 1;
        req.wC.flags = 0x80000000 | 0x202;
        req.arg0 = len;
        req.arg1 = arg;
        req.arg2 = 0;
        req.reply = &sHostIoReplyQueue;
        osSendMesg(&sHostIoRequestQueue, &req, 1);
        osRecvMesg(&sHostIoReplyQueue, &reply, 1);
        if (reply & 0x8000) {
            reply |= 0xFFFF0000;
        }
    } else {
        saved = HostIo_DisableInt();
        HostIo_WriteBlock(0xB1FF0000, src, len);
        reply = HostIo_Call(0x202, len, arg, 0);
        if (reply & 0x8000) {
            reply |= 0xFFFF0000;
        }
        HostIo_RestoreInt(saved);
    }
    return reply;
}

/* Send a string to the debugger host (command 0x303), through the I/O thread when it runs. */
s32 PCcreat(u8 *src, s32 arg) {
    HostIoRequest req;
    s32 reply;
    s32 len;
    s32 saved;

    len = HostIo_StrSize(src);
    if (sHostIoState == 2) {
        req.buffer = src;
        req.size = len;
        req.write = 1;
        req.wC.flags = 0x80000000 | 0x303;
        req.arg0 = len;
        req.arg1 = arg;
        req.arg2 = 0;
        req.reply = &sHostIoReplyQueue;
        osSendMesg(&sHostIoRequestQueue, &req, 1);
        osRecvMesg(&sHostIoReplyQueue, &reply, 1);
        if (reply & 0x8000) {
            reply |= 0xFFFF0000;
        }
    } else {
        saved = HostIo_DisableInt();
        HostIo_WriteBlock(0xB1FF0000, src, len);
        reply = HostIo_Call(0x303, len, arg, 0);
        if (reply & 0x8000) {
            reply |= 0xFFFF0000;
        }
        HostIo_RestoreInt(saved);
    }
    return reply;
}

s32 PCclose(s32 arg) {
    HostIoRequest req;
    s32 reply;
    s32 mask;
    if (sHostIoState == 2) {
        req.size = 0;
        req.wC.flags = 0x80000404;
        req.arg0 = arg;
        req.arg1 = 0;
        req.arg2 = 0;
        req.reply = &sHostIoReplyQueue;
        osSendMesg(&sHostIoRequestQueue, &req, 1);
        osRecvMesg(&sHostIoReplyQueue, (OSMesg *)&reply, 1);
        if (reply & 0x8000) {
            reply |= 0xFFFF0000;
        }
    } else {
        mask = HostIo_DisableInt();
        reply = HostIo_Call(0x404, arg, 0, 0);
        if (reply & 0x8000) {
            reply |= 0xFFFF0000;
        }
        HostIo_RestoreInt(mask);
    }
    return reply;
}

s32 PCread(s32 arg, u8 *buf, u32 n) {
    HostIoRequest req;
    s32 reply;
    s32 done = 0;
    s32 mask;
    u32 chunk;
    if (sHostIoState == 2) {
        while (n != 0) {
            chunk = n;
            if (chunk > 0x8000) {
                chunk = 0x8000;
            }
            req.buffer = buf;
            req.size = 1;
            req.write = 0;
            req.wC.flags = 0x80000505;
            req.arg0 = arg;
            req.arg1 = chunk;
            req.arg2 = 0;
            req.reply = &sHostIoReplyQueue;
            osSendMesg(&sHostIoRequestQueue, &req, 1);
            osRecvMesg(&sHostIoReplyQueue, (OSMesg *)&reply, 1);
            if (reply == 0) {
                return done;
            }
            buf += reply;
            n -= reply;
            done += reply;
        }
    } else {
        mask = HostIo_DisableInt();
        while (n != 0) {
            chunk = n;
            if (chunk > 0x8000) {
                chunk = 0x8000;
            }
            reply = HostIo_Call(0x505, arg, chunk, 0);
            if (reply == 0) {
                break;
            }
            HostIo_ReadBlock(0xB1FF0000, buf, reply);
            buf += reply;
            n -= reply;
            done += reply;
        }
        HostIo_RestoreInt(mask);
    }
    return done;
}

s32 PCwrite(s32 arg, u8 *buf, u32 n) {
    HostIoRequest req;
    s32 reply;
    s32 done = 0;
    s32 mask;
    u32 chunk;
    if (sHostIoState == 2) {
        while (n != 0) {
            chunk = n;
            if (chunk > 0x8000) {
                chunk = 0x8000;
            }
            req.buffer = buf;
            req.size = chunk;
            req.write = 1;
            req.wC.flags = 0x80000606;
            req.arg0 = arg;
            req.arg1 = chunk;
            req.arg2 = 0;
            req.reply = &sHostIoReplyQueue;
            osSendMesg(&sHostIoRequestQueue, &req, 1);
            osRecvMesg(&sHostIoReplyQueue, (OSMesg *)&reply, 1);
            if (reply == 0) {
                return done;
            }
            buf += reply;
            n -= reply;
            done += reply;
        }
    } else {
        mask = HostIo_DisableInt();
        while (n != 0) {
            chunk = n;
            if (chunk > 0x8000) {
                chunk = 0x8000;
            }
            HostIo_WriteBlock(0xB1FF0000, buf, chunk);
            reply = HostIo_Call(0x606, arg, chunk, 0);
            if (reply == 0) {
                break;
            }
            buf += reply;
            n -= reply;
            done += reply;
        }
        HostIo_RestoreInt(mask);
    }
    return done;
}

typedef struct IoReq {
               s32 unk0;
               s32 unk4;
               s32 unk8;
               u32 cmd;
               s32 a0;
               s32 a1;
               s32 a2;
               OSMesgQueue *reply;
} IoReq;

s32 PClseek(s32 a0, s32 a1, s32 a2) {
    IoReq req;
    s32 result;
    s32 mask;
    if (sHostIoState == 2) {
        req.unk4 = 0;
        req.cmd = 0x80000707;
        req.a0 = a0;
        req.a1 = a1;
        req.a2 = a2;
        req.reply = &sHostIoReplyQueue;
        osSendMesg(&sHostIoRequestQueue, &req, 1);
        osRecvMesg(&sHostIoReplyQueue, (OSMesg *)&result, 1);
    } else {
        mask = HostIo_DisableInt();
        result = HostIo_Call(0x707, a0, a1, a2);
        HostIo_RestoreInt(mask);
    }
    return result;
}
