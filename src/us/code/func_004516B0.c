#include "common.h"
#include "structs/game.h"
#include "structs/menu.h"
#include "globals/game.h"

s32 MenuItem_TextLength_2(MenuItem *item); /* length of the item's text line */
s32 sprintf(char *buf, const char *fmt, ...); /* sprintf */

extern char sBloodLustMenuText_TextureSetCastle[];
extern char sBloodLustMenuText_TextureSetSteelMill[];
extern char sBloodLustMenuText_TextureSetOakFort[];
extern char sBloodLustMenuText_TextureSetCatacombs[];
extern char sBloodLustMenuText_TextureSetJailhouse[];
extern char sBloodLustMenuText_TextureSetBigCity[];
extern char sBloodLustMenuText_TextureSetRedDragon[];
extern char sBloodLustMenuText_TextureSetPaintball[];
extern char sBloodLustMenuText_TextureSetMrHappy[];

/* Names setting 0x28. */
s32 func_004516B0(MenuItem *item) {
    switch (gEngineApp_m_Settings.tagTextureSet) {
    case 0:
        item->text = sBloodLustMenuText_TextureSetCastle;
        break;
    case 1:
        item->text = sBloodLustMenuText_TextureSetSteelMill;
        break;
    case 2:
        item->text = sBloodLustMenuText_TextureSetOakFort;
        break;
    case 3:
        item->text = sBloodLustMenuText_TextureSetCatacombs;
        break;
    case 4:
        item->text = sBloodLustMenuText_TextureSetJailhouse;
        break;
    case 5:
        item->text = sBloodLustMenuText_TextureSetBigCity;
        break;
    case 6:
        item->text = sBloodLustMenuText_TextureSetRedDragon;
        break;
    case 7:
        item->text = sBloodLustMenuText_TextureSetPaintball;
        break;
    case 8:
        item->text = sBloodLustMenuText_TextureSetMrHappy;
        break;
    }
    return 0;
}
