/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80444CF8_de_3.c, func_8044421C_de.
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
s32 func_002855E8(char *);
extern char D_800F5C78;
s32 func_0041BD64(void) {
    extern u32 D_8011AAE0[];
 if(((s32 (*)(char *))func_00285628)(&D_800F5C78)) {
 switch(D_8011AAE0[0]) {
 case 8: D_8011AAE0[0]=32; break;
 case 16: D_8011AAE0[0]=8; break;
 case 32: D_8011AAE0[0]=16; break;
 }
 }
 if(((s32 (*)(char *))func_00285670)(&D_800F5C78) || func_002855E8(&D_800F5C78)) {
 switch(D_8011AAE0[0]) {
 case 8: D_8011AAE0[0]=16; break;
 case 16: D_8011AAE0[0]=32; break;
 case 32: D_8011AAE0[0]=8; break;
 }
 }
 return 0;
}
