#include "common.h"
#include "gfx.h"
#include "structs/vector.h"
/* protos/scene.h's prototypes of what this file calls (the module defines them all; the
 * merged file declares them from the definitions). */
struct CCamera;
struct CGameObjectInstance;
struct CScene;
struct CSceneRecord;
struct LevelEvent;
void CScene__DrawParticles(u8 *, sptr);
#ifdef TARGET_PORT
#include "structs/world.h"
#endif

void Render_ResetBatches(void);
void CParticleSystem__Draw(void *, sptr);
void Render_FlushBatches(void);

void CScene__DrawParticles(u8 *arg0, sptr arg1) {
    Gfx *g;

    /* gSPClipRatio(FRUSTRATIO_1) */
    g = gDList++;
    g->w0 = 0xDB040004;
    g->w1 = 1;
    g = gDList++;
    g->w0 = 0xDB04000C;
    g->w1 = 1;
    g = gDList++;
    g->w0 = 0xDB040014;
    g->w1 = 0xFFFF;
    g = gDList++;
    g->w0 = 0xDB04001C;
    g->w1 = 0xFFFF;
    Render_ResetBatches();
#ifdef TARGET_PORT
    CParticleSystem__Draw(&((CScene *) arg0)->m_ParticleSystem, arg1); /* N64 0x18B0 */
#else
    CParticleSystem__Draw(arg0 + 0x18B0, arg1);
#endif
    Render_FlushBatches();
}
