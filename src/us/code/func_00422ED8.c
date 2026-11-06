/* WRONG CUT: the function really starts at 0x00422ED8 (size 0x3C). splat left its prologue
 * (addiu sp,-0x18; sw ra) in stray_00422ED8: cuts.us.txt marks those two words as never
 * executed, misled by a data word 0x00422EE0 in virtual_rodata_1 that belongs to another
 * module mapped at the same addresses. SavePortalMenu_SelectSave inlines this same function. */
#include "common.h"

s32 Cinema_IsActive(void);
s32 Cinema_IsFinished(void);

s32 func_00422ED8(void) {
    if (Cinema_IsActive() != 0 && Cinema_IsFinished() == 0) {
        return 1;
    }
    return 0;
}
