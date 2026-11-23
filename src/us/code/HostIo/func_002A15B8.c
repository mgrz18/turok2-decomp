/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80200800_de.c, func_80200880_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 HostIo_ReadWord(s32 arg0);
extern int HostIo_ReadByte(int arg0);
int HostIo_ReadByte(int arg0);
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
