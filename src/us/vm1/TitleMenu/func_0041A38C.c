#include "common.h"
#include "m2c_macros.h"

s32 Menu_Draw(sptr, sptr);

void PauseMenu_Update(sptr arg0, sptr arg1) { /* sptr: the menu and its pool (soak S4, --high-memory) */
    Menu_Draw(arg0, arg1);
}
