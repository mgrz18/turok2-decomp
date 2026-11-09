/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002A7F58.c, func_802A7554_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "globals/scene.h"
#include "common.h"
#include "globals/engine.h"

#include "common.h"
#include "globals/frame.h"
#include "globals/save.h"
#include "globals/game.h"
/* Turok 2 region sizes differ from Rage Wars (0x430, 0x584, 0xB0). */
s32 Boot_CanOverwrite(u32 address) {
    address |= 0x80000000;
    if (((address) >= (uptr)gBootSignature && (address) < (uptr)gBootSignature + (0x10)) || address == (uptr)&gWarmBoot) {
        return 0;
    }
    if (gWarmBoot != 0) {
        return 1;
    }
    if (((address) >= (uptr)&(gSavedPlayers) && (address) < (uptr)&(gSavedPlayers) + (0x1400))) {
        return 0;
    }
    if (((address) >= (uptr)&(gEngineApp_m_Settings) && (address) < (uptr)&(gEngineApp_m_Settings) + (0x184))) {
        return 0;
    }
    if (((address) >= (uptr)&(gLoadSavePersistentData) && (address) < (uptr)&(gLoadSavePersistentData) + (0x5800))) {
        return 0;
    }
    if (((address) >= (uptr)&(gGamePersistentData) && (address) < (uptr)&(gGamePersistentData) + (0x5000))) {
        return 0;
    }
    if (address >= (uptr)&gEngineApp_m_Settings_match && address <= (uptr)&gEngineApp_m_Settings_match + 0x4C) {
        return 0;
    }
    return 1;
}
