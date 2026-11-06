#include "structs/vector.h"
#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/menu.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MULTI/8043E9A8_4.CPP, func_8043EA98_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "gfx.h"
#include "structs/controller.h"
#include "structs/list.h"
#include "structs/table.h"
struct ObjRecAt20V2;
struct ObjRecAt20V2 {
    char pad0[0x20];
    char * unk20;
};
struct ObjRecAt20V2;
typedef struct ObjRecAt20V2 ObjRecAt20V2;
struct ObjRecAt20V2;
/* Turok 2 layouts (Rage Wars: 8 records of 0x96 bytes at 0xD0, local/mode at 0x1D/0x1E). */
extern void TitleMenu_Single(void *arg0, struct ObjRecAt20V2 *menu);
extern void Music_RestoreVolume();
extern s32 Controller_ConfirmPressed(char *);
extern void Music_RestoreVolume(void);
extern void func_00430454(char *, s32);
void TitleMenu_Single(void *arg0, struct ObjRecAt20V2 *menu) {
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
    func_00430454(&gEngineApp, -1);
    gInSinglePlayerMenu = 1;
}
