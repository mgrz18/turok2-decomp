/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/libgultra/part_800C7F20/code_802B66A0.c, func_802B15D0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 func_00292530(void *arg0, s32 arg1, s32 arg2);
extern void func_002933B0(void *arg0);
extern void func_002933E0(void *arg0, void **arg1);
s32 func_00292530(void *arg0, s32 arg1, s32 arg2) {
    s32 amount;
    s32 total;
    s32 result;
    char *next;
    char *cur;
    total = 0;
    cur = ((ALSeqPlayer *)arg0)->evtq.allocList.next;
    result = 1;
    if (cur != 0) {
        do {
            amount = ((ALEventListItem *)cur)->delta;
            next = ((ALEventListItem *)cur)->node.next;
            total += amount;
            if (((ALEventListItem *)cur)->evt.type == 5 && (s32)((ALEvent *)&((ALEventListItem *)cur)->evt)->msg.note.voice == arg1) {
                if (arg2 < total) {
                    if (next != 0) {
                        ((ALEventListItem *)next)->delta = ((ALEventListItem *)next)->delta + amount;
                    }
                    func_002933B0(cur);
                    func_002933E0(cur, &((ALSeqPlayer *)arg0)->evtq.freeList.next);
                    goto done;
                }
                result = 0;
                goto done;
            }
            cur = next;
        } while (cur != 0);
    }
done:
    return result;
}
