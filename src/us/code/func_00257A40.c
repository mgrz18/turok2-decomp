/* NOT MATCHING: the ROM's func_00257A3C starts with a stray
 * `addiu $sp, $sp, -0x90` ahead of the real prologue (never undone). The C
 * below reproduces the other 88 words exactly (shifted by that one word). */
#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/weapon.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x34];
    /* 0x034 */ CVector3 offset;
    /* 0x040 */ u8 pad40[0x58 - 0x40];
    /* 0x058 */ f32 matrix[4][4];
    /* 0x098 */ u8 pad98[0x1A8 - 0x98];
    /* 0x1A8 */ CPlayer *owner;
} WeaponModel;

extern s32 sWeaponFlipX;             /* flip x */
extern s32 sWeaponFlipY;             /* flip y */
extern s32 sWeaponFlipZ;             /* flip z */
extern void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
extern void CMtxF__SetScale(f32 *mtx, f32 x, f32 y, f32 z);
extern void CMtxF__GetTranslation(void *src, CVector3 *out);
extern void CQuaternion__ToMatrix(void *q, f32 *mtx);
extern void CMtxF__Translate(f32 *mtx, f32 x, f32 y, f32 z);
extern void CMtxF__SwapYZRows(f32 *mtx);
extern void CMtxF__MultiplyAffine(f32 *dst, f32 *a, f32 *b);

/* Builds the first-person weapon model's matrix from the owner's camera:
 * its own offset scaled by 0.1, the weapon's view offset (optionally
 * mirrored per axis) scaled by -10.24, the camera rotation and position. */
void WeaponActor_BuildMatrix(WeaponModel *model) {
    CVector3 cameraPos;
    f32 rotation[4][4];
    CVector3 scaled;
    CVector3 viewOffset;
    f32 mtx[4][4];
    CPlayer *player;

    player = model->owner;
    viewOffset = gWeaponInfoTable[player->m_Data.m_CurrentWeapon]->viewOffset;
    if (sWeaponFlipX) {
        viewOffset.x = -viewOffset.x;
    }
    if (sWeaponFlipY) {
        viewOffset.y = -viewOffset.y;
    }
    if (sWeaponFlipZ) {
        viewOffset.z = -viewOffset.z;
    }
    CVector3__Scale(&scaled, &model->offset, 0.1f);
    CMtxF__SetScale(mtx[0], scaled.x, scaled.y, scaled.z);
    CVector3__Scale(&viewOffset, &viewOffset, -10.24f);
    CMtxF__GetTranslation(((u8 *)player->m_pCamera) + 0x14C, &cameraPos);
    CQuaternion__ToMatrix(((u8 *)player->m_pCamera) + 0x12C, rotation[0]);
    CMtxF__Translate(mtx[0], viewOffset.x, viewOffset.y, viewOffset.z);
    CMtxF__SwapYZRows(mtx[0]);
    CMtxF__MultiplyAffine(model->matrix[0], mtx[0], rotation[0]);
    CMtxF__Translate(model->matrix[0], cameraPos.x, cameraPos.y, cameraPos.z);
}
