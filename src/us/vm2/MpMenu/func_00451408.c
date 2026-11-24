#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/game.h"

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */
extern void sprintf(char *dst, char *fmt, ...);

/* Writes setting unk25 (or "none" when 0) over the end of the item's text. */
s32 FragTagMenu_TimeLimitLabel_2(MenuItem *item) {
    GameSettings *s = &gEngineApp_m_Settings;

    if (s->tagTimeLimit != 0) {
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "  %02d", s->tagTimeLimit);
    } else {
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "none");
    }
    return 0;
}
