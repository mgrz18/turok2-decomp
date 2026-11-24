#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/pak.h"

#define T2_TEXT(s) ((s32 *) (s))

#ifdef TARGET_PORT
#else
#endif

s32 PakMenu_UpdateCancel_2(MenuItem *arg0) {
    if (gPakMenuAnyPak != 0) {
        arg0->table = T2_TEXT(sPakMenuText_Exit);
    }
    return 0;
}
