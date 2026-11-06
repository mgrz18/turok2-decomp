/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80403E90_de_5.c, func_80404D84_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/pak.h"
#include "structs/pak.h"

void Pak_AllocDirectory(void);
struct Shape_typemap_165 *CCache__Allocate(s32, s32, u32, void *);
void *memset(void *, s32, u32);
void Pak_AllocDirectory(void) {
    struct Shape_typemap_165 *temp_v0;
    s32 temp_a0;
    temp_v0 = CCache__Allocate(0, 0x810, 0x23, "Mempak pakstate");
    temp_a0 = temp_v0->field_0;
    gPakStateBuffer = (CMemEntry *)temp_v0;
    gPakDirectories = (PakDirectory *)temp_a0;
    memset((void *)temp_a0, 0, 0x810);
    gPakDirectoryAllocated = 1;
}
