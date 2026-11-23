#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

#ifdef TARGET_PORT
#define T2_TEXT(s) ((s32 *) (s))
#else
#define T2_TEXT(s) (&s)
#endif

extern char D_800B4B84[]; /* the port: a string (gendata copies it as bytes, not as a swapped word) */
extern char D_800B4B8C[]; /* the port: a string (gendata copies it as bytes, not as a swapped word) */
extern char D_800B4B94[]; /* the port: a string (gendata copies it as bytes, not as a swapped word) */
extern s32 D_801309A8;

s32 CharNoteMenu_UpdatePrompt(MenuItem *arg0) {
    if (sCharNoteLoad != 0) {
        arg0->table = T2_TEXT(D_800B4B84);
    } else if (D_801309A8 != 0) {
        arg0->table = T2_TEXT(D_800B4B94);
    } else {
        arg0->table = T2_TEXT(D_800B4B8C);
    }
    return 0;
}
