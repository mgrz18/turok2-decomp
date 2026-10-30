#include "common.h"

typedef struct Widget {
    /* 0x00 */ u8 pad0[0x28];
    /* 0x28 */ u8 unk28;
    /* 0x29 */ u8 unk29;
    /* 0x2A */ u8 unk2A;
    /* 0x2B */ u8 unk2B;
    /* 0x2C */ u8 unk2C;
    /* 0x2D */ u8 unk2D;
    /* 0x2E */ u8 unk2E;
    /* 0x2F */ u8 unk2F;
    /* 0x30 */ u8 unk30;
    /* 0x31 */ u8 unk31;
    /* 0x32 */ u8 unk32;
} Widget;

void func_0041BBB0(Widget *w);

void func_00454B64(Widget *w, u8 arg1) {
    w->unk29 = 0x80;
    w->unk2A = 0x80;
    w->unk28 = 0;
    w->unk2C = 0;
    w->unk2B = 0;
    w->unk2D = 1;
    w->unk2E = 0;
    w->unk2F = arg1;
    w->unk30 = 0;
    w->unk31 = 1;
    w->unk32 = 0;
    func_0041BBB0(w);
}
