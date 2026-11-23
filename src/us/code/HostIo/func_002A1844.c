/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80200800_de_3.c, func_80200B14_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 HostIo_ReadWord(s32 arg0);
extern void HostIo_PokeByte(s32 arg0, s32 arg1);
extern int HostIo_ReadByte(int arg0);
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
