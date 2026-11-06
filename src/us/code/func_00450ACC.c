/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/OVLGAME/8043D54C_4.CPP, func_8043DE50_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/controller.h"
#include "globals/game.h"
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
struct SharedRecAt148;
typedef struct SharedRecAt148 SharedRecAt148;
struct Shared_func_8043DEDC_S3;
typedef struct Shared_func_8043DEDC_S3 Shared_func_8043DEDC_S3;
extern void Menu_ClaimPressingController_2(func_80250BD4_S1 *arg0);
struct SharedRecAt224;
typedef struct SharedRecAt224 SharedRecAt224;
struct SharedRecAt148;
struct SharedRecAt148 {
    char pad0[0xA8];
    s8 unkA8; /* Turok 2: 0x40-byte records */
};
struct Shared_func_8043DEDC_S3;
struct Shared_func_8043DEDC_S3 {
    char pad0[0x28];
    s8 unk28;
    char pad29[0x6];
    s8 unk2F;
};
void Music_RestoreVolume(void);
unsigned int Controller_ConfirmPressed(void *);
extern struct SharedRecAt148 D_8011AB98[];
void Menu_ClaimPressingController_2(func_80250BD4_S1 *arg0) {
    Shared_func_8043DEDC_S3 *settings;
    s8 *status;
    ControllerProfile *profile;
    s32 on;
    s32 i;
    i = 3;
    status = (s8 *)&D_8011AB98[0];
    for (; i >= 0; i--) {
        ((SharedRecAt148 *)status)->unkA8 = 0;
        status -= 0x40;
    }
    i = 0;
    settings = (Shared_func_8043DEDC_S3 *)&gEngineApp_m_Settings.defaultSlot;
    on = 1;
    for (profile = gControllers; i < 4; i++, profile++) {
        if (Controller_ConfirmPressed(profile) != 0) {
            settings->unk28 = on;
            settings->unk2F = i;
            arg0->unk20 = profile;
            break;
        }
    }
    Music_RestoreVolume();
}
