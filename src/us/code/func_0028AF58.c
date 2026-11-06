/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8028DF90_de_7.c, func_8028F8BC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void osScRemoveClient(struct OSSched *arg0, void *arg1);
s32 osSetIntMask(s32);
extern s32 osSetIntMask(s32);
void osScRemoveClient(OSSched *arg0, void *arg1) {
    s32 saved;
    OSScClientWork *cur;
    OSScClientWork *prev;
    prev = 0;
    cur = arg0->clientList;
    saved = osSetIntMask(1);
    while (cur != 0) {
        if (cur == arg1) {
            if (prev != 0) {
                prev->next = cur->next;
            } else {
                arg0->clientList = cur->next;
            }
            break;
        }
        prev = cur;
        cur = cur->next;
    }
    osSetIntMask(saved);
}
