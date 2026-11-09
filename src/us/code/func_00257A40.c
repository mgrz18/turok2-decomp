/* NOT MATCHING: the ROM's func_00257A3C starts with a stray
 * `addiu $sp, $sp, -0x90` ahead of the real prologue (never undone). The C
 * below reproduces the other 88 words exactly (shifted by that one word). */
#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/weapon.h"
#include "globals/player.h"

#ifdef TARGET_PORT
#include "structs/game_object.h"
#include "structs/camera.h"
/* The host: the weapon actor itself (scale, orientation, owner in m_AI.m_pParent) and the
 * owner's camera by member; the view below and the camera offsets 0x12C/0x14C are N64 ones. */
typedef CGameObjectInstance WeaponModel;
#define WM_OFFSET(m) (&(m)->m_vScale)
#define WM_MATRIX(m) ((m)->m_mfOrient)
#define WM_OWNER(m) ((CPlayer *)(m)->m_AI.m_pParent)
#define CAMERA_VIEW_ORIENT(cam) ((cam)->m_mfViewOrient)
#define CAMERA_VIEW_ALIGN(cam) (&(cam)->m_qViewAlign)
#else
#define WM_OFFSET(m) (&(m)->offset)
#define WM_MATRIX(m) ((m)->matrix)
#define WM_OWNER(m) ((m)->owner)
#define CAMERA_VIEW_ORIENT(cam) (((u8 *)(cam)) + 0x14C)
#define CAMERA_VIEW_ALIGN(cam) (((u8 *)(cam)) + 0x12C)
typedef struct {
    /* 0x000 */ u8 pad0[0x34];
    /* 0x034 */ CVector3 offset;
    /* 0x040 */ u8 pad40[0x58 - 0x40];
    /* 0x058 */ f32 matrix[4][4];
    /* 0x098 */ u8 pad98[0x1A8 - 0x98];
    /* 0x1A8 */ CPlayer *owner;
} WeaponModel;
#endif

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

    player = WM_OWNER(model);
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
    CVector3__Scale(&scaled, WM_OFFSET(model), 0.1f);
    CMtxF__SetScale(mtx[0], scaled.x, scaled.y, scaled.z);
    CVector3__Scale(&viewOffset, &viewOffset, -10.24f);
    CMtxF__GetTranslation(CAMERA_VIEW_ORIENT(player->m_pCamera), &cameraPos);
    CQuaternion__ToMatrix(CAMERA_VIEW_ALIGN(player->m_pCamera), rotation[0]);
    CMtxF__Translate(mtx[0], viewOffset.x, viewOffset.y, viewOffset.z);
    CMtxF__SwapYZRows(mtx[0]);
    CMtxF__MultiplyAffine(WM_MATRIX(model)[0], mtx[0], rotation[0]);
    CMtxF__Translate(WM_MATRIX(model)[0], cameraPos.x, cameraPos.y, cameraPos.z);
}
