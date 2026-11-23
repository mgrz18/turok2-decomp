#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

#ifdef TARGET_PORT
#define T2_TEXT(s) ((s32 *) (s))
#else
#define T2_TEXT(s) (&s)
#endif

#ifdef TARGET_PORT
extern char sCharNoteMenuText_Load[]; /* the port: a string (gendata copies it as bytes, not as a swapped word) */
#else
extern s32 sCharNoteMenuText_Load;
#endif
#ifdef TARGET_PORT
extern char sCharNoteMenuText_Delete_B4B8C[]; /* the port: a string (gendata copies it as bytes, not as a swapped word) */
#else
extern s32 sCharNoteMenuText_Delete_B4B8C;
#endif

s32 GameNoteMenu_UpdatePrompt(MenuItem *arg0) {
    if (sGameNoteLoad != 0) {
        arg0->table = T2_TEXT(sCharNoteMenuText_Load);
    } else {
        arg0->table = T2_TEXT(sCharNoteMenuText_Delete_B4B8C);
    }
    return 0;
}
