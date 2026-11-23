#include "common.h"
#include "globals/libultra.h"

/* libultra pigetcmdq.c (2.0I), as in ultralib: the PI manager's command queue, or NULL before
 * osCreatePiManager. Round 22 de-faked it (the m2c draft had two equal arms on an unset local). */
OSMesgQueue *osPiGetCmdQueue(void) {
    if (!__osPiDevMgr.active) {
        return NULL;
    } else {
        return __osPiDevMgr.cmdQueue;
    }
}
