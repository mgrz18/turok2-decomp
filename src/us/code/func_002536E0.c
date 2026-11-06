#include "common.h"
#include "structs/player.h"

extern s32 gEngineApp_m_Settings;

void CPlayer__UseSpecialPickup(CPlayer *arg0, s32 arg1) {

    if (!(gEngineApp_m_Settings & 0x10)) {
        if (arg1 < 0x1121) {
            if (arg1 < 0x111C) {
                goto block_3;
            }
        } else {
block_3:
            arg0->m_Data.m_SpecialPickupCount[arg1 - 0xFA0]--;
        }
    }
}
