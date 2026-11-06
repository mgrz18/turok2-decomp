#include "structs/vector.h"
#include "globals/game.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/MISC/8043DF84.CPP, func_8043DFC8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/controller.h"
#include "structs/player_options.h"
#include "structs/list.h"
struct StatusView;
struct StatusView {
    char pad[0xB0];
    s8 active;                 /* 0xB0 (0x150) */
};
struct StatusView;
typedef struct StatusView StatusView;
struct StatusStep;
typedef struct StatusStep StatusStep;
struct StatusStep;
struct StatusStep {
    char pad[0x40];
};
extern void Mp_JoinPressingController_2(void);
extern StatusStep D_8011AB98[];
extern s32 Controller_ConfirmPressed(ControllerProfile *);
extern s32 Controller_ConfirmPressed(ControllerProfile *slot);
void Mp_JoinPressingController_2(void) {
    StatusStep *status;
    CPlayerOptions *settings;
    s32 i;
    s32 active;
    i = 3; /* 4 entries in Turok 2 (8 in Rage Wars) */
    status = D_8011AB98;
    for (; i >= 0; i--) {
        ((StatusView *)status)->active = 0;
        status--;
    }
    i = 0;
    active = 1;
    settings = gEngineApp_m_Settings.slots;
    for (; i < 4; i++) {
        if (Controller_ConfirmPressed(&gControllers[i]) != 0) {
            settings[i].m_bActive = active;
            settings[i].m_nController = i;
            break;
        }
    }
}
