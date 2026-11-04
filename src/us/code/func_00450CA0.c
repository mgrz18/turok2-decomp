#include "structs/vector.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/MISC/8043DF84.CPP, func_8043E05C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

struct ControllerProfile;
#include "common.h"
#include "ultra_types.h"
#include "structs/list.h"

struct CVector3_t;
struct CVector3_t;
struct CVector3_t;
struct CVector3_t {
    f32 x;
    f32 y;
    f32 z;
};
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
typedef struct ControllerButtonHistory {
    s8 taps;
    s8 age;
    s8 repeat;
    s8 held;
} ControllerButtonHistory;
struct ControllerProfile;
struct ControllerProfile {
    s32 connected;
    s8 port;
    u8 padding005[3];
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 inputPreviousButtons;
    s32 inputButtons;
    u32 inputPressedButtons;
    s32 inputHistory[3];
    ControllerButtonHistory buttonHistory[32];
    s32 previousButtons;
    s32 buttons;
    u32 pressedButtons;
    s32 history[2];
    u32 buttonState;
    s8 stickX;
    s8 stickY;
    s8 previousStickX;
    s8 previousStickY;
    s32 pakActive;
    s32 mode;
    f32 pakAmount;
    f32 pakTimer;
    OSPfs pak;
    struct PoolRecAt28V1 effectPool;
    struct NodeCode8028469C effectNodes[3];
    s32 reset;
};
struct ControllerProfile;
typedef struct ControllerProfile ControllerProfile;
struct NodeCode8028469C;
typedef struct NodeCode8028469C NodeCode8028469C;
struct ControllerProfile;
extern struct ControllerProfile D_800F5EC0[4];
extern ControllerProfile *func_00450CA0(s32 *out);
extern s32 func_002855E8(ControllerProfile *);
extern s32 func_002855E8(ControllerProfile *slot);
ControllerProfile *func_00450CA0(s32 *out) {
    s32 i;
    ControllerProfile *s;
    i = 0;
    s = D_800F5EC0;
    while (i < 4) {
        if (func_002855E8(s) != 0) {
            *out = i;
            return s;
        }
        i++;
        s++;
    }
    *out = -1;
    return 0;
}
