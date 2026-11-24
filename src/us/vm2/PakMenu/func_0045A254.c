#include "common.h"
#include "globals/menu_text.h"
#include "globals/scene.h"
#include "protos/pak.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/PAK/80405DC0_16.CPP, func_804098CC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

extern void *memset(void *dst, s32 value, uptr size);
extern void CCache__Flush_Arg(s32) __asm__("CCache__Flush"); /* the calls pass 0; the definition (code/CCache) takes none */
extern void CCache__Deallocate(sptr, void *);
extern void **CCache__Allocate(s32, s32, s32, char *);
extern void CCache__Deallocate(sptr, void *);
extern void CCache__Flush_Arg(s32) __asm__("CCache__Flush"); /* the calls pass 0; the definition (code/CCache) takes none */
void Pak_AllocBuffer_2(void ***handle, void **data, s32 units) {
    void **block;
    void *first;
    if (*handle != 0) {
        CCache__Flush_Arg(0);
        CCache__Deallocate(0, *handle);
    }
    if (units != 0) {
        units <<= 8;
    } else {
        units = (s32)((*(char * *) &gScene.unk_30) + 0x688);
    }
    block = CCache__Allocate(0, units, 0x33, sPakMenuText_SingleSaveBlock);
    *handle = block;
    first = *block;
    *data = first;
    memset(first, 0, units);
}
