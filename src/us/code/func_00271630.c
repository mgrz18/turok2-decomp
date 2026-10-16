/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80258800_de.c, func_80258A7C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
struct Vec3;
typedef struct Vec3 Vec3;
struct Vec3;
struct Vec3 {
    f32 x;
    f32 y;
    f32 z;
};
struct MenuRules;
typedef struct MenuRules MenuRules;
struct MenuRules;
struct MenuRules {
    char pad0[0x1C];
    s32 locked;
};
struct Voice;
typedef struct Voice Voice;
struct Voice;
struct Voice {
    s32 unk0;
    s32 sound;
    s32 unk8;
    s32 unkC;
    char pad10[0x28];
    s16 priority;
    s16 channel;
    char pad3C[0x64];
    s32 pending;
    s32 flags;
    char padA8[0x24];
};
struct MenuRules;
struct MenuRules;
struct ContextRecAt2BC0;
typedef struct ContextRecAt2BC0 ContextRecAt2BC0;
struct Font_func_80257360_de;
typedef struct Font_func_80257360_de Font_func_80257360_de;
struct Font_func_80257360_de;
struct Font_func_80257360_de {
    char pad0[4];
    struct Font_func_80257360_de *next;
    char pad8[4];
    struct Font_func_80257360_de *child;
};
struct ContextRecAt2BC0;
struct Font_func_80257360_de;
struct ContextRecAt2BC0 {
    char pad0[0x24];
    s32 field24;
    s32 field28;
    char *field2C;
    char pad30[0x7C - 0x30];
    struct Font_func_80257360_de *font;
    struct Font_func_80257360_de *resource;
    char list[0x104 - 0x84];
    s32 frame;
    u8 reserved108[8];
    MenuRules lock;
    u8 pad130[4];
    s32 track;
    char view[0x1D64 - 0x138];
    char panel[0x1D6C - 0x1D64];
    s32 field1D6C;
    char pad1D70[0x1DA8 - 0x1D70];
    char frames[0x10];
    char menu[4];
    Voice voices[17];
    char pad2B48[0x28];
    char *table2B70;
    char *table2B74;
    char *table2B78;
    char pad2B7C[0xC];
    s32 field2B88;
    s16 field2B8C;
    char pad2B8E[2];
    s32 field2B90;
    u8 field2B94;
    char pad2B95[3];
    s32 field2B98;
    s32 field2B9C;
    f32 red;
    f32 green;
    f32 blue;
    s32 field2BAC;
    s32 field2BB0;
    s32 field2BB4;
    s32 field2BB8;
    f32 alpha;
    char dialog[1];
};
struct ContextRecAt2BC0;
extern f32 D_800A80D0[];
extern s32 func_00270C80(void *, s32, Vec3, s32, s32);
void func_00271630(ContextRecAt2BC0 *arg0, s32 arg1, Vec3 arg2, s32 arg3, s32 arg4, f32 arg5) {
    arg0->alpha = arg5;
    func_00270C80(arg0, arg1, arg2, arg3, arg4);
    arg0->alpha = D_800A80D0[1];
}
extern s32 func_00270C80(void *, s32, Vec3, s32, s32);
