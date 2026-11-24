#include "common.h"
#include "m2c_macros.h"

s32 Menu_Draw_2(sptr, sptr);

void PauseMenu_Update_2(sptr arg0, sptr arg1) { /* sptr: the menu and its pool (soak S4, --high-memory) */
    Menu_Draw_2(arg0, arg1);
}
