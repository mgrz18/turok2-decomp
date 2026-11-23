#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

#ifdef TARGET_PORT
#define T2_TEXT(s) ((s32 *) (s))
#else
#define T2_TEXT(s) (&s)
#endif

#ifdef TARGET_PORT
extern char sPakMenuText_Exit[]; /* the port: a string (gendata copies it as bytes, not as a swapped word) */
#else
extern s32 sPakMenuText_Exit;
#endif

s32 PakMenu_UpdateCancel(MenuItem *arg0) {
    if (gPakMenuAnyPak != 0) {
        arg0->table = T2_TEXT(sPakMenuText_Exit);
    }
    return 0;
}
