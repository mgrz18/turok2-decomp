/* t2sched.c: the game's scheduler (Acclaim's PR/sched.c), typed for the host.
 *
 * On the N64 the scheduler files (src/us/code/func_0028A020.c .. func_0028B3E8.c) reach the
 * OSSched and its OSScTasks through local views at the N64 offsets: on the host the message
 * queues, the thread and the pointers are larger, so those views overrun each other (the
 * scheduler thread started with its argument overwritten). This file is the same code over
 * T2Sched / T2ScTask (structs/sched.h), function by function, and port/cmake/link.cmake leaves
 * the original files out of the link. Behaviour is unchanged: tasks still go through
 * osSpTaskLoad/StartGo (t2ultra.c posts their SP/DP completions), frames are swapped by
 * osViSwapBuffer, and the clients get the retrace message.
 *
 * gScheduler stays the generated .bss (a byte view, twice its 0x304 N64 bytes: T2Sched is
 * smaller than that on LP64, checked in osCreateScheduler). */
#include "common.h"
#include "ultra_types.h"
#include "structs/sched.h"
#include "structs/frame.h"
#include "structs/game.h"
#include "globals/frame.h"
#include "globals/render.h"
#include "globals/engine.h"
#include "globals/sysrc.h"
#include "globals/framebuffer.h"
#include "protos/ultra.h"
#include "protos/cache.h"

#define SC_RETRACE_MSG 0x29A
#define SC_RSP_DONE_MSG 0x29B
#define SC_RDP_DONE_MSG 0x29C
#define SC_PRE_NMI_MSG 0x29D
#define SC_CMD_MSG 6

extern u8 gScheduler[];
extern s32 D_800B6E60;
extern s32 D_800B6E64;
extern u32 D_800F6CA8;

extern void Video_UpdateResolution(void);
extern void Music_FadeOut(void);
extern void Sound__StopAllExcept(s32);
extern void LoopSounds_StopAll(sptr);
extern s32 LoopSounds_Get(void);
extern void Controller_Service(void);
extern void func_00228BBC(CFrameData *frame);
extern s32 osSpTaskYielded(OSTask *task);
extern s32 osSpTaskYield(void);
extern s32 osSetIntMask(s32 mask);
extern void osViBlack(u8 black);

void __scMain(T2Sched *sc);
void __scHandleRetrace(T2Sched *sc);
void __scHandleRSP(T2Sched *sc);
void __scHandleRDP(T2Sched *sc);
s32 __scSchedule(T2Sched *sc, T2ScTask **sp, T2ScTask **dp, s32 availRCP);
void __scExec(T2Sched *sc, T2ScTask *sp, T2ScTask *dp);
void __scYield(T2Sched *sc);
void __scAppendList(T2Sched *sc, T2ScTask *t);
s32 __scTaskComplete(T2Sched *sc, T2ScTask *t);
void Sched_HandleCmdMsg(T2Sched *sc);

/* The time conversions of the original: OSTime ticks to seconds as f32. */
#define TICKS_TO_SEC(t) ((f32)((((t)) * 64) / 3) * 0.000001f)

/* func_0028ADF0 */
void osCreateScheduler(T2Sched *sc, void *stack, s32 priority, s32 arg3, u8 numFields) {
    if (sizeof(T2Sched) > 2 * 0x304) {
        *(volatile u8 *)0 = 0; /* gScheduler is generated as 2 x 0x304 bytes (gendata) */
    }
    sc->retraceMsg.type = 1;
    sc->audioListHead = NULL;
    sc->gfxListHead = NULL;
    sc->audioListTail = NULL;
    sc->gfxListTail = NULL;
    sc->curRSPTask = NULL;
    sc->curRDPTask = NULL;
    sc->clientList = NULL;
    sc->frameCount = 0;
    sc->doAudio = 0;
    sc->prenmiMsg.type = 4;
    osCreateMesgQueue(&sc->interruptQ, sc->intBuf, 8);
    osCreateMesgQueue(&sc->cmdQ, sc->cmdMsgBuf, 8);
    osSetEventMesg(4, &sc->interruptQ, (OSMesg)SC_RSP_DONE_MSG);
    osSetEventMesg(9, &sc->interruptQ, (OSMesg)SC_RDP_DONE_MSG);
    osSetEventMesg(0xE, &sc->interruptQ, (OSMesg)SC_PRE_NMI_MSG);
    osViSetEvent(&sc->interruptQ, (OSMesg)SC_RETRACE_MSG, numFields);
    osCreateThread(&sc->thread, 7, (void (*)(void *))__scMain, sc, stack, priority);
    osStartThread(&sc->thread);
}

/* func_0028AF00 */
void osScAddClient(T2Sched *sc, OSScClient *c, OSMesgQueue *msgQ) {
    s32 mask = osSetIntMask(1);
    c->msgQ = msgQ;
    c->next = sc->clientList;
    sc->clientList = c;
    osSetIntMask(mask);
}

/* func_0028AF58 */
void osScRemoveClient(T2Sched *sc, OSScClient *c) {
    OSScClient *cur = sc->clientList;
    OSScClient *prev = NULL;
    s32 mask = osSetIntMask(1);
    while (cur != NULL) {
        if (cur == c) {
            if (prev != NULL) {
                prev->next = cur->next;
            } else {
                sc->clientList = cur->next;
            }
            break;
        }
        prev = cur;
        cur = cur->next;
    }
    osSetIntMask(mask);
}

/* func_0028AFE8 */
OSMesgQueue *osScGetCmdQ(T2Sched *sc) {
    return &sc->cmdQ;
}

/* func_0028B3E0 */
OSMesgQueue *Sched_GetInterruptQ(T2Sched *sc) {
    return &sc->interruptQ;
}

/* func_0028B3E8 */
void Sched_SubmitTask(T2Sched *sc, T2ScTask *t) {
    osSendMesg(&sc->cmdQ, t, 1);
    osSendMesg(&sc->interruptQ, (OSMesg)SC_CMD_MSG, 1);
}

/* func_0028A020 */
void __scMain(T2Sched *sc) {
    OSMesg msg;
    OSScClient *client;

    while (1) {
        osRecvMesg(&sc->interruptQ, &msg, 1);
        switch ((s32)(sptr)msg) {
            case SC_RETRACE_MSG:
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
            case SC_RSP_DONE_MSG:
                __scHandleRSP(sc);
                break;
            case SC_RDP_DONE_MSG:
                __scHandleRDP(sc);
                break;
            case SC_PRE_NMI_MSG:
                Music_FadeOut();
                Sound__StopAllExcept(-1);
                LoopSounds_StopAll(LoopSounds_Get());
                client = sc->clientList;
                sSchedResetPressed = 1;
                sSchedResetBlankRetraces = 20;
                while (client != NULL) {
                    osSendMesg(client->msgQ, &sc->prenmiMsg, 0);
                    client = client->next;
                }
                break;
            case SC_CMD_MSG:
                Sched_HandleCmdMsg(sc);
                break;
        }
    }
}

/* func_0028A17C */
void __scHandleRetrace(T2Sched *sc) {
    T2ScTask *rspTask = NULL;
    T2ScTask *sp = NULL;
    T2ScTask *dp = NULL;
    T2ScTask *task;
    T2ScTask *next;
    OSScClient *client;
    CFrameData *frame;
    s32 state;
    u16 *fb;

    if (gDecompressorBoostRetraces != 0) {
        gDecompressorBoostRetraces = gDecompressorBoostRetraces - 1;
        if (gDecompressorBoostRetraces == 0) {
            osSetThreadPri(&gDecompressor.thread, gThreadPriorityDecompressorLow);
        }
    }
    sc->frameCount = sc->frameCount + 1;
    while (osRecvMesg(&sc->cmdQ, (OSMesg *)&rspTask, 0) != -1) {
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
            gPendingFB = (u16 *)(uptr)frame->m_pFrameBuffer;
            osViSwapBuffer((void *)(uptr)frame->m_pFrameBuffer);
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
        *(volatile u8 *)0 = 0;
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
        osSendMesg(client->msgQ, sc, 0);
        client = client->next;
    }
    Controller_Service();
}

/* The frame whose colour image is fb (the first one if none). */
static CFrameData *frameOf(void *fb) {
    CFrameData *frames = gEngineApp.m_FrameData;
    if ((uptr)fb == frames[0].m_pFrameBuffer) {
        return &frames[0];
    }
    if ((uptr)fb == frames[1].m_pFrameBuffer) {
        return &frames[1];
    }
    if ((uptr)fb == frames[2].m_pFrameBuffer) {
        return &frames[2];
    }
    return &frames[0];
}

/* func_0028A43C */
void __scHandleRSP(T2Sched *sc) {
    T2ScTask *t = sc->curRSPTask;
    T2ScTask *sp = NULL;
    T2ScTask *dp = NULL;
    CFrameData *frame;
    s32 state;

    sc->curRSPTask = NULL;
    if ((t->state & 0x10) && osSpTaskYielded(&t->list) != 0) {
        t->state |= 0x20;
        if ((t->flags & 7) == 3) {
            t->next = sc->gfxListHead;
            sc->gfxListHead = t;
            if (sc->gfxListTail == NULL) {
                sc->gfxListTail = t;
            }
        }
        if (t->flags & 0x20) {
            sGfxRspTime = sGfxRspTime + TICKS_TO_SEC(osGetTime() - sGfxRspResumeTime);
        }
    } else {
        t->state &= ~2;
        if (t->list.t.type == 1) {
            sSchedGfxWatchdog = 0;
            if (t->flags & 0x20) {
                sGfxRspTime = sGfxRspTime + TICKS_TO_SEC(osGetTime() - sGfxRspResumeTime);
                gSchedRspTotalTime = sGfxRspTime;
                gSchedRspTime = TICKS_TO_SEC(osGetTime() - sAudioTaskStartTime);
                frame = frameOf(t->framebuffer);
                CCache__PostDisplayListDone(0, frame->m_pDisplayList == (void *)gDisplayList0);
                if (osSendMesg(t->msgQ, t->msg, 0) == -1) {
                    *(volatile u8 *)0 = 0;
                }
            }
        } else {
            gSchedRspTime = TICKS_TO_SEC(osGetTime() - sAudioTaskStartTime);
            if (osSendMesg(t->msgQ, t->msg, 0) == -1) {
                *(volatile u8 *)0 = 0;
            }
        }
        __scTaskComplete(sc, t);
    }
    state = (sc->curRSPTask == NULL) << 1 | (sc->curRDPTask == NULL);
    if (__scSchedule(sc, &sp, &dp, state) != state) {
        __scExec(sc, sp, dp);
    }
}

/* func_0028A914 */
void __scHandleRDP(T2Sched *sc) {
    T2ScTask *sp = NULL;
    T2ScTask *dp = NULL;
    T2ScTask *t = sc->curRDPTask;
    CFrameData *frame;
    s32 state;

    t->state &= ~1;
    if (t->list.t.type == 1) {
        if (t->flags & 0x20) {
            gSchedRdpTime = TICKS_TO_SEC(osGetTime() - sRdpStartTime);
        }
        sc->curRDPTask = NULL;
        if (t->flags & 0x20) {
            frame = frameOf(t->framebuffer);
            gNumTicks3 = gNumTicks2;
            gNumTicks2 = D_800F6CA8;
            D_800F6CA8 = gNumTicks0;
            gNumTicks0 = gCurrentTick + 1;
            if (gNumTicks0 >= frame->m_nPredictTicks && gNumTicksWaiting == 0 && gNumTicksPending == 0) {
                gPendingFB = (u16 *)(uptr)frame->m_pFrameBuffer;
                osViSwapBuffer((void *)(uptr)frame->m_pFrameBuffer);
                gNumTicksPending++;
            } else {
                gNumTicksWaiting++;
                gWaitingFrames[gCurrentTickWaiting] = frame;
                gCurrentTickWaiting++;
                if (gCurrentTickWaiting >= gTotalFramebuffers) {
                    gCurrentTickWaiting = 0;
                }
            }
            func_00228BBC(frame);
        }
    }
    __scTaskComplete(sc, t);
    state = (sc->curRSPTask == NULL) << 1 | (sc->curRDPTask == NULL);
    if (__scSchedule(sc, &sp, &dp, state) != state) {
        __scExec(sc, sp, dp);
    }
}

/* A gfx task may run unless its frame buffer is on screen or about to be (func_0028B048). */
T2ScTask *__scTaskReady(T2ScTask *t) {
    if (t == NULL) {
        return NULL;
    }
    if (t->framebuffer == (void *)gCurrentFB) {
        return NULL;
    }
    if (t->framebuffer == osViGetCurrentFramebuffer()) {
        return NULL;
    }
    if (t->framebuffer != (void *)gPendingFB) {
        return t;
    }
    return NULL;
}

/* func_0028ABE0: picks the tasks for the free RSP (2) and RDP (1). */
s32 __scSchedule(T2Sched *sc, T2ScTask **sp, T2ScTask **dp, s32 availRCP) {
    T2ScTask *gfx = sc->gfxListHead;
    T2ScTask *audio = sc->audioListHead;
    s32 avail = availRCP;

    if (sc->doAudio != 0 && (avail & 2)) {
        if (gfx == NULL || !(gfx->flags & 0x10)) {
            *sp = audio;
            sc->doAudio = 0;
            avail &= ~2;
            sc->audioListHead = sc->audioListHead->next;
            if (sc->audioListHead == NULL) {
                sc->audioListTail = NULL;
            }
        } else {
            *sp = gfx;
            avail &= ~2;
        }
    } else if (__scTaskReady(gfx) != NULL) {
        switch (gfx->flags & 7) {
            case 3:
                if (gfx->state & 0x20) {
                    if (avail & 2) {
                        *sp = gfx;
                        avail &= ~2;
                        if (gfx->state & 1) {
                            *dp = gfx;
                            avail &= ~1;
                        }
                        sc->gfxListHead = sc->gfxListHead->next;
                        if (sc->gfxListHead == NULL) {
                            sc->gfxListTail = NULL;
                        }
                    }
                } else if (avail == 3) {
                    *dp = gfx;
                    *sp = gfx;
                    sc->gfxListHead = sc->gfxListHead->next;
                    avail = 0;
                    if (sc->gfxListHead == NULL) {
                        sc->gfxListTail = NULL;
                    }
                }
                break;
            case 2:
            case 6:
            case 7:
                if (gfx->state & 2) {
                    if (avail & 2) {
                        *sp = gfx;
                        avail &= ~2;
                    }
                } else if ((gfx->state & 1) && (avail & 1)) {
                    *dp = gfx;
                    sc->gfxListHead = sc->gfxListHead->next;
                    avail &= ~1;
                    if (sc->gfxListHead == NULL) {
                        sc->gfxListTail = NULL;
                    }
                }
                break;
            default: /* 1, 4, 5 */
                break;
        }
    }
    if (avail != availRCP) {
        avail = __scSchedule(sc, sp, dp, avail);
    }
    return avail;
}

/* func_0028AFF0 */
void __scAppendList(T2Sched *sc, T2ScTask *t) {
    if (t->list.t.type == 2) {
        if (sc->audioListTail != NULL) {
            sc->audioListTail->next = t;
        } else {
            sc->audioListHead = t;
        }
        sc->audioListTail = t;
        sc->doAudio = 1;
    } else {
        if (sc->gfxListTail != NULL) {
            sc->gfxListTail->next = t;
        } else {
            sc->gfxListHead = t;
        }
        sc->gfxListTail = t;
    }
    t->next = NULL;
    t->state = t->flags & 3;
}

/* func_0028B0A8 */
s32 __scTaskComplete(T2Sched *sc, T2ScTask *t) {
    if (t->state & 3) {
        return 0;
    }
    if (t->list.t.type != 1) {
        return 1;
    }
    if ((t->flags & 0x60) != 0x60) {
        return 1;
    }
    osViSwapBuffer(t->framebuffer);
    return 1;
}

/* func_0028B0FC */
void __scExec(T2Sched *sc, T2ScTask *sp, T2ScTask *dp) {
    s64 now;

    if (dp != NULL && (dp->flags & 0x20) && !(dp->state & 0x10)) {
        sRdpStartTime = osGetTime();
    }
    if (sp != NULL) {
        if (sp->list.t.type == 2) {
            osWritebackDCacheAll();
            sAudioTaskStartTime = osGetTime();
        } else if (sp->flags & 0x20) {
            now = osGetTime();
            sGfxRspResumeTime = now;
            if (!(sp->state & 0x10)) {
                sGfxTaskStartTime = now;
                sGfxRspTime = 0;
            }
        }
        sp->state &= ~0x30;
        osSpTaskLoad(&sp->list);
        osSpTaskStartGo(&sp->list);
        sc->curRSPTask = sp;
        if (sp == dp) {
            sc->curRDPTask = dp;
        }
    }
    if (dp != NULL && dp != sp) {
        osDpSetNextBuffer(dp->list.t.output_buff, *dp->list.t.output_buff_size);
        D_800B6E60 = 1;
        D_800B6E64 = 0;
        sc->curRDPTask = dp;
    }
}

/* func_0028B274 */
void __scYield(T2Sched *sc) {
    T2ScTask *t = sc->curRSPTask;
    if (t->list.t.type == 1) {
        t->state |= 0x10;
        osSpTaskYield();
    }
}

/* func_0028B2AC */
void Sched_HandleCmdMsg(T2Sched *sc) {
    T2ScTask *t = NULL;
    T2ScTask *sp = NULL;
    T2ScTask *dp = NULL;
    s32 state;

    while (osRecvMesg(&sc->cmdQ, (OSMesg *)&t, 0) != -1) {
        if (t->list.t.type == 1 && (t->flags & 0x20) && gDecompressorBoostRetraces == 0) {
            gDecompressorBoostRetraces = 2;
            osSetThreadPri(&gDecompressor.thread, gThreadPriorityDecompressorHigh);
        }
        __scAppendList(sc, t);
    }
    if (sc->doAudio != 0 && sc->curRSPTask != NULL) {
        __scYield(sc);
        return;
    }
    state = (sc->curRSPTask == NULL) << 1 | (sc->curRDPTask == NULL);
    if (__scSchedule(sc, &sp, &dp, state) != state) {
        __scExec(sc, sp, dp);
    }
}

/* gRefreshRate (globals/framebuffer.h): 50.0 or 60.0, set by CEngineApp__Main. */
f32 t2_gRefreshRate;
