#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"

extern s32 gEngineApp_m_Settings_paused;
extern char gPauseMenuDesc; /* death screen */

/* Whether the game is paused (gEngineApp_m_Settings_paused) or a menu other than the death
 * screen is open on the player's camera. */
s32 CPlayer__IsPausedOrInMenu(CPlayer *p) {
    Menu *menu;

    if (gEngineApp_m_Settings_paused) {
        return 1;
    }
    if (p->m_pCamera != NULL && (menu = p->m_pCamera->m_Menus.head) != NULL) {
        if (menu->desc != (MenuDesc *) &gPauseMenuDesc) {
            return 1;
        }
    }
    return 0;
}
