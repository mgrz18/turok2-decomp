/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80403E90_de_6.c, func_804053E0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "structs/pak.h"

extern int gPakState[]; /* local type: globals/pak.h declares it; this view (its type here) is what keeps the ROM's code */
extern s32 Pak_GetNotePages(s32 index, s32 entry, s32 *out);
extern s32 Pak_BytesToPages(s32);
s32 Pak_GetNotePages(s32 index, s32 entry, s32 *out) {
    if (gPakState[index] != 3) {
        return -2;
    }
    *out = Pak_BytesToPages(gPakDirectories[index].notes[entry].file_size);
    return 0;
}
