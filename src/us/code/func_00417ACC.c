/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/OVLGAME/8043D54C_4.CPP, func_8043DE50_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/controller.h"
#include "structs/player_options.h"
struct func_80250BD4_S1;
struct func_80250BD4_S1 {
    char pad0[0x20];
    ControllerProfile *unk20; /* Menu::controller */
};
struct func_80250BD4_S1;
typedef struct func_80250BD4_S1 func_80250BD4_S1;
struct SharedRecAt224;
struct SharedRecAt224 {
    char pad0[0x224];
    s32 unk224;
};
struct SharedRecAtA8;
typedef struct SharedRecAtA8 SharedRecAtA8;
extern void Menu_ClaimPressingController(func_80250BD4_S1 *arg0);
struct SharedRecAt224;
typedef struct SharedRecAt224 SharedRecAt224;
struct SharedRecAtA8;
struct SharedRecAtA8 {
    char pad0[0xA8];
    s8 unkA8;
};
void Music_RestoreVolume(void);
unsigned int Controller_ConfirmPressed(void *);
extern CPlayerOptions D_8011AB18;
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
    settings = &D_8011AB18;
    on = 1;
    for (profile = D_800F5EC0; i < 4; i++, profile++) {
        if (Controller_ConfirmPressed(profile) != 0) {
            settings->m_bActive = on;
            settings->m_nController = i;
            arg0->unk20 = profile;
            break;
        }
    }
    Music_RestoreVolume();
}
