#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/pak.h"

#ifdef TARGET_PORT
#define T2_TEXT(s) ((s32 *) (s))
#else
#define T2_TEXT(s) (&s)
#endif

#ifdef TARGET_PORT
#else
#endif

s32 PakMenu_UpdateCancel(MenuItem *arg0) {
    if (gPakMenuAnyPak != 0) {
        arg0->table = T2_TEXT(sPakMenuText_Exit);
    }
    return 0;
}
