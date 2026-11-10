#include "common.h"
#include "m2c_macros.h"

s32 Menu_Draw(sptr, sptr);

void PauseMenu_Update(s32 arg0, s32 arg1) {
    Menu_Draw(arg0, arg1);
}
