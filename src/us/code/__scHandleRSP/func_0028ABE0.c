/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8028DF90_de_7.c, func_8028F544_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "protos/sched.h"
#include "protos/ultra.h"
#include "globals/frame.h"
#include "globals/framebuffer.h"

s32 __scSchedule(OSSched *arg0, OSScTask **arg1, OSScTask **arg2, s32 arg3) {
    OSScTask *temp_s0;
    OSScTask *temp_v1_0;
    OSScTask *temp_v0;
    OSScTask *var_v1;
    s32 temp_s5;
    s32 temp_v0_2;
    s32 temp_v1;
    s32 var_s1;
    var_s1 = arg3;
    temp_s0 = arg0->gfxListHead;
    temp_v1_0 = arg0->audioListHead;
    temp_s5 = var_s1;
    if ((arg0->doAudio != 0) && (var_s1 & 2)) {
        if ((temp_s0 == 0) || !(temp_s0->flags & 0x10)) {
            *arg1 = temp_v1_0;
            arg0->doAudio = 0;
            temp_v0 = arg0->audioListHead->next;
            var_s1 &= ~2;
            arg0->audioListHead = temp_v0;
            if (temp_v0 == 0) {
                arg0->audioListTail = 0;
            }
        } else {
            *arg1 = temp_s0;
            var_s1 &= ~2;
        }
    } else {
        if (temp_s0 == 0) {
            var_v1 = 0;
        } else if (temp_s0->framebuffer == gCurrentFB) {
            var_v1 = 0;
        } else if (temp_s0->framebuffer == osViGetCurrentFramebuffer()) {
            var_v1 = 0;
        } else if (temp_s0->framebuffer == gPendingFB) {
            var_v1 = 0;
        } else {
            var_v1 = temp_s0;
        }
        if (var_v1 != 0) {
            temp_v0_2 = temp_s0->flags & 7;
            switch (temp_v0_2) {
            case 1:
            case 4:
            case 5:
                break;
            case 3:
                if (temp_s0->state & 0x20) {
                    if (var_s1 & 2) {
                        *arg1 = temp_s0;
                        var_s1 &= ~2;
                        if (temp_s0->state & 1) {
                            *arg2 = temp_s0;
                            var_s1 &= ~1;
                        }
                        arg0->gfxListHead = arg0->gfxListHead->next;
                        if (arg0->gfxListHead == 0) {
                            arg0->gfxListTail = 0;
                        }
                    }
                } else if (var_s1 == 3) {
                    *arg2 = temp_s0;
                    *arg1 = temp_s0;
                    arg0->gfxListHead = arg0->gfxListHead->next;
                    var_s1 = 0;
                    if (arg0->gfxListHead == 0) {
                        arg0->gfxListTail = 0;
                    }
                }
                break;
            case 2:
            case 6:
            case 7:
                temp_v1 = temp_s0->state;
                if (temp_v1 & 2) {
                    if (var_s1 & 2) {
                        *arg1 = temp_s0;
                        var_s1 &= ~2;
                    }
                } else if ((temp_v1 & 1) && (var_s1 & 1)) {
                    *arg2 = temp_s0;
                    arg0->gfxListHead = arg0->gfxListHead->next;
                    var_s1 &= ~1;
                    if (arg0->gfxListHead == 0) {
                        arg0->gfxListTail = 0;
                    }
                }
                break;
            }
        }
    }
    if (var_s1 != temp_s5) {
        var_s1 = __scSchedule(arg0, arg1, arg2, var_s1);
    }
    return var_s1;
}
