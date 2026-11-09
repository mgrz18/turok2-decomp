#include "common.h"
#include "globals/libultra.h"
#include "protos/ultra.h"


void __osSiRelAccess(void) {
    osSendMesg(&__osSiAccessQueue, 0, 0);
}
