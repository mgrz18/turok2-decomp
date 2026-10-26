#include "common.h"

/* Draws a simple object: rebuilds its matrix for this frame's slot once per
 * frame (from a fixed rotation for type 8, else its yaw), then draws its
 * geometry. Type 12 animates by its speed; the others pick a variant, which
 * models 0x6A9..0x6AA override from the game flags (cf. Rage Wars'
 * func_8024F634_de and func_8024F858_de). */

typedef struct {
    f32 x, y, z;
} Vec3;

typedef struct {
    /* 0x00 */ s32 type;
    /* 0x04 */ u8 pad04[0xA];
    /* 0x0E */ s8 variant;
    /* 0x0F */ u8 pad0F[3];
    /* 0x12 */ s8 field12;
} SimpleObjectDef;

typedef struct {
    /* 0x000 */ u8 pad000[2];
    /* 0x002 */ u16 id;
    /* 0x004 */ Vec3 position;
    /* 0x010 */ void *link;
    /* 0x014 */ SimpleObjectDef *def;
    /* 0x018 */ u8 pad018[0x20];
    /* 0x038 */ f32 matrices[4][16];
    /* 0x138 */ u8 pad138[0xC];
    /* 0x144 */ f32 angle;
    /* 0x148 */ u8 pad148[0x1C];
    /* 0x164 */ f32 scale;
    /* 0x168 */ f32 height;
    /* 0x16C */ u8 pad16C[8];
    /* 0x174 */ f32 speed;
    /* 0x178 */ u8 animation[0x1C];
    /* 0x194 */ u8 drawnFrame;
} SimpleObject;

s32 *func_00215944(SimpleObject *);
void func_00211114(void *, f32 *);
void func_002105A4(f32 *, f32);
void func_0021034C(f32 *, f32, f32, f32);
void func_0020F510(Vec3 *, f32);
void func_00210318(f32 *, f32, f32, f32);
void func_00210C3C(f32 *);
void func_0020D274(f32 *, f32 *);
void func_0020B4F0(s32 *, f32);
void func_0020B6A0(s32 *, f32 *, s32, void *, s32, s32, s32);
void func_002051F4(s32, void *);
s32 func_002241C0(void *, s32);
s32 *func_002017D4(s32, s32);

extern u8 D_800B6D1B;
extern s32 D_800B6D1C;
extern u8 D_8010FF20[];
extern u8 D_800F7078[];
extern s32 D_8011AAD8;

void func_002152AC(SimpleObject *obj) {
    s32 *geometry;
    f32 matrix[16];
    s32 variant;
    s32 index;

    geometry = func_00215944(obj);
    if (geometry == NULL) {
        return;
    }
    if (obj->drawnFrame != D_800B6D1B) {
        if (obj->def->type == 8) {
            func_00211114(D_8010FF20, matrix);
        } else {
            func_002105A4(matrix, obj->angle);
        }
        func_0021034C(matrix, obj->scale, obj->scale, obj->scale);
        func_0020F510(&obj->position, 20000.0f);
        func_00210318(matrix, obj->position.x, obj->position.y + obj->height, obj->position.z);
        func_00210C3C(matrix);
        func_0020D274(matrix, obj->matrices[D_800B6D1C]);
    }
    if (obj->def->type == 12) {
        if (obj->drawnFrame != D_800B6D1B) {
            func_0020B4F0(geometry, obj->speed);
        }
        func_0020B6A0(geometry, obj->matrices[D_800B6D1C], 0, obj->animation, 0, D_800B6D1C, obj->def->field12);
    } else {
        variant = obj->def->variant;
        index = func_002241C0(D_800F7078, obj->id);
        if (index < 0x6AB) {
            if (index >= 0x6A9) {
                if ((D_8011AAD8 & 0x800) && *func_002017D4(*geometry, 2) >= 2) {
                    variant = 1;
                }
                if ((D_8011AAD8 & 0x1000) && *func_002017D4(*geometry, 2) >= 3) {
                    variant = 2;
                }
            }
        }
        func_0020B6A0(geometry, obj->matrices[D_800B6D1C], 0, obj->animation, 0, variant, obj->def->field12);
    }
    func_002051F4(0, geometry);
    obj->drawnFrame = D_800B6D1B;
}
