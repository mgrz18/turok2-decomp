/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802846C8_de_17.c, func_80285D30_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 CList__AddTail(IntrusiveList *list, void *node);
void func_0026EDA8(void *arg0) {
    extern void func_0029E010(s32 arg0);
    extern s32 func_0029DFF0(void);
    extern void CList__Remove(void *, s32);
    void *node;
    void *next;
    s32 saved;
    saved = func_0029DFF0();
    node = ((PoolRecAt28V1 *)arg0)->active.head;
    if (node != 0) {
        do {
            next = ((NodeCode8028469C *)node)->next;
            CList__Remove(&((PoolRecAt28V1 *)arg0)->active, node);
            CList__AddTail(arg0, node);
            node = next;
        } while (node != 0);
    }
    ((PoolRecAt28V1 *)arg0)->baseLevel = 0;
    func_0029E010(saved);
}
