#include "common.h"
#include "structs/game.h"
#include "structs/controller.h"
#include "globals/game.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MULTI/8043E9A8_4.CPP, func_8043EA98_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

struct ObjRecAt20V2 {
    char pad0[0x20];
    char *unk20;
};

extern s32 gInSinglePlayerMenu;
extern s32 Controller_ConfirmPressed(char *);
extern void Music_RestoreVolume(void);
extern void func_00469374(void *, s32);

void TitleMenu_Single_2(void *arg0, struct ObjRecAt20V2 *menu) {
    GameSettings *globals;
    CPlayerOptions *settings;
    ControllerProfile *profile;
    s32 on;
    s32 i;
    globals = &gEngineApp_m_Settings;
    globals->multiplayer = 0;
    globals->playerCount = 1;
    for (i = 3; i >= 0; i--) {
        globals->slots[i].m_bActive = 0;
    }
    i = 0;
    settings = &gEngineApp_m_Settings.defaultSlot;
    on = 1;
    for (profile = gControllers; i < 4; i++) {
        if (Controller_ConfirmPressed(profile) != 0) {
            settings->m_bActive = on;
            gEngineApp_m_Settings.defaultSlot.m_nController = i;
            menu->unk20 = profile;
            break;
        }
        profile++;
    }
    Music_RestoreVolume();
    func_00469374(&gEngineApp, -1);
    gInSinglePlayerMenu = 1;
}
