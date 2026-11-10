#include "common.h"
#include "gfx.h"
#include "protos/scene.h"

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
    CParticleSystem__Draw(arg0 + 0x18B0, arg1);
    Render_FlushBatches();
}
