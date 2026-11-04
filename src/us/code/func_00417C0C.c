/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/MISC/8043DF84.CPP, func_8043DFC8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/player_options.h"

typedef struct ControllerProfile {
    char pad0[0x224];
} ControllerProfile;

typedef struct StatusView {
    char pad[0xB0];
    s8 active;                 /* 0xB0 (Rage Wars 0x150) */
} StatusView;

typedef struct StatusStep {
    char pad[0x40];
} StatusStep;

extern ControllerProfile D_800F5EC0[4];
extern StatusStep D_8011AB98[];
extern CPlayerOptions D_8011AB58[];
extern s32 func_002855E8(ControllerProfile *slot);

void func_00417C0C(void) {
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
    settings = D_8011AB58;
    for (; i < 4; i++) {
        if (func_002855E8(&D_800F5EC0[i]) != 0) {
            settings[i].m_bActive = active;
            settings[i].m_nController = i;
            break;
        }
    }
}
