/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/8044ACCC_19.CPP, func_8044A62C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py.
 * Typed with structs/camera.h's CCameraPool (Turok 2's layout is 0x18 smaller than Rage Wars
 * from 0xF00 on); the lists link their elements through the first two pointers. */

#include "common.h"
#include "structs/list.h"
#include "structs/camera.h"
#include "protos/list.h"

extern void * memset(void *arg0, int arg1, u32 arg2);
extern void CCamera__Construct(void *);
extern void CCamera__Advance(void *);
void CCameraPool__Construct(CCameraPool *pool) {
    CCamera *main;
    memset(pool, 0, sizeof(CCameraPool));
    CList__Construct(&pool->m_FreeList, 0, sizeof(void *));
    CList__Construct(&pool->m_ActiveList, 0, sizeof(void *));
    CList__Construct(&pool->m_FreeTextList, 0, sizeof(void *));
    main = &pool->m_MainCamera;
    pool->m_bOnlyUseMainCameraForSound = 1;
    pool->m_pmeCameras = 0;
    pool->m_Cameras = 0;
    pool->m_nCameras = 0;
    pool->m_pmeTexts = 0;
    pool->m_Texts = 0;
    pool->m_nTexts = 0;
    pool->m_bActiveChanged = 2;
    CCamera__Construct(main);
    CCamera__Advance(main);
}
