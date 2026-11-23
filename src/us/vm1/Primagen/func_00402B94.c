#include "common.h"
#include "m2c_macros.h"

s32 Cinema_RequestById(s32);

void MantidQueen_EnterIntro(void) {
    Cinema_RequestById(0x12C);
}
