/* WRONG CUT: real function 0x00418DA0, size 0xBC (through 0x00418E5C): splat cut the last
 * case and the return off as func_00418E4C. */
#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/game.h"


s32 BloodLustMenu_TextureSetLabel(MenuItem *a) {
    switch (gEngineApp_m_Settings.dmTextureSet) {
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
