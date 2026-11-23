#include "common.h"
#include "structs/controller.h"


s32 Controller_IsConnected(s32 arg0) {
    return ((gContStatus[arg0].errno >> 3) ^ 1) & 1;
}
