#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */
s32 sprintf(char *buf, const char *fmt, ...); /* sprintf */

/* Writes setting unk21 (or "none" when 0) over the end of the item's text. */
s32 func_00451AC4(MenuItem *item) {
    GameSettings *s = &gEngineApp_m_Settings;

    if (s->dmScoreLimit != 0) {
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "  %02d", s->dmScoreLimit);
    } else {
        sprintf(&item->text[MenuItem_TextLength_2(item) - 4], "none");
    }
    return 0;
}
