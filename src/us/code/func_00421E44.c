#include "common.h"
#include "structs/menu.h"
#include "globals/pak.h"

#ifdef TARGET_PORT
#define T2_TEXT(s) ((s32 *) (s))
#else
#define T2_TEXT(s) (&s)
#endif

#ifdef TARGET_PORT
extern char sGameNoteMenuText_NoteInfo[]; /* the port: a string (gendata copies it as bytes, not as a swapped word) */
#else
extern s32 sGameNoteMenuText_NoteInfo;
#endif
#ifdef TARGET_PORT
extern char sGameNoteMenuText_DeleteNote[]; /* the port: a string (gendata copies it as bytes, not as a swapped word) */
#else
extern s32 sGameNoteMenuText_DeleteNote;
#endif

s32 GameNoteMenu_UpdateTitle(MenuItem *arg0) {
    if (sGameNoteLoad != 0) {
        arg0->table = T2_TEXT(sGameNoteMenuText_NoteInfo);
    } else {
        arg0->table = T2_TEXT(sGameNoteMenuText_DeleteNote);
    }
    return 0;
}
