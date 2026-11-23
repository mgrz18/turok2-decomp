/* WRONG CUT: the real function is CCameraPool__ResetEmitterPools, size 0x6C (splat split it at func_0042FF10). */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/ITEMS/8044ACCC_19.CPP, func_8044A6CC_de, with Turok 2's layouts. */
#include "common.h"

#ifdef TARGET_PORT
#include "structs/camera.h"

void CEmitterPool__Construct(char *);

/* Host body: the pool and its cameras through CCameraPool/CCamera (the view below has the camera
 * table and count at their N64 offsets 4/8 and the 0xEC0 camera stride). */
void CCameraPool__ResetEmitterPools(CCameraPool *owner) {
    s32 i;

    for (i = 0; i < owner->m_nCameras; i++) {
        CEmitterPool__Construct((char *)owner->m_Cameras[i].m_SkySystem);
    }
    CEmitterPool__Construct((char *)owner->m_MainCamera.m_SkySystem);
}
#else
typedef struct Cameras {
    /* 0x0 */ s32 handle;
    /* 0x4 */ s32 cameras; /* address of count 0xEC0-byte records */
    /* 0x8 */ s32 count;
} Cameras;

void CEmitterPool__Construct(char *);

/* Updates each camera's message block (at 0x558), then the owner's own at 0x598. */
void CCameraPool__ResetEmitterPools(Cameras *owner) {
    sptr i;

    for (i = 0; i < owner->count; i++) {
        CEmitterPool__Construct((char *)(i * 0xEC0 + owner->cameras + 0x558));
    }
    CEmitterPool__Construct((char *)owner + 0x598);
}
#endif
