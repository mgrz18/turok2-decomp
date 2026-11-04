#include "common.h"
#include "structs/particle.h"

s32 func_00228B4C(s32 *, CDLight *);              /* extern */
s32 func_002327F4();                            /* extern */
s32 func_00234E48(s32 *, void *);           /* extern */

extern s32 D_8010FF34;
extern s32 D_801102B8;

void func_00231E94(CParticle *arg0) {
    CDLight *temp_a1;

    if (arg0->m_ActiveSwooshes != 0) {
        func_002327F4();
        func_00234E48(&D_801102B8, arg0);
    }
    temp_a1 = arg0->m_pDLight;
    if (temp_a1 != 0) {
        func_00228B4C(&D_8010FF34, temp_a1);
        arg0->m_pDLight = NULL;
    }
}
