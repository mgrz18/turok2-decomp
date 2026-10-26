/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/8027A0F4_4.CPP, Particle_Attach, with Turok 2's layouts. */
#include "common.h"
#include "structs/game_object.h"

/* Moves a particle attached to an actor along with it: onto a bone of its
 * parent's skeleton (or the parent's position) and/or through its owner's
 * transform; returns the resulting position and heading. */

typedef f32 Mtx4f[4][4];

typedef struct {
    /* 0x000 */ u8 pad0[0x14C];
    /* 0x14C */ Mtx4f transform;
} PlayerView;

typedef struct {
    /* 0x000 */ u8 pad0[0x51C];
    /* 0x51C */ PlayerView *view;
} Player;

typedef struct {
    /* 0x000 */ u8 pad0[0x2];
    /* 0x002 */ u16 type;
    /* 0x004 */ CVector3 position;
    /* 0x010 */ void *region;
    /* 0x014 */ u8 pad14[0x18 - 0x14];
    /* 0x018 */ CVector3 heading;
    /* 0x024 */ u8 pad24[0x34 - 0x24];
    /* 0x034 */ CVector3 offset;
    /* 0x040 */ s32 flags;
    /* 0x044 */ u8 pad44[0x10C - 0x44];
    /* 0x10C */ CGameObjectInstance *owner;
    /* 0x110 */ u8 pad110[0x114 - 0x110];
    /* 0x114 */ CGameObjectInstance *parent;
    /* 0x118 */ u8 pad118[0x150 - 0x118];
    /* 0x150 */ CVector3 facing;
    /* 0x15C */ u8 pad15C[0x1AD - 0x15C];
    /* 0x1AD */ s8 bone;
} Particle;

extern void func_0020D908(Mtx4f out, void *in);              /* unpack a bone matrix */
extern void func_0020F85C(Mtx4f m, CVector3 *in, CVector3 *out);   /* transform point */
extern void func_0020FAFC(Mtx4f m, CVector3 *in, CVector3 *out);   /* transform direction */

void func_0022D09C(Particle *actor, CVector3 *outPosition, CVector3 *outHeading) {
    Mtx4f matrix;
    CVector3 position;
    CVector3 heading;
    CGameObjectInstance *link;
    u8 *skeleton;

    position = actor->position;
    heading = actor->heading;
    if (actor->flags & 0x10000) {
        link = actor->parent;
        switch (actor->type) {
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
                func_0020D908(matrix, skeleton + actor->bone * 64);
                func_0020F85C(matrix, &actor->offset, &position);
                func_0020FAFC(matrix, &actor->heading, &heading);
                actor->position = position;
                actor->facing = heading;
            } else {
                position = link->header.m_vPos;
                actor->position = position;
            }
            break;
        }
        actor->region = link->header.m_pCurrentRegion;
    }
    if (actor->flags & 0x400000) {
        link = actor->owner;
        if (link->m_dwFlags & 0x300000) {
            /* a player: its view's transform */
            func_0020F85C(((Player *)link->m_AI.m_pParent)->view->transform, &actor->offset, &position);
        } else {
            func_0020F85C(link->m_mfOrient, &actor->offset, &position);
        }
        actor->position = position;
    }
    if (outPosition != NULL) {
        *outPosition = position;
    }
    if (outHeading != NULL) {
        *outHeading = heading;
    }
}
