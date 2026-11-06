/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8027A0F4_2.CPP, func_8027B428_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/particle.h"
#include "structs/collision.h"

extern CCollisionResults D_800F1CE0;  /* the last collision test (m_pInstance: what it hit) */

void CGameRegion__FireTriggers(s32 state, s32 bits, CParticle *p);
void GameObject_FireTriggers(CGameObjectInstance *inst, s32 bits, CParticle *p);

static __inline__ s32 isGroupA(CParticle *p) {
    switch (p->header.m_nObjType) {
        case 2:
        case 0x56:
        case 0x111:
        case 0x126:
            return 1;
    }
    return 0;
}

static __inline__ s32 isGroupB(CParticle *p) {
    switch (p->header.m_nObjType) {
        case 5:
        case 6:
        case 7:
        case 9:
        case 0x11:
        case 0x13:
        case 0x1A:
        case 0x101:
        case 0x110:
            return 1;
    }
    return 0;
}

static __inline__ s32 isGroupC(CParticle *p) {
    switch (p->header.m_nObjType) {
        case 8:
        case 0xD:
        case 0x12:
            return 1;
    }
    return 0;
}

/* Reports a particle to the two collision states by the type group it belongs to: with
 * one set of bits when its owner is a kind 1 object flagged 0x300000, otherwise with
 * another when its descriptor is of kind 1 or 4. */
void CParticle__ReportCollision(CParticle *p) {
    if (D_800F1CE0.m_pWallRegion != 0) {
        CGameObjectInstance *owner = p->m_pOwner;

        if (owner != NULL && owner->header.m_Type == 1 && (owner->m_dwFlags & 0x300000)) {
            if (isGroupA(p)) {
                CGameRegion__FireTriggers((s32)D_800F1CE0.m_pWallRegion, 0x10, p);
            }
            if (isGroupB(p)) {
                CGameRegion__FireTriggers((s32)D_800F1CE0.m_pWallRegion, 8, p);
            }
            if (isGroupC(p)) {
                CGameRegion__FireTriggers((s32)D_800F1CE0.m_pWallRegion, 0x20, p);
            }
        } else if (p->header.m_pIntelligence->m_Type == 1 || p->header.m_pIntelligence->m_Type == 4) {
            if (isGroupC(p)) {
                CGameRegion__FireTriggers((s32)D_800F1CE0.m_pWallRegion, 0x200, p);
            }
        }
    }
    if (D_800F1CE0.m_pInstance != NULL && D_800F1CE0.m_pInstance->header.m_Type == 1) {
        CGameObjectInstance *owner = p->m_pOwner;

        if (owner != NULL && owner->header.m_Type == 1 && (owner->m_dwFlags & 0x300000)) {
            if (isGroupA(p)) {
                GameObject_FireTriggers(D_800F1CE0.m_pInstance, 8, p);
            }
            if (isGroupB(p)) {
                GameObject_FireTriggers(D_800F1CE0.m_pInstance, 4, p);
            }
            if (isGroupC(p)) {
                GameObject_FireTriggers(D_800F1CE0.m_pInstance, 0x10, p);
            }
        } else if (p->header.m_pIntelligence->m_Type == 1 || p->header.m_pIntelligence->m_Type == 4) {
            if (isGroupA(p)) {
                GameObject_FireTriggers(D_800F1CE0.m_pInstance, 0x80, p);
            }
            if (isGroupB(p)) {
                GameObject_FireTriggers(D_800F1CE0.m_pInstance, 0x40, p);
            }
            if (isGroupC(p)) {
                GameObject_FireTriggers(D_800F1CE0.m_pInstance, 0x100, p);
            }
        }
    }
}
