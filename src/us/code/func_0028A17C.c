/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0028EAAC.c, func_0028EAAC, with Turok 2's layouts. */

#include "common.h"

typedef struct Client {
    /* 0x0 */ struct Client *next;
    /* 0x4 */ void *queue;
} Client;

typedef struct ScTask {
    /* 0x00 */ struct ScTask *next;
    /* 0x04 */ s32 state;
    /* 0x08 */ u8 pad8[0x10 - 0x8];
    /* 0x10 */ s32 type;
} ScTask;

typedef struct {
    /* 0x000 */ u8 pad0[0x78];
    /* 0x078 */ u8 cmdQueue[0x18];      /* OSMesgQueue */
    /* 0x090 */ u8 pad90[0x2E0 - 0x90];
    /* 0x2E0 */ Client *clients;
    /* 0x2E4 */ u8 pad2E4[4];
    /* 0x2E8 */ ScTask *gfxHead;
    /* 0x2EC */ u8 pad2EC[4];
    /* 0x2F0 */ ScTask *gfxTail;
    /* 0x2F4 */ ScTask *curRSPTask;
    /* 0x2F8 */ ScTask *curRDPTask;
    /* 0x2FC */ s32 frameCount;
    /* 0x300 */ s32 doAudio;
} Sched;

typedef struct {
    /* 0x000 */ u8 pad0[0x110];
    /* 0x110 */ s32 framebuffer;
    /* 0x114 */ u8 pad114[0x124 - 0x114];
    /* 0x124 */ u32 minRetraces;
} GfxTask;

extern s32 D_800B6E54;          /* retraces before the thread priority is restored */
extern u8 D_800D6D68[];         /* OSThread */
extern s32 D_800B6E7C;          /* its priority */
extern s32 D_8011AD88;          /* retraces since the last swap */
extern s32 D_8011F130;          /* swap pending */
extern s32 D_8011B098;          /* framebuffer just swapped in */
extern s32 D_8011B0A0;          /* framebuffer on screen */
extern s32 D_8011F138;          /* frames waiting to be shown */
extern GfxTask *D_8011AD18[];
extern u32 D_800F6CA4;          /* next frame to show */
extern u32 D_800C2210;          /* task count */
extern s32 D_800B6E50;          /* retrace counter */

extern s32 osRecvMesg(void *queue, void *msg, s32 flag);
extern s32 osSendMesg(void *queue, void *msg, s32 flag);
extern void func_0029B9B0(void *thread, s32 pri);   /* osSetThreadPri */
extern void func_0029AC90(s32 framebuffer);         /* osViSwapBuffer */
extern void func_0028AFF0(Sched *sc, ScTask *task);
extern void func_0028A43C(Sched *sc);
extern void func_0028B274(Sched *sc);
extern s32 func_0028ABE0(Sched *sc, ScTask **sp, ScTask **dp, s32 state);
extern void func_0028B0FC(Sched *sc, ScTask *sp, ScTask *dp);
extern void func_00284B94(void);

/* Retrace: queue new tasks, swap the framebuffer when due, start the next tasks, wake the clients. */
void func_0028A17C(Sched *sc) {
    ScTask *rspTask;
    ScTask *sp;
    ScTask *dp;
    ScTask *task;
    ScTask *next;
    Client *client;
    GfxTask *frame;
    s32 state;
    s32 fb;

    rspTask = NULL;
    sp = NULL;
    dp = NULL;
    if (D_800B6E54 != 0) {
        D_800B6E54 = D_800B6E54 - 1;
        if (D_800B6E54 == 0) {
            func_0029B9B0(D_800D6D68, D_800B6E7C);
        }
    }
    sc->frameCount = sc->frameCount + 1;
    while (osRecvMesg(sc->cmdQueue, &rspTask, 0) != -1) {
        func_0028AFF0(sc, rspTask);
    }
    D_8011AD88 = D_8011AD88 + 1;
    if (D_8011F130 != 0) {
        D_8011AD88 = 0;
        D_8011F130 = 0;
        fb = D_8011B098;
        D_8011B098 = 0;
        D_8011B0A0 = fb;
    }
    if (D_8011F138 != 0) {
        frame = D_8011AD18[D_800F6CA4];
        if (!((u32)(D_8011AD88 + 1) < frame->minRetraces)) {
            D_8011B098 = frame->framebuffer;
            func_0029AC90(frame->framebuffer);
            D_8011F138 = D_8011F138 - 1;
            D_8011F130 = D_8011F130 + 1;
            D_800F6CA4 = D_800F6CA4 + 1;
            if (!(D_800F6CA4 < D_800C2210)) {
                D_800F6CA4 = 0;
            }
        }
    }
    if (sc->curRSPTask == NULL && sc->curRDPTask == NULL) {
        task = sc->gfxHead;
        if (task != NULL && task->type == 1 && task->state == 0x32) {
            task->state = 2;
            sc->curRSPTask = sc->gfxHead;
            next = sc->gfxHead->next;
            sc->gfxHead = next;
            if (next == NULL) {
                sc->gfxTail = NULL;
            }
            func_0028A43C(sc);
        }
    }
    D_800B6E50 = D_800B6E50 + 1;
    if (D_800B6E50 == 1200) {
        *(u8 *)0 = 0;
    }
    if (sc->doAudio != 0 && sc->curRSPTask != NULL) {
        func_0028B274(sc);
    } else {
        state = (sc->curRSPTask == NULL) << 1 | (sc->curRDPTask == NULL);
        if (func_0028ABE0(sc, &sp, &dp, state) != state) {
            func_0028B0FC(sc, sp, dp);
        }
    }
    client = sc->clients;
    while (client != NULL) {
        osSendMesg(client->queue, sc, 0);
        client = client->next;
    }
    func_00284B94();
}
