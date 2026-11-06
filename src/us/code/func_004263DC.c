/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80403E90_de_5.c, func_80404D84_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void Pak_AllocDirectory(void);
extern int *gPakStateBuffer;
extern s32 gPakDirectoryAllocated;
struct Shape_typemap_165 *CCache__Allocate(s32, s32, u32, void *);
void *memset(void *, s32, u32);
extern s32 D_800C18A0;
extern s32 gPakDirectories;
extern int gPakDirectoryAllocated;
extern int gPakDirectories;
extern int *gPakStateBuffer;
void Pak_AllocDirectory(void) {
    struct Shape_typemap_165 *temp_v0;
    s32 temp_a0;
    temp_v0 = CCache__Allocate(0, 0x810, 0x23, &D_800C18A0);
    temp_a0 = temp_v0->field_0;
    gPakStateBuffer = (s32 *)temp_v0;
    gPakDirectories = temp_a0;
    memset((void *)temp_a0, 0, 0x810);
    gPakDirectoryAllocated = 1;
}
