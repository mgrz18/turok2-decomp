#include "common.h"
#include "structs/particle.h"

/* Particle type 0x56, mode 2: sets flag 0x04000000. */
void func_00232018(CParticle *arg0, s32 arg1) {
    if ((arg0->header.m_nObjType == 0x56) && (arg1 == 2)) {
        arg0->m_dwFlags |= 0x04000000;
    }
}
