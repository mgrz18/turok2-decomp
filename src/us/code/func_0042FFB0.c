#include "common.h"
#include "m2c_macros.h"
#include "protos/list.h"


void CCameraPool__FreeCamera(s32 arg0, s32 arg1) {
    CList__Remove(arg0 + 0x20, arg1);
    CList__AddHead(arg0 + 0xC, arg1);
}
