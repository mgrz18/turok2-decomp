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
#ifdef TARGET_PORT
#else
#endif
#ifdef TARGET_PORT
#else
#endif
extern s32 D_801309A8;

s32 CharNoteMenu_UpdatePrompt(MenuItem *arg0) {
    if (sCharNoteLoad != 0) {
        arg0->table = T2_TEXT(sCharNoteMenuText_Load);
    } else if (D_801309A8 != 0) {
        arg0->table = T2_TEXT(sCharNoteMenuText_Delete_B4B94);
    } else {
        arg0->table = T2_TEXT(sCharNoteMenuText_Delete_B4B8C);
    }
    return 0;
}
