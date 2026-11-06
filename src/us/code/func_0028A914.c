/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/fuzzy/func_8028F278_de.c, func_8028F278_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "globals/frame.h"
#include "structs/game.h"
#include "globals/render.h"
#include "globals/engine.h"

extern void osViSwapBuffer(s32 arg0);
extern s32 __scSchedule(struct Ctx_func_8028F544_de *, struct Node_func_8028F544_de **, struct Node_func_8028F544_de **, s32);
extern void __scExec(struct View_func_8028FA60_de *, struct Scene_func_8028FA60_de *, struct Scene_func_8028FA60_de *);
extern s32 __scTaskComplete(void *,void *);
extern FrameSchedule *gWaitingFrames[];
extern void func_00228BBC();
extern u64 osGetTime(void);
extern u32 D_800F6CA8;
extern u32 gNumTicks2;
extern u32 gNumTicks3;
extern u32 gNumTicks0;
extern u32 gCurrentTick;
extern u32 gCurrentTickWaiting;
void __scHandleRDP(OSSched *sc)
{
    s32 value1 = 0;
    s32 value2 = 0;
    OSScTask_s *task = sc->curRDPTask;
    void *framebuffer;
    FrameSchedule *schedule;
    FrameSchedule *schedules;
    s32 flags;
    task->state &= ~1;
    if (task->list.type == 1) {
        if ((task->flags & 0x20) != 0) {
            gSchedRdpTime = (f32)(((osGetTime() - sRdpStartTime) << 6) / 3) * 1.0e-6f;
        }
        sc->curRDPTask = 0;
        if ((task->flags & 0x20) != 0) {
            schedules = (FrameSchedule *)gEngineApp.m_FrameData;
            framebuffer = task->framebuffer;
            if (framebuffer == (void *)schedules[0].task) {
                schedule = &schedules[0];
            } else if (framebuffer == (void *)schedules[1].task) {
                schedule = &schedules[1];
            } else if (framebuffer == (void *)schedules[2].task) {
                schedule = &schedules[2];
            } else {
                schedule = &schedules[0];
            }
            gNumTicks3 = gNumTicks2;
            gNumTicks2 = D_800F6CA8;
            D_800F6CA8 = gNumTicks0;
            gNumTicks0 = gCurrentTick + 1;
            if (gNumTicks0 >= schedule->duration && gNumTicksWaiting == 0 && gNumTicksPending == 0) {
                gPendingFB = schedule->task;
                osViSwapBuffer(schedule->task);
                gNumTicksPending++;
            } else {
                gNumTicksWaiting++;
                gWaitingFrames[gCurrentTickWaiting] = schedule;
                gCurrentTickWaiting++;
                if (gCurrentTickWaiting >= gTotalFramebuffers) {
                    gCurrentTickWaiting = 0;
                }
            }
            func_00228BBC(schedule);
        }
    }
    __scTaskComplete(sc, task);
    flags = (sc->curRSPTask == 0) * 2 | (sc->curRDPTask == 0);
    if (__scSchedule(sc, &value1, &value2, flags) != flags) {
        __scExec(sc, value1, value2);
    }
}
