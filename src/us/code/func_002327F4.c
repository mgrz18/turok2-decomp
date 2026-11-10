#include "structs/vector.h"
#include "globals/engine.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/CODE_8028414C.CPP, func_80284178_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct Matrix {
    f32 m[4][4];
} Matrix;

/* Turok 2 layout (smaller than Rage Wars' Shared_Particle). */
typedef struct Shared_Particle {
    char pad0[0x4];
    CVector3 pos;      /* 0x04 */
    char pad10[0x30];
    s32 flags;         /* 0x40 */
    char pad44[0x4];
    Matrix mtx[2];     /* 0x48, double-buffered */
} Shared_Particle;

struct Shared_ParticleView {
    char pad0[0x20C];
    Matrix viewMtx;    /* 0x20C */
};

extern void CMtxF__TransformPoint(void *arg0, void *arg1, void *arg2);
extern s32 gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_nCount[]; /* local type: globals/camera.h declares it; this view (its type here) is what keeps the ROM's code */
extern void CParticle__BuildMatrix(void *, sptr, sptr, f32);

#ifdef TARGET_PORT
#include "structs/particle.h"
#include "structs/camera.h"
extern void *gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_pHead;
/* the host CParticle and CCamera by member (the camera list head by name: state[-4] is N64 layout) */
void CParticle__BuildCameraMatrix(void *arg0) {
    CParticle *p = (CParticle *) arg0;
    CCamera *cam;
    CVector3 delta;
    f32 amount;

    if (gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_nCount[0] == 1) {
        cam = (CCamera *) gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_pHead;
        CMtxF__TransformPoint(&cam->m_mfView, &p->header.m_vPos, &delta);
        amount = delta.z;
        if (amount < 0.0f) {
            amount = -amount;
        }
        CParticle__BuildMatrix(arg0, (sptr) p->m_mtxDraw[gEvenOdd], (sptr) cam, amount);
    } else {
        CParticle__BuildMatrix(arg0, (sptr) p->m_mtxDraw[gEvenOdd], 0, 0.0f);
    }
    p->m_dwFlags |= 0x100000;
}
#else
void CParticle__BuildCameraMatrix(void *arg0) {
    CVector3 delta;
    void *arg1;
    f32 amount;
    s32 *state;
    state = gEngineApp_m_Game_m_CameraPool_m_ActiveList_m_nCount;
    if (state[0] == 1) {
        arg1 = (void *)state[-4];
        CMtxF__TransformPoint(&((struct Shared_ParticleView *)arg1)->viewMtx, &((Shared_Particle *)arg0)->pos, &delta);
        amount = delta.z;
        if (amount < 0.0f) {
            amount = -amount;
        }
        CParticle__BuildMatrix(arg0,
                      (sptr)((char *)arg0 + ((gEvenOdd << 6) + 0x48)),
                      (sptr)arg1, amount);
    } else {
        CParticle__BuildMatrix(arg0,
                      (sptr)((char *)arg0 + ((gEvenOdd << 6) + 0x48)),
                      0, 0.0f);
    }
    ((Shared_Particle *)arg0)->flags |= 0x100000;
}
#endif
