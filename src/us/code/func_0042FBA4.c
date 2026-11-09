/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_9.CPP, func_8044A370_de, with Turok 2's layouts. */
#include "common.h"
#include "structs/camera.h"
#include "protos/list.h"
#ifdef TARGET_PORT /* the list links and the text block at their host sizes */
#define LINKS(T) T2_OFFSETOF(T, m_pLast), T2_OFFSETOF(T, m_pNext)
#define TEXTS_SIZE (16 * sizeof(CCameraText))
#else
#define LINKS(T) 0, 4
#define TEXTS_SIZE 0x400
#endif

void CCamera__Construct(void *);
void Sound__SetListener(void *);
void CCache__Flush(s32);
void CCache__Deallocate(s32, void *);
struct CMemEntry *CCache__Allocate(s32, s32, s32, char *);
void *memset(void *destination, s32 value, uptr size);

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

void CCameraPool__NewGame(CCameraPool *owner, s32 count) {
    s32 i;
    f32 value;

    owner->m_ActiveLastFrame = 0;
    owner->m_bActiveChanged = 0;
    owner->m_MainCamera.m_Index = 0;
    CCamera__Construct(&owner->m_MainCamera);
    owner->m_bActiveChanged = 2;
    CList__Construct(&owner->m_ShakeFreeList, LINKS(CShake));
    CList__Construct(&owner->m_ShakeActiveList, LINKS(CShake));
    value = 1.0f;
    for (i = 0; i < 8; i++) {
        initShake(&owner->m_ShakeVars[i], value);
        CList__AddTail(&owner->m_ShakeFreeList, &owner->m_ShakeVars[i]);
    }
    CList__Construct(&owner->m_FreeList, LINKS(CCamera));
    CList__Construct(&owner->m_ActiveList, LINKS(CCamera));
    CList__Construct(&owner->m_FreeTextList, LINKS(CCameraText));
    owner->m_bOnlyUseMainCameraForSound = 1;
    Sound__SetListener(&owner->m_MainCamera);
    CCache__Flush(0);
    if (owner->m_pmeCameras) {
        CCache__Deallocate(0, owner->m_pmeCameras);
        owner->m_pmeCameras = NULL;
        owner->m_Cameras = NULL;
        owner->m_nCameras = 0;
    }
    CCache__Flush(0);
    if (owner->m_pmeTexts) {
        CCache__Deallocate(0, owner->m_pmeTexts);
        owner->m_pmeTexts = NULL;
        owner->m_Texts = NULL;
        owner->m_nTexts = 0;
    }
    if (count) {
        owner->m_nCameras = count;
        owner->m_pmeCameras = CCache__Allocate(0, count * sizeof(CCamera), 0x23, "Cameras");
        owner->m_Cameras = *(CCamera **) owner->m_pmeCameras;
        memset(owner->m_Cameras, 0, count * sizeof(CCamera));
        for (i = 0; i < owner->m_nCameras; i++) {
            owner->m_Cameras[i].m_Index = i;
            CList__AddTail(&owner->m_FreeList, &owner->m_Cameras[i]);
        }
        Sound__SetListener(owner->m_Cameras);
        owner->m_bOnlyUseMainCameraForSound = 0;
    }
    owner->m_nTexts = 16;
    owner->m_pmeTexts = CCache__Allocate(0, TEXTS_SIZE, 0x23, "Camera Texts");
    owner->m_Texts = *(CCameraText **) owner->m_pmeTexts;
    memset(owner->m_Texts, 0, TEXTS_SIZE);
    for (i = 0; i < owner->m_nTexts; i++) {
        CList__AddTail(&owner->m_FreeTextList, &owner->m_Texts[i]);
    }
}
