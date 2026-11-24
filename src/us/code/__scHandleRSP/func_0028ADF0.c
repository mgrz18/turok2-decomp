/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8028DF90_de_7.c, func_8028F754_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "common.h"
#include "protos/sched.h"
#include "protos/ultra.h"

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
