#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/game.h"

extern s32 MenuItem_TextLength(MenuItem *a);
extern void sprintf(char *dst, char *fmt, ...);

/* Prints the time limit into the label, or "none". */
s32 FragTagMenu_TimeLimitLabel(MenuItem *a) {
    GameSettings *setup = &gEngineApp_m_Settings;

    if (setup->tagTimeLimit != 0) {
        sprintf(a->text + (MenuItem_TextLength(a) - 4), "  %02d", setup->tagTimeLimit);
    } else {
        sprintf(a->text + (MenuItem_TextLength(a) - 4), "none");
    }
    return 0;
}
