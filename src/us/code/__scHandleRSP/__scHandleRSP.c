/* Boundary fixed in versions/cuts.us.txt: splat starts func_0028A018 two stray words early (`lwc1 $f0, 0x6F98($at)`,
 * `addu $a0, $s3, $zero`, never executed); the function really starts at 0x0028A020,
 * the thread entry osCreateScheduler passes as the address 0x28A020. Written whole as __scMain. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0028E910.c, func_0028E910, with Turok 2's layouts. */
#include "common.h"
#include "protos/sched.h"
#include "protos/ultra.h"
#include "globals/engine.h"
#include "protos/game.h"

extern void osViBlack(s32 black);   /* osViBlack */
extern void Video_UpdateResolution(void);
extern void Music_FadeOut(void);
extern void Sound__StopAllExcept(s32);
extern void LoopSounds_StopAll(void *pool); /* SoundSlotPool * */

/* Scheduler thread: serves retrace, RSP/RDP done, pre-NMI and fault messages forever. */
void __scMain(OSSched *sc) {
    s32 msg;
    OSScClient *client;

    while (1) {
        osRecvMesg(&sc->interruptQ, (OSMesg *) &msg, 1);
        switch (msg) {
            case 0x29A:
                if (sSchedResetPressed != 0) {
                    sSchedResetBlankRetraces = sSchedResetBlankRetraces - 1;
                    if (sSchedResetBlankRetraces <= 0) {
                        osViBlack(1);
                        while (1) {
                        }
                    }
                }
                __scHandleRetrace(sc);
                Video_UpdateResolution();
                break;
            case 0x29B:
                __scHandleRSP(sc);
                break;
            case 0x29C:
                __scHandleRDP(sc);
                break;
            case 0x29D:
                Music_FadeOut();
                Sound__StopAllExcept(-1);
                LoopSounds_StopAll(LoopSounds_Get());
                client = sc->clientList;
                sSchedResetPressed = 1;
                sSchedResetBlankRetraces = 20;
                while (client != NULL) {
                    osSendMesg(client->msgQ, (OSMesg) &sc->prenmiMsg, 0);
                    client = client->next;
                }
                break;
            case 6:
                Sched_HandleCmdMsg(sc);
                break;
        }
    }
}

#include "globals/frame.h"
#include "globals/render.h"
#include "globals/sysrc.h"
#include "globals/framebuffer.h"

extern void Controller_Service(void);

void __scHandleRetrace(OSSched *sc) {
    OSScTask *rspTask;
    OSScTask *sp;
    OSScTask *dp;
    OSScTask *task;
    OSScTask *next;
    OSScClient *client;
    CFrameData *frame;
    s32 state;
    u16 *fb;

    rspTask = NULL;
    sp = NULL;
    dp = NULL;
    if (gDecompressorBoostRetraces != 0) {
        gDecompressorBoostRetraces = gDecompressorBoostRetraces - 1;
        if (gDecompressorBoostRetraces == 0) {
            osSetThreadPri(&gDecompressor.thread, gThreadPriorityDecompressorLow);
        }
    }
    sc->frameCount = sc->frameCount + 1;
    while (osRecvMesg(&sc->cmdQ, (OSMesg *) &rspTask, 0) != -1) {
        __scAppendList(sc, rspTask);
    }
    gCurrentTick = gCurrentTick + 1;
    if (gNumTicksPending != 0) {
        gCurrentTick = 0;
        gNumTicksPending = 0;
        fb = gPendingFB;
        gPendingFB = 0;
        gCurrentFB = fb;
    }
    if (gNumTicksWaiting != 0) {
        frame = gWaitingFrames[gNextFrameToShow];
        if (!((u32)(gCurrentTick + 1) < frame->m_nPredictTicks)) {
            gPendingFB = (u16 *) frame->m_pFrameBuffer;
            osViSwapBuffer((void *) frame->m_pFrameBuffer);
            gNumTicksWaiting = gNumTicksWaiting - 1;
            gNumTicksPending = gNumTicksPending + 1;
            gNextFrameToShow = gNextFrameToShow + 1;
            if (!(gNextFrameToShow < gTotalFramebuffers)) {
                gNextFrameToShow = 0;
            }
        }
    }
    if (sc->curRSPTask == NULL && sc->curRDPTask == NULL) {
        task = sc->gfxListHead;
        if (task != NULL && task->list.t.type == 1 && task->state == 0x32) {
            task->state = 2;
            sc->curRSPTask = sc->gfxListHead;
            next = sc->gfxListHead->next;
            sc->gfxListHead = next;
            if (next == NULL) {
                sc->gfxListTail = NULL;
            }
            __scHandleRSP(sc);
        }
    }
    sSchedGfxWatchdog = sSchedGfxWatchdog + 1;
    if (sSchedGfxWatchdog == 1200) {
        *(u8 *)0 = 0;
    }
    if (sc->doAudio != 0 && sc->curRSPTask != NULL) {
        __scYield(sc);
    } else {
        state = (sc->curRSPTask == NULL) << 1 | (sc->curRDPTask == NULL);
        if (__scSchedule(sc, &sp, &dp, state) != state) {
            __scExec(sc, sp, dp);
        }
    }
    client = sc->clientList;
    while (client != NULL) {
        osSendMesg(client->msgQ, (OSMesg) &sc->retraceMsg, 0);
        client = client->next;
    }
    Controller_Service();
}

#include "structs/game.h"
#include "protos/cache.h"

void __scHandleRSP(OSSched *o) {
    OSScTask *j;
    OSScTask *a;
    OSScTask *b;
    CFrameData *s;
    CFrameData *base;
    uptr fb;
    s32 mode;
    j = o->curRSPTask;
    o->curRSPTask = 0;
    a = 0;
    b = 0;
    if ((j->state & 0x10) && osSpTaskYielded(&j->list) != 0) {
        j->state |= 0x20;
        if ((j->flags & 7) == 3) {
            j->next = o->gfxListHead;
            o->gfxListHead = j;
            if (o->gfxListTail == 0) {
                o->gfxListTail = j;
            }
        }
        if (j->flags & 0x20) {
            sGfxRspTime = sGfxRspTime + (f32)(((osGetTime() - sGfxRspResumeTime) * 64) / 3) * 0.000001f;
        }
    } else {
        j->state &= ~2;
        if (j->list.t.type == 1) {
            sSchedGfxWatchdog = 0;
            if (!(j->flags & 0x20)) {
                goto done;
            }
            sGfxRspTime = sGfxRspTime + (f32)(((osGetTime() - sGfxRspResumeTime) * 64) / 3) * 0.000001f;
            gSchedRspTotalTime = sGfxRspTime;
            gSchedRspTime = (f32)(((osGetTime() - sAudioTaskStartTime) * 64) / 3) * 0.000001f;
            base = gEngineApp.m_FrameData;
            fb = (uptr)j->framebuffer;
            if (fb == base[0].m_pFrameBuffer) {
                s = base;
                goto sel;
            }
            if (fb == base[1].m_pFrameBuffer) {
                s = base + 1;
                goto sel;
            }
            s = base;
            if (fb == base[2].m_pFrameBuffer) {
                s += 2;
            }
sel:
            CCache__PostDisplayListDone(0, (sptr)s->m_pDisplayList == gDisplayList0);
            if (osSendMesg(j->msgQ, j->msg, 0 /* OS_MESG_NOBLOCK */) == -1) {
                *(u8 *)0 = 0;
            }
        } else {
            gSchedRspTime = (f32)(((osGetTime() - sAudioTaskStartTime) * 64) / 3) * 0.000001f;
            if (osSendMesg(j->msgQ, j->msg, 0 /* OS_MESG_NOBLOCK */) == -1) {
                *(u8 *)0 = 0;
            }
        }
done:
        __scTaskComplete(o, j);
    }
    mode = (o->curRSPTask == 0) << 1 | (o->curRDPTask == 0);
    if (__scSchedule(o, &a, &b, mode) != mode) {
        __scExec(o, a, b);
    }
}

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

void osCreateScheduler(OSSched *sc, void *stack, OSPri priority, u8 mode, u8 numFields) {
    sc->retraceMsg.type = 1;
    sc->curRSPTask = 0;
    sc->curRDPTask = 0;
    sc->clientList = 0;
    sc->frameCount = 0;
    sc->audioListHead = 0;
    sc->gfxListHead = 0;
    sc->audioListTail = 0;
    sc->gfxListTail = 0;
    sc->doAudio = 0;
    sc->prenmiMsg.type = 4;
    osCreateMesgQueue(&sc->interruptQ, sc->intBuf, 8);
    osCreateMesgQueue(&sc->cmdQ, sc->cmdMsgBuf, 8);
    osSetEventMesg(4, &sc->interruptQ, (OSMesg) 0x29B);
    osSetEventMesg(9, &sc->interruptQ, (OSMesg) 0x29C);
    osSetEventMesg(0xE, &sc->interruptQ, (OSMesg) 0x29D);
    osViSetEvent(&sc->interruptQ, (OSMesg) 0x29A, numFields);
    osCreateThread(&sc->thread, 7, (void (*)(void *)) __scMain, sc, stack, priority);
    osStartThread(&sc->thread);
}

void osScAddClient(OSSched *arg0, OSScClient *arg1, OSMesgQueue *arg2) {
    s32 temp_v0;

    temp_v0 = osSetIntMask(1);
    arg1->msgQ = arg2;
    arg1->next = arg0->clientList;
    arg0->clientList = arg1;
    osSetIntMask(temp_v0);
}

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

OSMesgQueue *osScGetCmdQ(OSSched *sc) {
    return &sc->cmdQ;
}

void __scAppendList(OSSched *lists, OSScTask *node) {
    if (node->list.t.type == 2) {
        if (lists->audioListTail != NULL) {
            lists->audioListTail->next = node;
        } else {
            lists->audioListHead = node;
        }
        lists->audioListTail = node;
        lists->doAudio = 1;
    } else {
        if (lists->gfxListTail != NULL) {
            lists->gfxListTail->next = node;
        } else {
            lists->gfxListHead = node;
        }
        lists->gfxListTail = node;
    }
    node->next = NULL;
    node->state = node->flags & 3;
}

OSScTask *__scTaskReady(OSScTask *arg0) {
    sptr f1a;
    sptr f1b;
    void *v2;
    if (arg0 == 0) {
        return 0;
    }
    f1a = (sptr)arg0->framebuffer;
    if (f1a == (sptr)gCurrentFB) {
        return 0;
    }
    v2 = osViGetCurrentFramebuffer();
    f1b = (sptr)arg0->framebuffer;
    if (f1b == (sptr)v2) {
        return 0;
    }
    if (f1b != (sptr)gPendingFB) {
        return arg0;
    }
    return 0;
}

s32 __scTaskComplete(OSSched *arg0, OSScTask *arg1) {
    if (((s32)arg1->state) & 3) {
        return 0;
    }
    if ((arg1->list.t.type) != 1) {
        return 1;
    }
    if ((((s32)arg1->flags) & 0x60) != 0x60) {
        return 1;
    }
    osViSwapBuffer(arg1->framebuffer);
    return 1;
}

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

void __scYield(OSSched *arg0) {
    OSScTask *temp_a0;

    temp_a0 = arg0->curRSPTask;
    if (temp_a0->list.t.type == 1) {
        temp_a0->state = (s32) (temp_a0->state | 0x10);
        osSpTaskYield();
    }
}

void Sched_HandleCmdMsg(OSSched *arg0) {
    OSScTask *node = 0;
    OSScTask *value1 = 0;
    OSScTask *value2 = 0;
    s32 flags;
    while (osRecvMesg(&arg0->cmdQ, (OSMesg *) &node, 0) != -1) {
        if (node->list.t.type == 1 && (node->flags & 0x20) && gDecompressorBoostRetraces == 0) {
            gDecompressorBoostRetraces = 2;
            osSetThreadPri(&gDecompressor.thread, gThreadPriorityDecompressorHigh);
        }
        __scAppendList(arg0, node);
    }
    if (arg0->doAudio != 0 &&
        arg0->curRSPTask != 0) {
        __scYield(arg0);
        return;
    }
    flags = (arg0->curRSPTask == 0) * 2 |
            (arg0->curRDPTask == 0);
    if (__scSchedule(arg0, &value1, &value2, flags) != flags) {
        __scExec(arg0, value1, value2);
    }
}

OSMesgQueue *Sched_GetInterruptQ(OSSched *sc) {
    return &sc->interruptQ;
}

void Sched_SubmitTask(OSSched *sc, OSScTask *t) {
    osSendMesg(&sc->cmdQ, (OSMesg) t, 1);
    osSendMesg(&sc->interruptQ, (OSMesg) 6, 1);
}
