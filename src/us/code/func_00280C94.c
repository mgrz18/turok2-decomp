/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80233930_de_11.c, func_80238F24_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "structs/world.h"
#include "globals/scene.h"
#include "globals/camera.h"
#include "protos/scene.h"

extern s32 Cinema_PathPlaying(void);
#ifdef TARGET_PORT
#include "structs/camera.h"
#include "structs/region.h"

extern f32 CGameRegion__GetHeight(CGameRegion *tri, f32 x, f32 z);

/* Host body on CCamera.m_ViewParams: the Rage Wars GameplayCamera view sits at N64 offsets (its
 * next pointer and the region slot are 4 bytes there), and its Shared_Quad (an s32/f32 union)
 * is passed in integer registers on x86-64 while the caller passes a CQuaternion in SSE
 * registers: every argument after it arrived wrong. */
void CCamera__SetViewParams(CCamera *cam, s32 type, f32 rotY, f32 rx, f32 ry, f32 rz, f32 x, f32 y,
                            f32 z, f32 yOffset, CQuaternion ground, CGameRegion *region, f32 zoomBlend,
                            f32 zoomFov) {
    CCameraViewParams *v = &cam->m_ViewParams;
    f32 value;

    v->m_nType = type;
    v->m_RotY = rotY;
    v->m_vRotOffset.x = rx;
    v->m_vRotOffset.y = ry;
    v->m_vRotOffset.z = rz;
    v->m_vPos.x = x;
    v->m_vPos.y = y;
    v->m_vPos.z = z;
    v->m_YPosOffset = yOffset;
    v->m_qGround = ground;
    v->m_pCurrentRegion = region;
    v->m_ZoomBlend = zoomBlend;
    v->m_ZoomFov = zoomFov;
    if (Cinema_PathPlaying() != 0) {
        v->m_pCurrentRegion = (CGameRegion *)CScene__FindRegionAt(&gScene, &v->m_vPos.x);
    }
    region = v->m_pCurrentRegion;
    v->m_VisBits = gDefaultVisBits;
    if (region != NULL && Cinema_PathPlaying() == 0) {
        value = ((v->m_vPos.y + v->m_YPosOffset) - CGameRegion__GetHeight(region, v->m_vPos.x, v->m_vPos.z)) * 0.09765625f;
        if (value < 11.0f && 5.0f < value) {
            v->m_VisBits = region->m_VisBits;
        }
    }
    if (Cinema_PathPlaying() != 0) {
        v->m_VisBits = gDefaultVisBits;
    }
    if (v->m_ZoomBlend > 0.0f) {
        v->m_VisBits = gDefaultVisBits;
    }
}
#else
extern f32 CGameRegion__GetHeight(s32 arg0, f32 arg1, f32 arg2);
void CCamera__SetViewParams(GameplayCamera *arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6,
                   f32 arg7, f32 arg8, f32 arg9, Shared_Quad arg10, s32 arg11, f32 arg12,
                   f32 arg13) {
    sptr object;
    f32 value;
    f32 x;
    f32 y;
    f32 z;
    f32 w;
    arg0->unk24 = arg1;
    arg0->unk28 = arg2;
    arg0->unk2C = arg3;
    arg0->unk30 = arg4;
    arg0->unk34 = arg5;
    arg0->unk38 = arg6;
    arg0->unk3C = arg7;
    arg0->unk40 = arg8;
    arg0->unk44 = arg9;
    arg0->unk48 = arg10;
    arg0->unk58 = (void *)arg11;
    arg0->unk5C = arg12;
    arg0->unk60 = arg13;
    if (Cinema_PathPlaying() != 0) {
        arg0->unk58 = (void *)CScene__FindRegionAt(&gScene, &arg0->unk38);
    }
    object = (sptr)arg0->unk58;
    x = arg0->unk38;
    y = arg0->unk3C;
    z = arg0->unk40;
    w = arg0->unk44;
    arg0->unk64 = gDefaultVisBits;
    if (object != 0 && Cinema_PathPlaying() == 0) {
        value = ((y + w) - CGameRegion__GetHeight(object, x, z)) * 0.09765625f;
        if (value < 11.0f && 5.0f < value) {
            arg0->unk64 = ((MenuRules *)(object))->locked;
        }
    }
    if (Cinema_PathPlaying() != 0) {
        arg0->unk64 = gDefaultVisBits;
    }
    if (arg0->unk5C > 0.0f) {
        arg0->unk64 = gDefaultVisBits;
    }
}
#endif
