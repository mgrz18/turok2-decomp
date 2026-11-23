#include "common.h"
#include "structs/controller.h"


s32 Controller_PakInserted(s32 arg0) {
    return gContStatus[arg0].status & 1;
}
