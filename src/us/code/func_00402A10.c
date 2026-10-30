#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3_00402A10;

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ void (*onReset)(s32 a0, void *obj);
} Def00402A10;

typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ u8 pad0[0x30 - 0x4];
    /* 0x30 */ Def00402A10 *def;
    /* 0x34 */ s8 kind;
    /* 0x35 */ u8 pad1[0x88 - 0x35];
    /* 0x88 */ s32 unk88;
    /* 0x8C */ u8 pad2[0x9C - 0x8C];
    /* 0x9C */ Vec3_00402A10 pos;
    /* 0xA8 */ s32 angle;
    /* 0xAC */ Vec3_00402A10 prevPos;
    /* 0xB8 */ s32 prevAngle;
} Obj00402A10;

extern s32 D_8012F5F0[];
extern u8 D_8012F3A0[];
void func_0025AD90(s32 a0, Obj00402A10 *obj, s32 a2);
void func_004021C8(void *state);

void func_00402A10(s32 a0, Obj00402A10 *obj) {
    obj->prevPos = obj->pos;
    obj->prevAngle = obj->angle;
    if (D_8012F5F0[0] != 0 || obj->kind == 15) {
        obj->unk88 = 0;
        obj->flags |= 0x20000000;
    } else {
        obj->flags &= ~0x20000000;
    }
    func_0025AD90(a0, obj, 0);
    if (obj->def != NULL && obj->def->onReset != NULL) {
        obj->def->onReset(a0, obj);
    }
    func_004021C8(D_8012F3A0);
}
