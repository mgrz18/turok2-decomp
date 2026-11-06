/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002A7F58.c, func_802A7554_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "globals/frame.h"
extern char gBootSignature;
extern char gSavedPlayers;
extern char gEngineApp_m_Settings;
extern char gLoadSavePersistentData;
extern char gGamePersistentData;
extern char gEngineApp_m_Settings_match;
/* Turok 2 region sizes differ from Rage Wars (0x430, 0x584, 0xB0). */
s32 Boot_CanOverwrite(u32 address) {
    address |= 0x80000000;
    if (((address) >= (u32)&(gBootSignature) && (address) < (u32)&(gBootSignature) + (0x10)) || address == (u32)&gWarmBoot) {
        return 0;
    }
    if (gWarmBoot != 0) {
        return 1;
    }
    if (((address) >= (u32)&(gSavedPlayers) && (address) < (u32)&(gSavedPlayers) + (0x1400))) {
        return 0;
    }
    if (((address) >= (u32)&(gEngineApp_m_Settings) && (address) < (u32)&(gEngineApp_m_Settings) + (0x184))) {
        return 0;
    }
    if (((address) >= (u32)&(gLoadSavePersistentData) && (address) < (u32)&(gLoadSavePersistentData) + (0x5800))) {
        return 0;
    }
    if (((address) >= (u32)&(gGamePersistentData) && (address) < (u32)&(gGamePersistentData) + (0x5000))) {
        return 0;
    }
    if (address >= (u32)&gEngineApp_m_Settings_match && address <= (u32)&gEngineApp_m_Settings_match + 0x4C) {
        return 0;
    }
    return 1;
}
