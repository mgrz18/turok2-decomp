/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/MISC/8043DF84.CPP, func_8043DFC8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/controller.h"
#include "structs/player_options.h"


typedef struct StatusView {
    char pad[0xB0];
    s8 active;                 /* 0xB0 (Rage Wars 0x150) */
} StatusView;

typedef struct StatusStep {
    char pad[0x40];
} StatusStep;

extern StatusStep D_8011AB98[];
extern CPlayerOptions gEngineApp_m_Settings_slots[];
extern s32 Controller_ConfirmPressed(ControllerProfile *slot);

void Mp_JoinPressingController(void) {
    StatusStep *status;
    CPlayerOptions *settings;
    s32 i;
    s32 active;
    i = 3;
    status = D_8011AB98;
    for (; i >= 0; i--) {
        ((StatusView *)status)->active = 0;
        status--;
    }
    i = 0;
    active = 1;
    settings = gEngineApp_m_Settings_slots;
    for (; i < 4; i++) {
        if (Controller_ConfirmPressed(&gControllers[i]) != 0) {
            settings[i].m_bActive = active;
            settings[i].m_nController = i;
            break;
        }
    }
}
