/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/MISC/8043DF84.CPP, func_8043DFC8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef signed int s32;

typedef struct ControllerProfile {
    char pad0[0x224];
} ControllerProfile;

/* Turok 2: 4 records of 0x40 bytes (Rage Wars: 8 of 150). */
typedef struct PlayerSettings {
    char pad0[0x28];
    s8 active;                 /* 0x28 (Rage Wars 0x78) */
    char pad29[6];
    s8 selected;               /* 0x2F (Rage Wars 0x7F) */
    char pad30[0x40 - 0x30];
} PlayerSettings;

typedef struct StatusView {
    char pad[0xB0];
    s8 active;                 /* 0xB0 (Rage Wars 0x150) */
} StatusView;

typedef struct StatusStep {
    char pad[0x40];
} StatusStep;

extern ControllerProfile D_800F5EC0[4];
extern StatusStep D_8011AB98[];
extern PlayerSettings D_8011AB58[];
extern s32 func_002855E8(ControllerProfile *slot);

void func_00417C0C(void) {
    StatusStep *status;
    PlayerSettings *settings;
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
            settings[i].active = active;
            settings[i].selected = i;
            break;
        }
    }
}
