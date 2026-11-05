#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"

extern s32 D_8011ACA4;
extern char D_00439640; /* death screen */

/* Whether the game is paused (D_8011ACA4) or a menu other than the death
 * screen is open on the player's camera. */
s32 func_00253FFC(CPlayer *p) {
    Menu *menu;

    if (D_8011ACA4) {
        return 1;
    }
    if (p->m_pCamera != NULL && (menu = p->m_pCamera->m_Menus.head) != NULL) {
        if (menu->desc != (MenuDesc *) &D_00439640) {
            return 1;
        }
    }
    return 0;
}
