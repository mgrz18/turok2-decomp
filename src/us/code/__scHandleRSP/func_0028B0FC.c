/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8028DF90_de_7.c, func_8028FA60_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "protos/sched.h"
#include "protos/ultra.h"
#include "globals/frame.h"

extern s32 D_800B6E60;
extern s32 D_800B6E64;

void __scExec(OSSched *view, OSScTask *current, OSScTask *next) {
    s64 now;
    if (next != 0 && (next->flags & 0x20) && !(next->state & 0x10)) {
        sRdpStartTime = osGetTime();
    }
    if (current != 0) {
        if (current->list.t.type == 2) {
            osWritebackDCacheAll();
            sAudioTaskStartTime = osGetTime();
        } else if (current->flags & 0x20) {
            now = osGetTime();
            sGfxRspResumeTime = now;
            if (!(current->state & 0x10)) {
                sGfxTaskStartTime = now;
                sGfxRspTime = 0;
            }
        }
        current->state &= ~0x30;
        osSpTaskLoad(&current->list);
        osSpTaskStartGo(&current->list);
        view->curRSPTask = current;
        if (current == next) {
            view->curRDPTask = next;
        }
    }
    if (next != 0 && next != current) {
        osDpSetNextBuffer(next->list.t.output_buff, *next->list.t.output_buff_size);
        D_800B6E60 = 1;
        D_800B6E64 = 0;
        view->curRDPTask = next;
    }
}
