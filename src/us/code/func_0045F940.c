/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80403BCC_8.CPP, func_804053E0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/pak.h"

extern s32 Pak_GetNotePages_2(s32 index, s32 entry, s32 *out);
extern s32 Pak_BytesToPages_2(s32);
s32 Pak_GetNotePages_2(s32 index, s32 entry, s32 *out) {
    if (gPakState[index] != 3) {
        return -2;
    }
    *out = Pak_BytesToPages_2(gPakDirectories[index].notes[entry].file_size);
    return 0;
}
