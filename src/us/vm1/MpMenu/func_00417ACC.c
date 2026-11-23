/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/OVLGAME/8043D54C_4.CPP, func_8043DE50_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/controller.h"
#include "structs/player_options.h"
#include "globals/game.h"
struct func_80250BD4_S1;
#ifdef TARGET_PORT
/* Menu::controller at its host offset (the N64 0x20 is inside Menu::desc on the host: a menu that
 * claimed a controller had its desc replaced by the profile; soak S5, the cheats menu) */
#include "structs/menu.h"
struct func_80250BD4_S1 {
    char pad0[__builtin_offsetof(Menu, controller)];
    ControllerProfile *unk20; /* Menu::controller */
};
#else
struct func_80250BD4_S1 {
    char pad0[0x20];
    ControllerProfile *unk20; /* Menu::controller */
};
#endif
struct func_80250BD4_S1;
typedef struct func_80250BD4_S1 func_80250BD4_S1;
struct SharedRecAtA8;
typedef struct SharedRecAtA8 SharedRecAtA8;
extern void Menu_ClaimPressingController(func_80250BD4_S1 *arg0);
struct SharedRecAtA8;
struct SharedRecAtA8 {
    char pad0[0xA8];
    s8 unkA8;
};
void Music_RestoreVolume(void);
unsigned int Controller_ConfirmPressed(void *);
extern struct SharedRecAtA8 D_8011AB98[];
void Menu_ClaimPressingController(func_80250BD4_S1 *arg0) {
    CPlayerOptions *settings;
    s8 *status;
    ControllerProfile *profile;
    s32 on;
    s32 i;
    i = 3;
    status = (s8 *)&D_8011AB98[0];
    for (; i >= 0; i--) {
        ((SharedRecAtA8 *)status)->unkA8 = 0;
        status -= 0x40;
    }
    i = 0;
    settings = &gEngineApp_m_Settings.defaultSlot;
    on = 1;
    for (profile = gControllers; i < 4; i++, profile++) {
        if (Controller_ConfirmPressed(profile) != 0) {
            settings->m_bActive = on;
            settings->m_nController = i;
            arg0->unk20 = profile;
            break;
        }
    }
    Music_RestoreVolume();
}
