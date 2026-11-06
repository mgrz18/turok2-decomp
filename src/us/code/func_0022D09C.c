/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8027A0F4_4.CPP, Particle_Attach, with Turok 2's layouts. */
#include "common.h"
#include "structs/player.h"
#include "structs/particle.h"

/* Moves a particle attached to an actor along with it: onto a bone of its
 * parent's skeleton (or the parent's position) and/or through its owner's
 * transform; returns the resulting position and heading. */

typedef struct {
    /* 0x000 */ u8 pad0[0x14C];
    /* 0x14C */ CMtxF transform;
} PlayerView;

extern void CMtxF__FromMtxAffine(CMtxF out, void *in);              /* unpack a bone matrix */
extern void CMtxF__TransformPoint(CMtxF m, CVector3 *in, CVector3 *out);   /* transform point */
extern void CMtxF__RotateVector(CMtxF m, CVector3 *in, CVector3 *out);   /* transform direction */

void CParticle__Attach(CParticle *actor, CVector3 *outPosition, CVector3 *outHeading) {
    CMtxF matrix;
    CVector3 position;
    CVector3 heading;
    CGameObjectInstance *link;
    u8 *skeleton;

    position = actor->header.m_vPos;
    heading = actor->m_vVelocity;
    if (actor->m_dwFlags & 0x10000) {
        link = actor->m_pTarget;
        switch (actor->header.m_nObjType) {
        case 2:
        case 0xB:
        case 0xF:
        case 0x2D:
        case 0x4F:
        case 0x56:
        case 0x126:
        case 0x12A:
        case 0x132:
            skeleton = (u8 *)link->m_pmtDrawMtxs;
            if (skeleton != NULL) {
                CMtxF__FromMtxAffine(matrix, skeleton + actor->m_nBone * 64);
                CMtxF__TransformPoint(matrix, &actor->m_vAttachPos, &position);
                CMtxF__RotateVector(matrix, &actor->m_vVelocity, &heading);
                actor->header.m_vPos = position;
                actor->m_vDirection = heading;
            } else {
                position = link->header.m_vPos;
                actor->header.m_vPos = position;
            }
            break;
        }
        actor->header.m_pCurrentRegion = link->header.m_pCurrentRegion;
    }
    if (actor->m_dwFlags & 0x400000) {
        link = actor->m_pOwner;
        if (link->m_dwFlags & 0x300000) {
            /* a player: its view's transform */
            CMtxF__TransformPoint(((PlayerView *)((CPlayer *)link->m_AI.m_pParent)->m_pCamera)->transform, &actor->m_vAttachPos, &position);
        } else {
            CMtxF__TransformPoint(link->m_mfOrient, &actor->m_vAttachPos, &position);
        }
        actor->header.m_vPos = position;
    }
    if (outPosition != NULL) {
        *outPosition = position;
    }
    if (outHeading != NULL) {
        *outHeading = heading;
    }
}
