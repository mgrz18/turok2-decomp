/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043F52C_de_6.c, func_804423BC_de.
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
s32 func_002855E8(s32);
s32 func_0041648C(func_8022A404_S1 *menu,s32 value,s32 step,s32 minimum,s32 maximum,s32 wrap) {
 if(((s32 (*)(s32))func_00285628)(menu->unk20))value-=step;
 if(((s32 (*)(s32))func_00285670)(menu->unk20)||func_002855E8(menu->unk20))value+=step;
 if(value<minimum) {
  value=minimum;if(wrap)value=maximum;
 } else if(value>maximum) {
  value=maximum;if(wrap)value=minimum;
 }
 return value;
}
