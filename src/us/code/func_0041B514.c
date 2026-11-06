/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/rodata.func_8043DD2C_us_rev1.800E2330.c, func_8043D5A8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern char D_800B58FC[];
extern char D_800B5918[];
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings_unlockedCheats;
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings_unlockedCheats;
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings_unlockedCheats;
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings_unlockedCheats;
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings_unlockedCheats;
s32 CheatMenu_TinyModeText(struct StateRecAt14 *item) {
    if (gEngineApp_m_Settings_unlockedCheats & 0x40) {
        item->unk8 |= 0x1000000;
    } else {
        item->unk8 &= ~0x1000000;
    }
    if (gEngineApp_m_Settings & 0x40) {
        item->unk14 = D_800B58FC;
    } else {
        item->unk14 = D_800B5918;
    }
    return 0;
}
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings_unlockedCheats;
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings_unlockedCheats;
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings_unlockedCheats;
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings_unlockedCheats;
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings_unlockedCheats;
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings_unlockedCheats;
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings_unlockedCheats;
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings_unlockedCheats;
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings;
extern s32 gEngineApp_m_Settings_unlockedCheats;
