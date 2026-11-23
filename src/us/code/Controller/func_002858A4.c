#include "common.h"
#include "globals/pak.h"
#include "protos/ultra.h"


void Controller_Unlock(void) {
    osSendMesg(&gPakAccessQueue, 0, 0);
}
