/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8028DF90_de_7.c, func_8028FA60_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/frame.h"
#include "protos/ultra.h"

extern int D_800B6E60;
extern void __scExec(struct View_func_8028FA60_de *, struct Scene_func_8028FA60_de *, struct Scene_func_8028FA60_de *);
extern s32 D_800B6E64;
extern s32 D_800B6E64;
void __scExec(View_func_8028FA60_de *view, Scene_func_8028FA60_de *current, Scene_func_8028FA60_de *next) {
    s64 now;
    if (next != 0 && (next->options & 0x20) && !(next->flags & 0x10)) {
        sRdpStartTime = osGetTime();
    }
    if (current != 0) {
        if (current->mode == 2) {
            osWritebackDCacheAll();
            sAudioTaskStartTime = osGetTime();
        } else if (current->options & 0x20) {
            now = osGetTime();
            sGfxRspResumeTime = now;
            if (!(current->flags & 0x10)) {
                sGfxTaskStartTime = now;
                sGfxRspTime = 0;
            }
        }
        current->flags &= ~0x30;
        osSpTaskLoad(&current->mode);
        osSpTaskStartGo(&current->mode);
        view->current = current;
        if (current == next) {
            view->next = next;
        }
    }
    if (next != 0 && next != current) {
        osDpSetNextBuffer(next->music, *next->time);
        D_800B6E60 = 1;
        D_800B6E64 = 0;
        view->next = next;
    }
}
