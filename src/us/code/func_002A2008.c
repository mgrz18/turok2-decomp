/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002012D8.c, func_002012D8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

u32 HostIo_ReadWord(u32 addr);
void HostIo_WriteWord(u32 addr, u32 val);
void HostIo_Delay(s32 n);
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
