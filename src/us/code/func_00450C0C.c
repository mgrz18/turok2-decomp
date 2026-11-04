#include "structs/vector.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/MISC/8043DF84.CPP, func_8043DFC8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "ultra_types.h"
#include "structs/controller.h"
#include "structs/player_options.h"
#include "structs/list.h"

struct Clip;
struct Clip {
    s16 mode;
    s16 frames;
};
struct Clip;
typedef struct Clip Clip;
struct NodeCode8028469C;
struct NodeCode8028469C {
    struct NodeCode8028469C *previous;
    struct NodeCode8028469C *next;
    Clip *key;
    CVector3 position;
    f32 value0;
    f32 value1;
    u32 amplitudeHandle;
    char pad24[8];
    u32 lifetimeHandle;
    char pad30[8];
    struct NodeCode8028469C **owner;
};
struct PoolRecAt28V1 {
    CList free;
    CList active;
    s32 baseLevel;
};
struct NodeCode8028469C;
typedef struct NodeCode8028469C NodeCode8028469C;
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
extern void func_00450C0C(void);
extern StatusStep D_8011AB98[];
extern CPlayerOptions D_8011AB58[];
extern s32 func_002855E8(ControllerProfile *);
extern s32 func_002855E8(ControllerProfile *slot);
void func_00450C0C(void) {
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
    settings = D_8011AB58;
    for (; i < 4; i++) {
        if (func_002855E8(&D_800F5EC0[i]) != 0) {
            settings[i].m_bActive = active;
            settings[i].m_nController = i;
            break;
        }
    }
}
