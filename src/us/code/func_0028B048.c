/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8028DF90_de_7.c, func_8028F9AC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void *__scTaskReady(struct OSScTask_s *arg0);
extern void *osViGetCurrentFramebuffer();
extern s32 gPendingFB;
extern s32 gCurrentFB;
void *__scTaskReady(struct OSScTask_s *arg0) {
    s32 f1a;
    s32 f1b;
    void *v2;
    if (arg0 == 0) {
        return 0;
    }
    f1a = (s32)arg0->framebuffer;
    if (f1a == gCurrentFB) {
        return 0;
    }
    v2 = osViGetCurrentFramebuffer();
    f1b = (s32)arg0->framebuffer;
    if (f1b == (s32)v2) {
        return 0;
    }
    if (f1b != gPendingFB) {
        return arg0;
    }
    return 0;
}
