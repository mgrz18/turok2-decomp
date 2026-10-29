/* NOT MATCHING: the ROM's func_00257A3C starts with a stray
 * `addiu $sp, $sp, -0x90` ahead of the real prologue (never undone). The C
 * below reproduces the other 88 words exactly (shifted by that one word). */
#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3;

typedef struct {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ Vec3 viewOffset;
} WeaponInfo;

typedef struct {
    /* 0x000 */ u8 pad0[0x51C];
    /* 0x51C */ u8 *camera;
    /* 0x520 */ u8 pad520[0x996 - 0x520];
    /* 0x996 */ s16 weapon;
} Player;

typedef struct {
    /* 0x000 */ u8 pad0[0x34];
    /* 0x034 */ Vec3 offset;
    /* 0x040 */ u8 pad40[0x58 - 0x40];
    /* 0x058 */ f32 matrix[4][4];
    /* 0x098 */ u8 pad98[0x1A8 - 0x98];
    /* 0x1A8 */ Player *owner;
} WeaponModel;

extern WeaponInfo *D_800AF618[];   /* weapon table */
extern s32 D_800AE37C;             /* flip x */
extern s32 D_800AE380;             /* flip y */
extern s32 D_800AE384;             /* flip z */
extern void func_0020EF60(Vec3 *out, Vec3 *in, f32 scale);
extern void func_0020FC74(f32 *mtx, f32 x, f32 y, f32 z);
extern void func_002101A0(void *src, Vec3 *out);
extern void func_00211114(void *q, f32 *mtx);
extern void func_00210318(f32 *mtx, f32 x, f32 y, f32 z);
extern void func_00210C3C(f32 *mtx);
extern void func_0020C618(f32 *dst, f32 *a, f32 *b);

/* Builds the first-person weapon model's matrix from the owner's camera:
 * its own offset scaled by 0.1, the weapon's view offset (optionally
 * mirrored per axis) scaled by -10.24, the camera rotation and position. */
void func_00257A40(WeaponModel *model) {
    Vec3 cameraPos;
    f32 rotation[4][4];
    Vec3 scaled;
    Vec3 viewOffset;
    f32 mtx[4][4];
    Player *player;

    player = model->owner;
    viewOffset = D_800AF618[player->weapon]->viewOffset;
    if (D_800AE37C) {
        viewOffset.x = -viewOffset.x;
    }
    if (D_800AE380) {
        viewOffset.y = -viewOffset.y;
    }
    if (D_800AE384) {
        viewOffset.z = -viewOffset.z;
    }
    func_0020EF60(&scaled, &model->offset, 0.1f);
    func_0020FC74(mtx[0], scaled.x, scaled.y, scaled.z);
    func_0020EF60(&viewOffset, &viewOffset, -10.24f);
    func_002101A0(player->camera + 0x14C, &cameraPos);
    func_00211114(player->camera + 0x12C, rotation[0]);
    func_00210318(mtx[0], viewOffset.x, viewOffset.y, viewOffset.z);
    func_00210C3C(mtx[0]);
    func_0020C618(model->matrix[0], mtx[0], rotation[0]);
    func_00210318(model->matrix[0], cameraPos.x, cameraPos.y, cameraPos.z);
}
