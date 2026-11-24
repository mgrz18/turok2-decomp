#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/pak.h"

#define T2_TEXT(s) ((s32 *) (s))

#ifdef TARGET_PORT
#else
#endif
#ifdef TARGET_PORT
#else
#endif

s32 GameNoteMenu_UpdateTitle_2(MenuItem *arg0) {
    if (sGameNoteLoad != 0) {
        arg0->table = T2_TEXT(sGameNoteMenuText_NoteInfo);
    } else {
        arg0->table = T2_TEXT(sGameNoteMenuText_DeleteNote);
    }
    return 0;
}
