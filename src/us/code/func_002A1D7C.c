/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00201064.c, func_00201064.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/junk_types.h"

u32 HostIo_ReadByte(u32 addr);
void HostIo_WriteByte(u32 addr, u32 val);
void HostIo_CopyCartBytes(u32 dst, u32 src, u32 n) {
    while (n--) {
        HostIo_WriteByte(dst++, HostIo_ReadByte(src++) & 0xFF);
    }
}
