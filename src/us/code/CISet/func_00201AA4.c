#include "common.h"
#include "ultra_types.h"
#include "protos/ultra.h"


void CMutex__Construct(CMutex *arg0) {
    arg0->count = 0;
    osCreateMesgQueue(&arg0->queue, &arg0->msg, 1);
}
