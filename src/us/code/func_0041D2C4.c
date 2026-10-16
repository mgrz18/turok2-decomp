/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80445964_de_7.c, func_80445964_de.
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
struct MenuRecAt20V1;
struct MenuRecAt20V1 {
    s16 state;
    char pad2[0x1A];
    s32 id;
    s32 input;
};
struct Spinner;
typedef struct Spinner Spinner;
struct Spinner;
struct Spinner {
    s32 value;
    f32 timer;
    s32 max;
    char padC[0xC];
};
struct MenuRecAt20V1;
typedef struct MenuRecAt20V1 MenuRecAt20V1;
struct ControllerProfile;
struct ControllerProfile;
extern f32 D_800B6D28;
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
extern char D_80119870[];
extern Spinner D_800C1FB0[];
extern s32 func_0025340C(char *, s32);
s32 func_0041D2C4(void *unused, MenuRecAt20V1 *menu) {
    s32 i;
    f32 t;
    i = func_0025340C(D_80119870, menu->id);
    t = D_800C1FB0[i].timer + D_800B6D28;
    D_800C1FB0[i].timer = t;
    if (t >= (15.0f)) {
        D_800C1FB0[i].timer = t - (15.0f);
    }
    if (menu->state == 1) {
        if (((s32 (*)(s32))func_00285628)(menu->input) != 0 && D_800C1FB0[i].value > 0) {
            D_800C1FB0[i].value--;
            D_800C1FB0[i].timer = 0.0f;
        } else if (((s32 (*)(s32))func_00285670)(menu->input) != 0) {
            if (D_800C1FB0[i].value < D_800C1FB0[i].max) {
                D_800C1FB0[i].value++;
                D_800C1FB0[i].timer = 0.0f;
            }
        }
    }
    return 0;
}
