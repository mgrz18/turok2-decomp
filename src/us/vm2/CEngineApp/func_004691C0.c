/* MATCHES at its real start (checked with the symbol moved to 0x4301C0, size 0x90). */
/* WRONG CUT: the real function starts at 0x004691C0 (size 0x90 = 144); the two
 * words at 0x004691B8 (sw v0,0(t3); move a1,zero) are junk between objects. */
#include "common.h"
#include "structs/gbi.h"

/* Initialise a unit quad (4 vertices in the XZ plane, 32x32 texels, white). */


extern Vtx_t gUnitQuadVtx[4]; /* local: no header declares it (globals/render.h has no Vtx_t) */

void Render_InitUnitQuad_2(void) {
    Vtx_t *v;
    s32 i;

    i = 0;
    v = gUnitQuadVtx;
    v[0].ob[0] = -1;
    v[0].ob[2] = 1;
    v[0].tc[0] = 0;
    v[0].tc[1] = 0;
    v[1].ob[0] = 1;
    v[1].ob[2] = 1;
    v[1].tc[0] = 0x400;
    v[1].tc[1] = 0;
    v[2].ob[0] = 1;
    v[2].ob[2] = -1;
    v[2].tc[0] = 0x400;
    v[2].tc[1] = 0x400;
    v[3].ob[0] = -1;
    v[3].ob[2] = -1;
    v[3].tc[0] = 0;
    v[3].tc[1] = 0x400;
    for (; i < 4; i++) {
        v[i].ob[1] = 0;
        v[i].flag = 0;
        v[i].cn[0] = 0xFF;
        v[i].cn[1] = 0xFF;
        v[i].cn[2] = 0xFF;
        v[i].cn[3] = 0xFF;
    }
}
