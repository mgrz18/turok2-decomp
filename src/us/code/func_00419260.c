/* WRONG CUT: real function 0x00419260, size 0xBC (through 0x0041931C): splat cut the last
 * case and the return off as func_0041930C. */
#include "common.h"
#include "structs/menu.h"
#include "globals/game.h"

extern char sBloodLustMenuText_TextureSetCastle[];
extern char sBloodLustMenuText_TextureSetSteelMill[];
extern char sBloodLustMenuText_TextureSetOakFort[];
extern char sBloodLustMenuText_TextureSetCatacombs[];
extern char sBloodLustMenuText_TextureSetJailhouse[];
extern char sBloodLustMenuText_TextureSetBigCity[];
extern char sBloodLustMenuText_TextureSetRedDragon[];
extern char sBloodLustMenuText_TextureSetPaintball[];
extern char sBloodLustMenuText_TextureSetMrHappy[];

s32 TeamBloodMenu_TextureSetLabel(MenuItem *a) {
    switch (gEngineApp_m_Settings.teamTextureSet) {
    case 0:
        a->text = sBloodLustMenuText_TextureSetCastle;
        break;
    case 1:
        a->text = sBloodLustMenuText_TextureSetSteelMill;
        break;
    case 2:
        a->text = sBloodLustMenuText_TextureSetOakFort;
        break;
    case 3:
        a->text = sBloodLustMenuText_TextureSetCatacombs;
        break;
    case 4:
        a->text = sBloodLustMenuText_TextureSetJailhouse;
        break;
    case 5:
        a->text = sBloodLustMenuText_TextureSetBigCity;
        break;
    case 6:
        a->text = sBloodLustMenuText_TextureSetRedDragon;
        break;
    case 7:
        a->text = sBloodLustMenuText_TextureSetPaintball;
        break;
    case 8:
        a->text = sBloodLustMenuText_TextureSetMrHappy;
        break;
    }
    return 0;
}
