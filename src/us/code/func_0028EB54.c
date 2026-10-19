/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8023A294_de_15.c, func_8023CCD4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void func_0028EB54(OwnerRecAt8V2 *arg0);
extern Slot_func_8023CCD4_de D_80120FC0[];
void func_0028EB54(OwnerRecAt8V2 *arg0) {
    extern s32 osSendMesg(s32, Node_func_8023CCD4_de *, s32);
    extern void func_0028DEFC(Slot_func_8023CCD4_de *);
    extern Node_func_8023CCD4_de D_80121378;
    Node_func_8023CCD4_de *node;
    Node_func_8023CCD4_de *prev;
    u8 *slot;
    s32 i;
    u32 key;
    node = &D_80121378;
    key = arg0->handle >> 12;
    prev = 0;
    while (node != 0) {
        if (node->id == key) {
            break;
        }
        prev = node;
        node = node->next;
    }
    prev->next = node->next;
    slot = node->slots;
    for (i = 0; i < node->count; i++) {
        if (*slot != 0xFF) {
            func_0028DEFC(&D_80120FC0[*slot]);
            D_80120FC0[*slot].id = 0xFFFF;
        }
        slot++;
    }
    osSendMesg(arg0->queue, node, 1);
}
