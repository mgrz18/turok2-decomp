#include "common.h"
#include "protos/sched.h"
#include "protos/ultra.h"

void Sched_SubmitTask(OSSched *sc, OSScTask *t) {
    osSendMesg(&sc->cmdQ, (OSMesg) t, 1);
    osSendMesg(&sc->interruptQ, (OSMesg) 6, 1);
}
