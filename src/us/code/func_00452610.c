#include "common.h"
#include "structs/game.h"
#include "structs/controller.h"
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MULTI/8043E9A8_4.CPP, func_8043EA98_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

struct Part {
    char pad[0x28];
    u8 active;         /* 0x28 */
};

struct ObjRecAt20V2 {
    char pad0[0x20];
    char *unk20;
};

typedef struct GameSession GameSession;

extern struct Part D_8011AB18;
extern signed char D_8011AB47;
extern GameSession D_800F6CB0;
extern s32 D_801308F8;
extern s32 func_002855E8(char *);
extern void func_00275A74(void);
extern void func_00430374(void *, s32);

void func_00452610(void *arg0, struct ObjRecAt20V2 *menu) {
    GameSettings *globals;
    struct Part *settings;
    ControllerProfile *profile;
    s32 on;
    s32 i;
    globals = &D_8011AAD8;
    globals->multiplayer = 0;
    globals->playerCount = 1;
    for (i = 3; i >= 0; i--) {
        globals->slots[i].m_bActive = 0;
    }
    i = 0;
    settings = &D_8011AB18;
    on = 1;
    for (profile = D_800F5EC0; i < 4; i++) {
        if (func_002855E8(profile) != 0) {
            settings->active = on;
            D_8011AB47 = i;
            menu->unk20 = profile;
            break;
        }
        profile++;
    }
    func_00275A74();
    func_00430374(&D_800F6CB0, -1);
    D_801308F8 = 1;
}
