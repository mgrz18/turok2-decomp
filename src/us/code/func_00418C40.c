#include "common.h"
#include "structs/menu.h"

extern s8 gEngineApp_m_Settings_dmArena; /* gEngineApp_m_Settings + 0x22 */
extern char D_800B5F3C[];
extern char D_800B5F54[];
extern char D_800B5F6C[];
extern char D_800B5F84[];
extern char D_800B5F9C[];
extern char D_800B5FB4[];
extern char D_800B5FCC[];
extern char D_800B5FE4[];
extern char D_800B5FFC[];
extern char D_800B6014[];
extern char D_800B602C[];
extern char D_800B6044[];
extern char D_800B605C[];
extern char D_800B6074[];

s32 BloodLustMenu_LevelLabel(MenuItem *a) {
    switch (gEngineApp_m_Settings_dmArena) {
    case 0:
        a->text = D_800B5F3C;
        break;
    case 1:
        a->text = D_800B5F54;
        break;
    case 2:
        a->text = D_800B5F6C;
        break;
    case 3:
        a->text = D_800B5F84;
        break;
    case 4:
        a->text = D_800B5F9C;
        break;
    case 5:
        a->text = D_800B5FB4;
        break;
    case 6:
        a->text = D_800B5FCC;
        break;
    case 7:
        a->text = D_800B5FE4;
        break;
    case 8:
        a->text = D_800B5FFC;
        break;
    case 9:
        a->text = D_800B6014;
        break;
    case 10:
        a->text = D_800B602C;
        break;
    case 11:
        a->text = D_800B6044;
        break;
    case 12:
        a->text = D_800B605C;
        break;
    case 13:
        a->text = D_800B6074;
        break;
    }
    return 0;
}
