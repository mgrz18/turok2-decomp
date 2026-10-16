/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043E134_de_2.c, func_8043E05C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

struct ControllerProfile;
typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
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
extern ControllerProfile *func_00417CA0(s32 *out);
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
extern s32 func_002855E8(ControllerProfile *);
extern s32 func_002855E8(ControllerProfile *slot);
ControllerProfile *func_00417CA0(s32 *out) {
    extern ControllerProfile D_800F5EC0[4];
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
