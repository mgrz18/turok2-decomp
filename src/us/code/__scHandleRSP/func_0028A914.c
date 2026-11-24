/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/fuzzy/func_8028F278_de.c, func_8028F278_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "protos/sched.h"
#include "protos/ultra.h"
#include "globals/frame.h"
#include "structs/game.h"
#include "globals/render.h"
#include "globals/engine.h"

extern void Scheduler_RDPDoneHook();

void __scHandleRDP(OSSched *sc)
{
    OSScTask *value1 = 0;
    OSScTask *value2 = 0;
    OSScTask *task = sc->curRDPTask;
    void *framebuffer;
    CFrameData *schedule;
    CFrameData *schedules;
    s32 flags;
    task->state &= ~1;
    if (task->list.t.type == 1) {
        if ((task->flags & 0x20) != 0) {
            gSchedRdpTime = (f32)(((osGetTime() - sRdpStartTime) << 6) / 3) * 1.0e-6f;
        }
        sc->curRDPTask = 0;
        if ((task->flags & 0x20) != 0) {
            schedules = gEngineApp.m_FrameData;
            framebuffer = task->framebuffer;
            if (framebuffer == (void *)schedules[0].m_pFrameBuffer) {
                schedule = &schedules[0];
            } else if (framebuffer == (void *)schedules[1].m_pFrameBuffer) {
                schedule = &schedules[1];
            } else if (framebuffer == (void *)schedules[2].m_pFrameBuffer) {
                schedule = &schedules[2];
            } else {
                schedule = &schedules[0];
            }
            gNumTicks3 = gNumTicks2;
            gNumTicks2 = gNumTicks1;
            gNumTicks1 = gNumTicks0;
            gNumTicks0 = gCurrentTick + 1;
            if (gNumTicks0 >= schedule->m_nPredictTicks && gNumTicksWaiting == 0 && gNumTicksPending == 0) {
                gPendingFB = (u16 *)schedule->m_pFrameBuffer;
                osViSwapBuffer((void *)schedule->m_pFrameBuffer);
                gNumTicksPending++;
            } else {
                gNumTicksWaiting++;
                gWaitingFrames[gCurrentTickWaiting] = schedule;
                gCurrentTickWaiting++;
                if (gCurrentTickWaiting >= gTotalFramebuffers) {
                    gCurrentTickWaiting = 0;
                }
            }
            Scheduler_RDPDoneHook(schedule);
        }
    }
    __scTaskComplete(sc, task);
    flags = (sc->curRSPTask == 0) * 2 | (sc->curRDPTask == 0);
    if (__scSchedule(sc, &value1, &value2, flags) != flags) {
        __scExec(sc, value1, value2);
    }
}
