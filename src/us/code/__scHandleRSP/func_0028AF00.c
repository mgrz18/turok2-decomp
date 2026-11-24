#include "common.h"
#include "protos/sched.h"
#include "protos/ultra.h"

void osScAddClient(OSSched *arg0, OSScClient *arg1, OSMesgQueue *arg2) {
    s32 temp_v0;

    temp_v0 = osSetIntMask(1);
    arg1->msgQ = arg2;
    arg1->next = arg0->clientList;
    arg0->clientList = arg1;
    osSetIntMask(temp_v0);
}
