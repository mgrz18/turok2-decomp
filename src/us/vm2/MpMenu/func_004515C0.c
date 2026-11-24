#include "common.h"
#include "globals/menu_text.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/game.h"

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */
extern void sprintf(char *dst, char *fmt, ...);


/* Names setting 0x27. */
s32 FragTagMenu_LevelLabel_2(MenuItem *item) {
    switch (gEngineApp_m_Settings.tagArena) {
    case 0:
        item->text = sBloodLustMenuText_LevelRandom;
        break;
    case 1:
        item->text = sFragTagMenuText_LevelMadhouse;
        break;
    case 2:
        item->text = sFragTagMenuText_LevelSnap;
        break;
    case 3:
        item->text = sFragTagMenuText_LevelSneaksNLadders;
        break;
    case 4:
        item->text = sFragTagMenuText_LevelPrimattica;
        break;
    case 5:
        item->text = sFragTagMenuText_LevelCrossfire;
        break;
    case 6:
        item->text = sFragTagMenuText_LevelYNGWTSF;
        break;
    }
    return 0;
}
