/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043F52C_de_6.c, func_80442488_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

struct ControllerProfile;
typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
extern int func_00285628(struct ControllerProfile *object);
extern int func_00285670(struct ControllerProfile *object);
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
struct ControllerProfile;
struct ControllerProfile;
struct ControllerProfile;
struct func_8022A404_S1;
struct func_8022A404_S1 {
    char pad0[0x20];
    int unk20;
};
struct func_8022A404_S1;
typedef struct func_8022A404_S1 func_8022A404_S1;
struct ControllerProfile;
struct ControllerProfile;
s32 func_0044F558(func_8022A404_S1 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 temp_s0;
    s32 var_s0;
    s32 var_v0;
    var_s0 = arg1;
    if (((s32 (*)(s32))func_00285628)(arg0->unk20) != 0) {
        var_s0 -= arg2;
        if (var_s0 < arg3) {
            var_s0 = arg3;
            if (arg5 != 0) {
                var_s0 = arg4;
            }
        }
    }
    var_v0 = var_s0;
    if (((s32 (*)(s32))func_00285670)(arg0->unk20) != 0) {
        temp_s0 = var_s0 + arg2;
        var_v0 = temp_s0;
        if (arg4 < temp_s0) {
            var_s0 = arg4;
            if (arg5 != 0) {
                var_s0 = arg3;
            }
            var_v0 = var_s0;
        }
    }
    return var_v0;
}
