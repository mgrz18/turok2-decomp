/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8028DF90_de_7.c, func_8028F9AC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/frame.h"
#include "globals/framebuffer.h"
#include "protos/ultra.h"

extern void *__scTaskReady(struct OSScTask_s *arg0);
void *__scTaskReady(struct OSScTask_s *arg0) {
    sptr f1a;
    sptr f1b;
    void *v2;
    if (arg0 == 0) {
        return 0;
    }
    f1a = (sptr)arg0->framebuffer;
    if (f1a == gCurrentFB) {
        return 0;
    }
    v2 = osViGetCurrentFramebuffer();
    f1b = (sptr)arg0->framebuffer;
    if (f1b == (sptr)v2) {
        return 0;
    }
    if (f1b != gPendingFB) {
        return arg0;
    }
    return 0;
}
