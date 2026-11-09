/* Boundary fixed in versions/cuts.us.txt: splat starts func_0028A018 two stray words early (`lwc1 $f0, 0x6F98($at)`,
 * `addu $a0, $s3, $zero`, never executed); the function really starts at 0x0028A020,
 * the thread entry osCreateScheduler passes as &D_0028A020. Written whole as __scMain. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0028E910.c, func_0028E910, with Turok 2's layouts. */

#include "common.h"
#include "globals/engine.h"
#include "protos/game.h"

typedef struct Client {
    /* 0x0 */ struct Client *next;
    /* 0x4 */ void *queue;
} Client;

typedef struct {
    /* 0x000 */ u8 pad0[0x20];
    /* 0x020 */ s16 retraceMsg;
    /* 0x022 */ u8 pad22[0x40 - 0x22];
    /* 0x040 */ u8 queue[0x18];         /* OSMesgQueue */
    /* 0x058 */ u8 pad58[0x2E0 - 0x58];
    /* 0x2E0 */ Client *clients;
} Sched;


extern s32 osRecvMesg(void *queue, void *msg, s32 flag);
extern s32 osSendMesg(void *queue, void *msg, s32 flag);
extern void osViBlack(s32 black);   /* osViBlack */
extern void __scHandleRetrace(Sched *sc);
extern void Video_UpdateResolution(void);
extern void __scHandleRSP(Sched *sc);
extern void __scHandleRDP(Sched *sc);
extern void Music_FadeOut(void);
extern void Sound__StopAllExcept(s32);
extern void LoopSounds_StopAll(s32);
extern void Sched_HandleCmdMsg(Sched *sc);

/* Scheduler thread: serves retrace, RSP/RDP done, pre-NMI and fault messages forever. */
void __scMain(Sched *sc) {
    s32 msg;
    Client *client;

    while (1) {
        osRecvMesg(sc->queue, &msg, 1);
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
                client = sc->clients;
                sSchedResetPressed = 1;
                sSchedResetBlankRetraces = 20;
                while (client != NULL) {
                    osSendMesg(client->queue, &sc->retraceMsg, 0);
                    client = client->next;
                }
                break;
            case 6:
                Sched_HandleCmdMsg(sc);
                break;
        }
    }
}
