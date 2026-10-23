#include "common.h"

typedef struct {
    f32 m[4][4];
} Mtx44f;

void func_00211114(void *src, Mtx44f *dst);
void func_002105A4(Mtx44f *mtx, f32 angle);
void func_0021034C(Mtx44f *mtx, f32 x, f32 y, f32 z);
void func_0020F510(void *pos, f32 value);
void func_00210318(Mtx44f *mtx, f32 x, f32 y, f32 z);
void func_00210C3C(Mtx44f *mtx);
void func_0020D274(Mtx44f *mtx, void *dst);

extern s32 D_8010FF20;
extern s32 D_800B6D1C; /* current buffer index */

typedef struct {
    /* 0x00 */ s32 type;
} Func00215884Def;

typedef struct {
    /* 0x000 */ u8 pad000[4];
    /* 0x004 */ f32 pos[3];
    /* 0x010 */ u8 pad010[4];
    /* 0x014 */ Func00215884Def *def;
    /* 0x018 */ u8 pad018[0x20];
    /* 0x038 */ Mtx44f mtx[2];
    /* 0x0B8 */ u8 pad0B8[0x8C];
    /* 0x144 */ f32 angle;
    /* 0x148 */ u8 pad148[0x1C];
    /* 0x164 */ f32 scale;
    /* 0x168 */ f32 yOffset;
} Func00215884Obj;

/* Builds the object transform (rotation, uniform scale, translation) into its current matrix slot. */
void func_00215884(Func00215884Obj *obj) {
    Mtx44f mtx;

    if (obj->def->type == 8) {
        func_00211114(&D_8010FF20, &mtx);
    } else {
        func_002105A4(&mtx, obj->angle);
    }
    func_0021034C(&mtx, obj->scale, obj->scale, obj->scale);
    func_0020F510(obj->pos, 20000.0f);
    func_00210318(&mtx, obj->pos[0], obj->pos[1] + obj->yOffset, obj->pos[2]);
    func_00210C3C(&mtx);
    func_0020D274(&mtx, &obj->mtx[D_800B6D1C]);
}
