/* Boundary fixed in versions/cuts.us.txt: splat's func_0028D374 starts with 3 stray words (a jal
 * Shadow_DrawBlob / swc1 / addu tail that nothing executes); the real function
 * starts at 0x28D380 (splat's label there), 0x38C bytes. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_002A7F58.c,
 * func_802A71C0_de, with Turok 2's layouts. */

#include "common.h"
#include "globals/game.h"
#include "globals/frame.h"
#include "globals/scene.h"
#include "globals/engine.h"

typedef struct Marker {
    char text[15];
} Marker;
extern Marker sBootMarker;           /* the boot marker string */
#include "globals/save.h"          /* the saved players (0x1400) */

void memset(void *dst, s32 value, sptr size);
s32 Util_StrCmp(char *marker, char *name);
void Util_StrCpy(char *marker, char *name);
void osInitialize(void);
void Engine_Boot(void);

/* Whether a word may be overwritten on reset (Boot_CanOverwrite inlined). */
static __inline__ s32 isRestorable(uptr address) {
    if (address >= (uptr)gBootSignature && address < (uptr)gBootSignature + 0x10) {
        return 0;
    }
    if (address == (uptr)&gWarmBoot) {
        return 0;
    }
    if (gWarmBoot != 0) {
        return 1;
    }
    if (address >= (uptr)&gSavedPlayers && address < (uptr)&gSavedPlayers + 0x1400) {
        return 0;
    }
    if (address >= (uptr)&gEngineApp_m_Settings && address < (uptr)&gEngineApp_m_Settings + 0x184) {
        return 0;
    }
    if (address >= (uptr)gLoadSavePersistentData && address < (uptr)gLoadSavePersistentData + 0x5800) {
        return 0;
    }
    if (address >= (uptr)gGamePersistentData && address < (uptr)gGamePersistentData + 0x5000) {
        return 0;
    }
    if (address >= (uptr)&gEngineApp_m_Settings.match && address <= (uptr)&gEngineApp_m_Settings.match.unk4C) {
        return 0;
    }
    return 1;
}

/* Soft reset: checks the boot marker, and on a cold boot copies the resident
 * code and data back from the cartridge word by word; then clears the BSS,
 * in both passes skipping the protected regions. */
s32 func_0028D380(void) {
    u32 *src;
    u32 *dst;
    s32 count;
    u32 status;
    u32 *piStatus;
    char name[16];

    *(Marker *)name = sBootMarker;
    memset(&name[15], 0, 1);
    if (Util_StrCmp(gBootSignature, name) == 0) {
        gWarmBoot = 0;
    } else {
        gWarmBoot = 1;
        Util_StrCpy(gBootSignature, name);
    }
    if (gWarmBoot == 0) {
        src = (u32 *)0xB0001000;
        dst = (u32 *)0x80000400;
        piStatus = (u32 *)0xA4600010;
        count = 0x3FFFF;
        do {
            status = *piStatus & 3;
            while (status != 0) {
            }
            if (isRestorable((uptr)dst | 0x80000000)) {
                *dst = *src;
            }
            dst++;
            src++;
        } while (count-- != 0);
    }
    for (dst = (u32 *)gGfxYieldBuffer; (uptr)dst < (uptr)gImageEnd; dst++) {
        if (isRestorable((uptr)dst | 0x80000000)) {
            *dst = 0;
        }
    }
    osInitialize();
    Engine_Boot();
    return 0;
}

#include "m2c_macros.h"

void func_0028D70C(void) {

}

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

void func_0028D814(void) {

}
