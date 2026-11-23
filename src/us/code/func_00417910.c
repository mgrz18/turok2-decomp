#include "common.h"
#include "globals/menu_text.h"
#include "structs/menu.h"
#include "globals/game.h"


/* Label of a weapon toggle in the weapons menu: "<weapon> on/off". */
s32 WeaponsMenu_ToggleLabel(MenuItem *a) {
    switch (a->index - 2) {
    default:
        a->text = "unknown weapon";
        break;
    case 0:
        if (gEngineApp_m_Settings.weaponEnabled[1]) {
            a->text = sWeaponsMenuText_ChargeDartRifleOn;
        } else {
            a->text = sWeaponsMenuText_ChargeDartRifleOff;
        }
        break;
    case 1:
        if (gEngineApp_m_Settings.weaponEnabled[2]) {
            a->text = sWeaponsMenuText_FirestormOn;
        } else {
            a->text = sWeaponsMenuText_FirestormOff;
        }
        break;
    case 2:
        if (gEngineApp_m_Settings.weaponEnabled[3]) {
            a->text = sWeaponsMenuText_AssaultRifleOn;
        } else {
            a->text = sWeaponsMenuText_AssaultRifleOff;
        }
        break;
    case 3:
        if (gEngineApp_m_Settings.weaponEnabled[4]) {
            a->text = sWeaponsMenuText_PlasmaRifleOn;
        } else {
            a->text = sWeaponsMenuText_PlasmaRifleOff;
        }
        break;
    case 4:
        if (gEngineApp_m_Settings.weaponEnabled[5]) {
            a->text = sWeaponsMenuText_CerebralBoreOn;
        } else {
            a->text = sWeaponsMenuText_CerebralBoreOff;
        }
        break;
    case 5:
        if (gEngineApp_m_Settings.weaponEnabled[6]) {
            a->text = sWeaponsMenuText_GrenadeLauncherOn;
        } else {
            a->text = sWeaponsMenuText_GrenadeLauncherOff;
        }
        break;
    case 6:
        if (gEngineApp_m_Settings.weaponEnabled[7]) {
            a->text = sWeaponsMenuText_ScorpionLauncherOn;
        } else {
            a->text = sWeaponsMenuText_ScorpionLauncherOff;
        }
        break;
    case 7:
        if (gEngineApp_m_Settings.weaponEnabled[9]) {
            a->text = sWeaponsMenuText_TorpedoLauncherOn;
        } else {
            a->text = sWeaponsMenuText_TorpedoLauncherOff;
        }
        break;
    }
    return 0;
}
