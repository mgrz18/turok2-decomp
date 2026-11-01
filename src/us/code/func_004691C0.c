/* MATCHES at its real start (checked with the symbol moved to 0x4301C0, size 0x90). */
/* WRONG CUT: the real function starts at 0x004691C0 (size 0x90 = 144); the two
 * words at 0x004691B8 (sw v0,0(t3); move a1,zero) are junk between objects. */
#include "common.h"

/* Initialise a unit quad (4 vertices in the XZ plane, 32x32 texels, white). */

typedef struct {
    s16 ob[3];  /* 0x00 */
    u16 flag;   /* 0x06 */
    s16 tc[2];  /* 0x08 */
    u8 cn[4];   /* 0x0C */
} QuadVtx; /* 0x10 bytes */

extern QuadVtx D_8011AD48[4];

void func_004691C0(void) {
    QuadVtx *v;
    s32 i;

    i = 0;
    v = D_8011AD48;
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
