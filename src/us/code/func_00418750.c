/* WRONG CUT: real function 0x00418750, size 0xBC (through 0x0041880C): splat cut the last
 * case and the return off as func_004187FC. */
#include "common.h"
#include "structs/menu.h"
#include "globals/game.h"

extern char D_800B608C[];
extern char D_800B60A4[];
extern char D_800B60BC[];
extern char D_800B60D4[];
extern char D_800B60EC[];
extern char D_800B6104[];
extern char D_800B611C[];
extern char D_800B6134[];
extern char D_800B614C[];

s32 FragTagMenu_TextureSetLabel(MenuItem *a) {
    switch (gEngineApp_m_Settings.tagTextureSet) {
    case 0:
        a->text = D_800B608C;
        break;
    case 1:
        a->text = D_800B60A4;
        break;
    case 2:
        a->text = D_800B60BC;
        break;
    case 3:
        a->text = D_800B60D4;
        break;
    case 4:
        a->text = D_800B60EC;
        break;
    case 5:
        a->text = D_800B6104;
        break;
    case 6:
        a->text = D_800B611C;
        break;
    case 7:
        a->text = D_800B6134;
        break;
    case 8:
        a->text = D_800B614C;
        break;
    }
    return 0;
}
