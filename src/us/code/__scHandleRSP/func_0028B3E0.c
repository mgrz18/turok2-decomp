#include "common.h"
#include "protos/sched.h"
#include "protos/ultra.h"

OSMesgQueue *Sched_GetInterruptQ(OSSched *sc) {
    return &sc->interruptQ;
}
