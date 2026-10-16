/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802591A0_de_13.c, func_80259C5C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;
extern f32 func_002119FC(f32 arg0, f32 arg1);
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
struct Angles;
typedef struct Angles Angles;
struct Bytes12;
typedef struct Bytes12 Bytes12;
struct ContextRecAt2BC0;
typedef struct ContextRecAt2BC0 ContextRecAt2BC0;
struct ControllerCode802591C0;
typedef struct ControllerCode802591C0 ControllerCode802591C0;
struct Font_func_80257360_de;
typedef struct Font_func_80257360_de Font_func_80257360_de;
struct Motion;
typedef struct Motion Motion;
struct ViewRecAtB0;
typedef struct ViewRecAtB0 ViewRecAtB0;
struct Angles;
struct Angles {
    u16 heading;
    u16 pitch;
    u16 target;
    s16 steps;
};
struct Bytes12;
struct Bytes12 {
    char bytes[12];
};
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
struct ControllerCode802591C0;
struct ControllerCode802591C0 {
    s16 group;
    s16 sample;
    s8 priority;
    s8 mode;
    u16 flags;
    s16 field8;
    s16 preset;
    s16 fieldC;
    s8 pitch;
    s8 fieldF;
    s16 yaw;
    s16 x;
    s16 y;
};
struct Motion;
struct Motion {
    char pad0[0x14];
    f32 heading;
    char pad18[0x48 - 0x18];
    f32 rate1;
    f32 rate2;
};
struct ViewRecAtB0;
struct ViewRecAtB0 {
    char pad0[0x10];
    Motion motion;
    Angles angles;
    char pad68[4];
    f32 pitch;
    f32 pitchStep;
    char pad74[0xB0 - 0x74];
    s32 presets;
};
extern f32 func_0026D210(s32);
extern float D_800A80FC;
extern float D_800A8100;
extern float D_800A8104;
extern float D_800A8108;
extern float D_800A810C;
extern float D_800B284C[];
extern struct Bytes12 *func_00271790(ContextRecAt2BC0 *, s32);
extern s16 func_0026D200(f32);
extern s32 func_002768A4(ControllerCode802591C0 *);
extern s32 func_002768B0(ControllerCode802591C0 *);
void func_00271F6C(ViewRecAtB0 *view, ControllerCode802591C0 *controller, s32 flags) {
    s16 x;
    s16 y;
    f32 fx;
    f32 fy;
    s32 turn;
    f32 diff;
    f32 position;
    s16 index;
    s16 next;
    f32 base;
    f32 result;
    struct Motion *motion;
    struct Angles *angles;
    motion = &view->motion;
    angles = &view->angles;
    motion->rate1 = motion->rate2 = 0.0f;
    if (controller->preset != -1) {
        *(struct Bytes12 *)angles = *func_00271790(view->presets, controller->preset);
    } else {
        angles->heading = func_0026D200(controller->yaw);
    }
    x = func_002768A4(controller);
    y = func_002768B0(controller);
    fy = y;
    fx = x;
    turn = (s16)(s32)(fx + ((func_002119FC(fy, fx)) < 0.0f ? -(func_002119FC(fy, fx)) : (func_002119FC(fy, fx))));
    angles->heading = func_0026D200(func_0026D210(angles->heading) + turn);
    if (flags & 0x40) {
        angles->heading = func_0026D200(func_0026D210(angles->heading) + D_800A80FC);
    }
    if (flags & 0x80) {
        angles->heading = func_0026D200(func_0026D210(angles->heading) - D_800A8100);
    }
    if (func_0026D210(angles->heading) >= D_800A8104) {
        angles->heading = func_0026D200(D_800A8104);
    }
    if (controller->flags & 2) {
        motion->heading = func_0026D210(angles->heading);
        diff = func_0026D210(angles->pitch);
        diff -= func_0026D210(angles->target);
        if (angles->steps == 0) {
            view->pitchStep = 0.0f;
        } else {
            view->pitchStep = diff / angles->steps;
        }
        view->pitch = func_0026D210(angles->pitch);
    }
    position = (func_0026D210(angles->heading) + D_800A8108) * D_800A810C;
    index = position;
    if (index != position) {
        next = index + 1;
        base = D_800B284C[index];
        result = base + (position - index) * (D_800B284C[next] - base);
    } else {
        result = D_800B284C[index];
    }
    motion->heading = result;
    angles->heading = func_0026D200(result);
}
