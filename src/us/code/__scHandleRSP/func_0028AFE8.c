#include "common.h"
#include "protos/sched.h"
#include "protos/ultra.h"

OSMesgQueue *osScGetCmdQ(OSSched *sc) {
    return &sc->cmdQ;
}
