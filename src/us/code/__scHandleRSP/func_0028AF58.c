/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8028DF90_de_7.c, func_8028F8BC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "protos/sched.h"
#include "protos/ultra.h"

void osScRemoveClient(OSSched *arg0, OSScClient *arg1) {
    s32 saved;
    OSScClient *cur;
    OSScClient *prev;
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
