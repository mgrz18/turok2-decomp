/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BB6C0_de_18.c, func_802BB9F8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern u64 D_80122A28;
extern s32 D_80122A20;
extern s32 D_80122A24;
extern void *D_800B8930;
extern void func_0029BDB8();
void func_0029BDB8(void) {
    NodeRecAt1C *node = (NodeRecAt1C *)D_800B8930;
    D_80122A28 = 0;
    D_80122A20 = 0;
    D_80122A24 = 0;
    node->field10 = 0;
    node->field8 = 0;
    node->prev = node;
    node->next = node;
    node->field18 = 0;
    node->field1C = 0;
}
