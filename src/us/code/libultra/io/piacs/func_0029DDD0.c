#include "common.h"
#include "globals/libultra.h"
#include "protos/ultra.h"


void __osPiRelAccess(void) {
    osSendMesg(&__osPiAccessQueue, 0, 0);
}
