#include "common.h"
#include "m2c_macros.h"

extern s32 gMantidQueen_pushed;

void MantidQueen_EnterPush(void) {
    gMantidQueen_pushed = 0;
}
