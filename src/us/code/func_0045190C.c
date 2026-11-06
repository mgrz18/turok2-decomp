#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/menu.h"

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */
s32 sprintf(char *buf, const char *fmt, ...); /* sprintf */

extern char D_800B6698[];
extern char D_800B66B0[];
extern char D_800C0E9C[];  /* "%3d", earlier in the file */

/* Names the game speed: a fixed line at 1.0, otherwise the setting relative to 5. */
s32 MpMenu_PlayerSpeedLabel_2(MenuItem *item) {
    if (gEngineApp_m_Settings.speed == 1.0f) {
        item->text = D_800B66B0;
    } else {
        item->text = D_800B6698;
        sprintf(&item->text[MenuItem_TextLength_2(item) - 3], D_800C0E9C, gMpPlayerSpeed - 5);
    }
    return 0;
}
