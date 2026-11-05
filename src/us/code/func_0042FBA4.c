/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_9.CPP, func_8044A370_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/camera.h"

extern char D_800A9A00[]; /* "Cameras" */
extern char D_800A9A08[]; /* "Camera Texts" */

void func_0042F9B0(void *);
void CList__Construct(CList *list, s32 prevOffset, s32 nextOffset);
s32 CList__AddTail(CList *list, void *node);
void func_00275AE0(void *);
void func_002053A8(s32);
void func_002052D8(s32, void *);
struct CMemEntry *func_00204EDC(s32, s32, s32, char *);
void *func_00266C5C(void *destination, s32 value, u32 size);

static __inline__ void clearAxis(CShakeAxis *axis) {
    axis->m_Shake = 0;
    axis->m_LastShake = 0;
    axis->m_Amp = 0;
    axis->m_Angle = 0;
}

static __inline__ void initShake(CShake *shake, f32 radius) {
    shake->m_vPos.x = 0;
    shake->m_vPos.y = 0;
    shake->m_vPos.z = 0;
    shake->m_Radius = radius;
    clearAxis(&shake->m_XAxis);
    clearAxis(&shake->m_YAxis);
    clearAxis(&shake->m_ZAxis);
}

void func_0042FBA4(CCameraPool *owner, s32 count) {
    s32 i;
    f32 value;

    owner->m_ActiveLastFrame = 0;
    owner->m_bActiveChanged = 0;
    owner->m_MainCamera.m_Index = 0;
    func_0042F9B0(&owner->m_MainCamera);
    owner->m_bActiveChanged = 2;
    CList__Construct(&owner->m_ShakeFreeList, 0, 4);
    CList__Construct(&owner->m_ShakeActiveList, 0, 4);
    value = 1.0f;
    for (i = 0; i < 8; i++) {
        initShake(&owner->m_ShakeVars[i], value);
        CList__AddTail(&owner->m_ShakeFreeList, &owner->m_ShakeVars[i]);
    }
    CList__Construct(&owner->m_FreeList, 0, 4);
    CList__Construct(&owner->m_ActiveList, 0, 4);
    CList__Construct(&owner->m_FreeTextList, 0, 4);
    owner->m_bOnlyUseMainCameraForSound = 1;
    func_00275AE0(&owner->m_MainCamera);
    func_002053A8(0);
    if (owner->m_pmeCameras) {
        func_002052D8(0, owner->m_pmeCameras);
        owner->m_pmeCameras = NULL;
        owner->m_Cameras = NULL;
        owner->m_nCameras = 0;
    }
    func_002053A8(0);
    if (owner->m_pmeTexts) {
        func_002052D8(0, owner->m_pmeTexts);
        owner->m_pmeTexts = NULL;
        owner->m_Texts = NULL;
        owner->m_nTexts = 0;
    }
    if (count) {
        owner->m_nCameras = count;
        owner->m_pmeCameras = func_00204EDC(0, count * sizeof(CCamera), 0x23, D_800A9A00);
        owner->m_Cameras = *(CCamera **) owner->m_pmeCameras;
        func_00266C5C(owner->m_Cameras, 0, count * sizeof(CCamera));
        for (i = 0; i < owner->m_nCameras; i++) {
            owner->m_Cameras[i].m_Index = i;
            CList__AddTail(&owner->m_FreeList, &owner->m_Cameras[i]);
        }
        func_00275AE0(owner->m_Cameras);
        owner->m_bOnlyUseMainCameraForSound = 0;
    }
    owner->m_nTexts = 16;
    owner->m_pmeTexts = func_00204EDC(0, 0x400, 0x23, D_800A9A08);
    owner->m_Texts = *(CCameraText **) owner->m_pmeTexts;
    func_00266C5C(owner->m_Texts, 0, 0x400);
    for (i = 0; i < owner->m_nTexts; i++) {
        CList__AddTail(&owner->m_FreeTextList, &owner->m_Texts[i]);
    }
}
