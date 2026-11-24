/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0028EAAC.c, func_0028EAAC, with Turok 2's layouts. */
#include "common.h"
#include "protos/sched.h"
#include "protos/ultra.h"
#include "globals/frame.h"
#include "globals/render.h"
#include "globals/engine.h"
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
