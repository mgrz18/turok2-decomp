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
