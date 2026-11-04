#include "common.h"

s32 func_0024E5F0(s32, s32, s32);
s32 func_00255714(s32, s32);

typedef struct {
    /* 0x000 */ u8 pad0[0xA10];
    /* 0xA10 */ f32 modeTime;
    /* 0xA14 */ u8 padA14[0xA64 - 0xA14];
    /* 0xA64 */ u32 buttons;
} Player;

typedef struct {
    /* 0x00 */ u8 pad0[0x20];
    /* 0x20 */ f32 speed;
} MoveDef;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ MoveDef *def;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ f32 speed;
} MoveState;

typedef struct {
    /* 0x00 */ f32 unk0;
    /* 0x04 */ f32 scale;
} Tuning;

extern Tuning D_800A7170;
extern s32 D_80110048;

void func_00255670(Player *arg0, MoveState *arg1) {
    f32 var_f2;

    var_f2 = D_800A7170.scale;
    if (D_80110048 == 0x1DB1) {
        var_f2 = 481.28f;
    }
    if (!(arg0->buttons & 0x10) || (arg0->modeTime >= 3.0f)) {
        func_0024E5F0((s32) arg0, (s32) arg1, 5);
    } else {
        arg1->speed = (f32) (var_f2 * arg1->def->speed);
    }
    func_00255714((s32) arg0, (s32) arg1);
}
