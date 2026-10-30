/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_9.CPP, func_8044A170_de, with Turok 2's layouts. */
#include "common.h"

typedef struct Vec5 {
    f32 v[6];
} Vec5;

typedef struct Viewport {
    s16 vscale[4];
    s16 vtrans[4];
} Viewport;

/* The view: camera, viewport, fog and clipping state. */
typedef struct View {
    /* 0x000 */ u8 pad0[0xC];
    /* 0x00C */ f32 scaleX;
    /* 0x010 */ f32 scaleY;
    /* 0x014 */ u8 pad14[0x24 - 0x14];
    /* 0x024 */ u8 camera[0x94 - 0x24];
    /* 0x094 */ Vec5 a;
    /* 0x0AC */ u8 padAC[0xE8 - 0xAC];
    /* 0x0E8 */ f32 unkE8;
    /* 0x0EC */ f32 unkEC;
    /* 0x0F0 */ u8 padF0[0x12C - 0xF0];
    /* 0x12C */ f32 orientation[3];
    /* 0x138 */ f32 unk138;
    /* 0x13C */ u8 pad13C[0x14C - 0x13C];
    /* 0x14C */ u8 matA[0x40];
    /* 0x18C */ u8 matB[0x40];
    /* 0x1CC */ u8 matC[0x288 - 0x1CC];
    /* 0x288 */ f32 width;
    /* 0x28C */ f32 height;
    /* 0x290 */ f32 unk290;
    /* 0x294 */ f32 unk294;
    /* 0x298 */ Viewport viewports[2];
    /* 0x2B8 */ u8 pad2B8[0x500 - 0x2B8];
    /* 0x500 */ s32 unk500;
    /* 0x504 */ f32 unk504;
    /* 0x508 */ u8 unk508[4];
    /* 0x50C */ u8 unk50C[4];
    /* 0x510 */ f32 unk510;
    /* 0x514 */ f32 unk514;
    /* 0x518 */ f32 unk518;
    /* 0x51C */ f32 unk51C;
    /* 0x520 */ u8 pad520[4];
    /* 0x524 */ s32 fogNear;
    /* 0x528 */ s32 unk528;
    /* 0x52C */ s32 unk52C;
    /* 0x530 */ u8 unk530;
    /* 0x531 */ u8 pad531[0x53C - 0x531];
    /* 0x53C */ u8 unk53C[0x558 - 0x53C];
    /* 0x558 */ u8 messages[0xE28 - 0x558];
    /* 0xE28 */ u8 objects[0x14];
    /* 0xE3C */ u8 matD[0x40];
    /* 0xE7C */ u8 matE[0x40];
} View;

typedef struct Floats {
    f32 unk0, unk4, unk8, unkC, unk10, unk14;
} Floats;

typedef struct IntPair {
    s32 field_0;
    s32 field_4;
} IntPair;

extern s32 D_800C2030;
extern s32 D_800C2034;
extern IntPair D_800F5A40;

void func_00280C28(void *);
void func_0020F79C(void *);
void func_00281278(View *);
void func_00280E64(View *, s32);
void func_00416614(void *);
void func_0042FFF0(void *);
void CList__Construct(void *list, s32 prevOffset, s32 nextOffset);
void func_00281A98(View *);

void func_0042F9B0(View *view) {
    f32 one;
    f32 zero;
    s16 size;
    s32 i;
    Floats *f1;
    Floats *f2;
    Floats *f3;
    Floats *f4;
    Floats *f5;

    func_00280C28(view->camera);
    func_0020F79C(view->matA);
    func_0020F79C(view->matC);
    zero = 0.0f;
    one = 1.0f;
    f1 = (Floats *)view->orientation;
    f1->unk8 = zero;
    f1->unk4 = zero;
    view->orientation[0] = zero;
    view->unk138 = one;
    func_0020F79C(view->matB);
    view->width = (f32)D_800C2030;
    view->height = (f32)D_800C2034;
    view->unk500 = -1;
    view->unk290 = zero;
    view->unk294 = zero;
    view->unk508[0] = 0;
    view->unk50C[0] = 0;
    view->unk508[1] = 0;
    view->unk50C[1] = 0;
    view->unk508[2] = 0;
    view->unk50C[2] = 0;
    view->unk508[3] = 0;
    view->unk50C[3] = 0;
    view->unk504 = one;
    view->fogNear = 0x3E3;
    view->unk528 = 0x3E3;
    view->unk530 = 0;
    view->scaleX = one;
    view->scaleY = one;
    view->unk510 = 1024.0f;
    view->unk514 = 1024.0f;
    view->unk518 = 47.5f;
    view->unk51C = 47.5f;
    func_00281278(view);
    f2 = (Floats *)&view->a;
    view->unkE8 = zero;
    view->unkEC = zero;
    f2->unk8 = zero;
    f2->unkC = zero;
    f2->unk10 = zero;
    f2->unk14 = one;
    f3 = (Floats *)((u8 *)view + 0xAC);
    f3->unk4 = zero;
    f3->unk8 = zero;
    f3->unkC = zero;
    f3->unk10 = zero;
    f4 = (Floats *)((u8 *)view + 0xC0);
    f4->unk4 = zero;
    f4->unk8 = zero;
    f4->unkC = zero;
    f4->unk10 = zero;
    f5 = (Floats *)((u8 *)view + 0xD4);
    f5->unk4 = zero;
    f5->unk8 = zero;
    f5->unkC = zero;
    f5->unk10 = zero;
    func_00280E64(view, 0);
    size = D_800C2030 * 2;
    for (i = 0; i < 2; i++) {
        view->viewports[i].vscale[0] = size;
        view->viewports[i].vscale[1] = size;
        view->viewports[i].vscale[2] = 0x3FF;
        view->viewports[i].vscale[3] = 0;
        view->viewports[i].vtrans[0] = size;
        view->viewports[i].vtrans[1] = size;
        view->viewports[i].vtrans[2] = 0;
        view->viewports[i].vtrans[3] = 0;
    }
    func_00416614(view->unk53C);
    func_0042FFF0(view->messages);
    func_0020F79C(view->matD);
    func_0020F79C(view->matE);
    view->unk52C = 0;
    D_800F5A40.field_4 = 0;
    CList__Construct(view->objects, 0, 4);
    func_00281A98(view);
}
