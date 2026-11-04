#include "common.h"
#include "structs/camera.h"

/* The region (floor triangle) the camera is in: its three corners at 4, 8, 0xC
 * (see func_00268B50) and the visibility bits the camera takes from it. */
struct CGameRegion {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ CVector3 *m_pCorners[3];
    /* 0x10 */ u8 pad10[0x1C - 0x10];
    /* 0x1C */ u32 unk1C; /* visibility bits, see func_00281E4C */
};

struct CGameRegion *func_00220A1C(s32, CVector3 *);
f32 func_0026BE60(struct CGameRegion *, f32, f32);  /* extern */
s32 Cinema_PathPlaying();                                /* extern */
extern u32 D_800B6D00;
extern s32 D_800F7078;

/* Picks the visibility bits of the camera: those of its region when it is 5 to 11 units
 * above the floor there, else the default D_800B6D00. */
void func_00281E4C(CCamera *arg0) {
    f32 temp_f1;
    f32 temp_f20;
    f32 temp_f21;
    f32 temp_f22;
    f32 temp_f23;
    struct CGameRegion *temp_s1;

    if (Cinema_PathPlaying() != 0) {
        arg0->m_ViewParams.m_pCurrentRegion = func_00220A1C((s32) &D_800F7078, &arg0->m_ViewParams.m_vPos);
    }
    temp_s1 = arg0->m_ViewParams.m_pCurrentRegion;
    temp_f21 = arg0->m_ViewParams.m_vPos.x;
    temp_f23 = arg0->m_ViewParams.m_vPos.y;
    temp_f20 = arg0->m_ViewParams.m_vPos.z;
    temp_f22 = arg0->m_ViewParams.m_YPosOffset;
    arg0->m_ViewParams.m_VisBits = D_800B6D00;
    if ((temp_s1 != 0) && (Cinema_PathPlaying() == 0)) {
        temp_f1 = ((temp_f23 + temp_f22) - func_0026BE60(temp_s1, temp_f21, temp_f20)) * 0.09765625f;
        if ((temp_f1 < 11.0f) && (temp_f1 > 5.0f)) {
            arg0->m_ViewParams.m_VisBits = temp_s1->unk1C;
        }
    }
    if (Cinema_PathPlaying() != 0) {
        arg0->m_ViewParams.m_VisBits = D_800B6D00;
    }
    if (arg0->m_ViewParams.m_ZoomBlend > 0.0f) {
        arg0->m_ViewParams.m_VisBits = D_800B6D00;
    }
}
