#include "common.h"
#include "globals/game.h"


/* Whether the unlock flag behind an id is set; ids without a flag count as set. */
s32 Generator_IsPickupUnlocked(s32 id) {
    switch (id) {
        default:
            return 1;
        case 0x5EB:
        case 0x842:
            return gEngineApp_m_Settings.weaponEnabled[1] == 1;
        case 0x5EC:
        case 0x843:
            return gEngineApp_m_Settings.weaponEnabled[2] == 1;
        case 0x5ED:
        case 0x844:
            return gEngineApp_m_Settings.weaponEnabled[3] == 1;
        case 0x5EE:
        case 0x845:
            return gEngineApp_m_Settings.weaponEnabled[4] == 1;
        case 0x5EF:
        case 0x846:
            return gEngineApp_m_Settings.weaponEnabled[5] == 1;
        case 0x5F0:
        case 0x847:
            return gEngineApp_m_Settings.weaponEnabled[6] == 1;
        case 0x5F1:
        case 0x848:
            return gEngineApp_m_Settings.weaponEnabled[7] == 1;
        case 0x5F3:
        case 0x84A:
            break;
    }
    return gEngineApp_m_Settings.weaponEnabled[9] == 1;
}
