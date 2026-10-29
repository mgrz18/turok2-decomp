/* Boundary fixed in versions/cuts.us.txt: splat starts func_0028A018 two stray words early (`lwc1 $f0, 0x6F98($at)`,
 * `addu $a0, $s3, $zero`, never executed); the function really starts at 0x0028A020,
 * the thread entry func_0028ADF0 passes as &D_0028A020. Written whole as func_0028A020. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0028E910.c, func_0028E910, with Turok 2's layouts. */

#include "common.h"

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

extern s32 D_800B6E58;  /* reset pressed */
extern s32 D_800B6E5C;  /* retraces left before blanking */

extern s32 osRecvMesg(void *queue, void *msg, s32 flag);
extern s32 osSendMesg(void *queue, void *msg, s32 flag);
extern void func_0029A580(s32 black);   /* osViBlack */
extern void func_0028A17C(Sched *sc);
extern void func_0042454C(void);
extern void func_0028A43C(Sched *sc);
extern void func_0028A914(Sched *sc);
extern void func_00275A08(void);
extern void func_00275904(s32);
extern s32 func_00275F7C(void);
extern void func_00275EFC(s32);
extern void func_0028B2AC(Sched *sc);

/* Scheduler thread: serves retrace, RSP/RDP done, pre-NMI and fault messages forever. */
void func_0028A020(Sched *sc) {
    s32 msg;
    Client *client;

    while (1) {
        osRecvMesg(sc->queue, &msg, 1);
        switch (msg) {
            case 0x29A:
                if (D_800B6E58 != 0) {
                    D_800B6E5C = D_800B6E5C - 1;
                    if (D_800B6E5C <= 0) {
                        func_0029A580(1);
                        while (1) {
                        }
                    }
                }
                func_0028A17C(sc);
                func_0042454C();
                break;
            case 0x29B:
                func_0028A43C(sc);
                break;
            case 0x29C:
                func_0028A914(sc);
                break;
            case 0x29D:
                func_00275A08();
                func_00275904(-1);
                func_00275EFC(func_00275F7C());
                client = sc->clients;
                D_800B6E58 = 1;
                D_800B6E5C = 20;
                while (client != NULL) {
                    osSendMesg(client->queue, &sc->retraceMsg, 0);
                    client = client->next;
                }
                break;
            case 6:
                func_0028B2AC(sc);
                break;
        }
    }
}
