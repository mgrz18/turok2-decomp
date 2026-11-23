#include "common.h"
#include "m2c_macros.h"
#include "protos/list.h"


void CCameraPool__FreeCamera(sptr arg0, sptr arg1) {
    CList__Remove(arg0 + 0x20, arg1);
    CList__AddHead(arg0 + 0xC, arg1);
}
