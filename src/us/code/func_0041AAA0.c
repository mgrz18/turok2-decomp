/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043C688_de_11.c, func_8043CB30_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

struct ControllerProfile;
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
extern int func_00285628(struct ControllerProfile *object);
extern int func_00285670(struct ControllerProfile *object);
struct MenuRecAt20V4;
struct MenuRecAt20V4 {
    s16 state;
    char pad2[0x1E];
    s32 input;
};
struct MenuRecAt20V4;
typedef struct MenuRecAt20V4 MenuRecAt20V4;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct Triple_func_802683E0_de;
typedef struct Triple_func_802683E0_de Triple_func_802683E0_de;
struct Triple_func_802683E0_de;
struct Triple_func_802683E0_de {
    s32 x;
    f32 y;
    s32 z;
};
struct ControllerProfile;
typedef struct ControllerConnectionSample {
    u16 reserved;
    u8 connectionStatus;
    u8 flags;
} __attribute__((aligned(4))) ControllerConnectionSample;
struct ControllerProfile {
    u8 padding000[4];
    s8 port;
    u8 padding005[0x18 - 5];
    s32 inputButtons;
    ControllerConnectionSample inputConnection;
    s32 inputHistory[3];
    u8 padding02C[0xAC - 0x2C];
    s32 previousButtons;
    s32 buttons;
    ControllerConnectionSample connection;
    s32 history[2];
    u32 buttonState;
    s8 stickX;
    s8 stickY;
    s8 previousStickX;
    s8 previousStickY;
    u8 padding0C8[4];
    s32 mode;
    u8 padding0D0[0x224 - 0xD0];
};
struct ControllerProfile;
typedef struct ControllerProfile ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
extern f32 D_800C1274[];
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
extern f32 D_800B6D28;
extern f32 D_800C1274[];
s32 func_0041AAA0(void *unused, MenuRecAt20V4 *menu) {
    extern Triple_func_802683E0_de D_800C1F80;
    f32 t;
    t = D_800C1F80.y + D_800B6D28;
    D_800C1F80.y = t;
    if (t >= D_800C1274[1]) {
        D_800C1F80.y = t - D_800C1274[1];
    }
    if (menu->state == 2) {
        if (((s32 (*)(s32))func_00285628)(menu->input) != 0 && D_800C1F80.x > 0) {
            D_800C1F80.x--;
            D_800C1F80.y = 0.0f;
        } else if (((s32 (*)(s32))func_00285670)(menu->input) != 0) {
            if (D_800C1F80.x < D_800C1F80.z) {
                D_800C1F80.x++;
                D_800C1F80.y = 0.0f;
            }
        }
    }
    return 0;
}
