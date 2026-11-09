#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"
#include "protos/iset.h"
#include "protos/cache.h"

/* A joint of an animated model: its box at 0x18 in model units. */
typedef struct Joint {
    /* 0x00 */ u8 pad0[0x18];
    /* 0x18 */ s16 pos[3];
    /* 0x1E */ u8 pad1E[0x22];
} Joint;

/* A hit volume of a model's node: eight corners, a size and a mask. */
typedef struct HitNode {
    /* 0x00 */ CVector3 corners[8];
    /* 0x60 */ f32 size;
    /* 0x64 */ u8 pad64;
    /* 0x65 */ u8 flags;
    /* 0x66 */ u8 pad66[6];
    /* 0x6C */ s32 mask;
} HitNode;

/* An instance that can be hit. */
typedef struct HitInst {
    /* 0x000 */ u8 type;
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[4];
    /* 0x014 */ CIntelligence *def;
    /* 0x018 */ u8 pad18[0x1C];
    /* 0x034 */ CVector3 scale;
    /* 0x040 */ u8 pad40[0x18];
    /* 0x058 */ f32 mtx[4][4];
    /* 0x098 */ Joint *joints;
    /* 0x09C */ u8 pad9C[8];
    /* 0x0A4 */ s32 animKey;
    /* 0x0A8 */ u8 padA8[8];
    /* 0x0B0 */ s32 animSize;
    /* 0x0B4 */ u8 padB4[8];
    /* 0x0BC */ CBox bounds;
    /* 0x0D4 */ u8 padD4[0x78];
    /* 0x14C */ s32 hitMask;
} HitInst;

void *CGameObjectInstance__LoadGeometry(HitInst *inst);
void CMtxF__FromMtxAffine(f32 (*mtx)[4], Joint *joint);
void CMtxF__ScaleAxes(f32 (*mtx)[4], f32 x, f32 y, f32 z);
void CMtxF__TransformPoints(f32 (*mtx)[4], CVector3 *in, CVector3 *out, s32 count);
s32 Sweep_HitBox(Sweep *sweep, CBox *box, f32 *t);
s32 Sweep_TestBoxFaces(Sweep *sweep, CBox *box, HitInst *other, CVector3 *start, CVector3 *corners, HitNode *node,
                  s32 index);

#define MAX(a, b) ({ f32 _m = (a); if (!((b) <= _m)) _m = (b); _m; })

/* The eight corners of a box, bottom face first. */
static __inline__ void BoxCorners(CVector3 *c, CBox *b) {
    c[0].x = b->m_vMin.x; c[0].y = b->m_vMin.y; c[0].z = b->m_vMin.z;
    c[1].x = b->m_vMax.x; c[1].y = b->m_vMin.y; c[1].z = b->m_vMin.z;
    c[2].x = b->m_vMax.x; c[2].y = b->m_vMin.y; c[2].z = b->m_vMax.z;
    c[3].x = b->m_vMin.x; c[3].y = b->m_vMin.y; c[3].z = b->m_vMax.z;
    c[4].x = b->m_vMin.x; c[4].y = b->m_vMax.y; c[4].z = b->m_vMin.z;
    c[5].x = b->m_vMax.x; c[5].y = b->m_vMax.y; c[5].z = b->m_vMin.z;
    c[6].x = b->m_vMax.x; c[6].y = b->m_vMax.y; c[6].z = b->m_vMax.z;
    c[7].x = b->m_vMin.x; c[7].y = b->m_vMax.y; c[7].z = b->m_vMax.z;
}

/* Sweep against an actor's boxes: each hit volume of its animated model's
 * nodes, or else its animation's box, or else its bounds. */
void Sweep_TestBox(Sweep *sweep, HitInst *other, CVector3 *start, CBox *box) {
    CVector3 out[8];
    CVector3 in[8];
    CVector3 centre;
    f32 mtx[4][4];
    CBox nodeBox;
    f32 t;
    void *nodes;
    s32 enlarge;
    void *model;
    s32 big;
    s32 count;
    s32 i;
    f32 scale;
    f32 maxXY;
    f32 radius;
    HitNode *node;
    s32 k;
    void **anim;
    Joint *joint;
    CBox *animBox;

    enlarge = 0;
    if ((s32)sweep->def->m_dwFlags < 0 && (other->def->m_Type == 1 || other->def->m_Type == 4)) {
        enlarge = 1;
    }
    big = 0;
    if (other->joints != NULL) {
        model = CGameObjectInstance__LoadGeometry(other);
    } else {
        model = NULL;
    }
    if (model != NULL) {
        maxXY = MAX(other->scale.y, other->scale.x);
        scale = other->scale.z;
        if (!(maxXY <= scale)) {
            scale = maxXY;
        }
        nodes = CISet__GetBlock(*(void **) model, 3);
        count = *(s32 *) nodes;
        for (i = 0; i < count; i++) {
            node = CISet__GetBlock(CISet__GetBlock(nodes, i), 1);
            if (!(node->mask & other->hitMask)) {
                continue;
            }
            if (enlarge) {
                k = node->flags & 3;
                big = 0;
                if (k < 4) {
                    big = k >= 0;
                }
            }
            joint = &other->joints[i];
            centre.x = joint->pos[0];
            centre.y = joint->pos[1];
            centre.z = joint->pos[2];
            radius = node->size * scale + 1.0f;
            if (big) {
                radius *= 1.5f;
            }
            nodeBox.m_vMin.x = centre.x - radius;
            nodeBox.m_vMin.y = centre.y - radius;
            nodeBox.m_vMin.z = centre.z - radius;
            nodeBox.m_vMax.x = centre.x + radius;
            nodeBox.m_vMax.y = centre.y + radius;
            nodeBox.m_vMax.z = centre.z + radius;
            if (!(box->m_vMin.x < nodeBox.m_vMax.x) || !(nodeBox.m_vMin.x < box->m_vMax.x) ||
                !(box->m_vMin.z < nodeBox.m_vMax.z) || !(nodeBox.m_vMin.z < box->m_vMax.z) ||
                !(box->m_vMin.y < nodeBox.m_vMax.y) || !(nodeBox.m_vMin.y < box->m_vMax.y)) {
                continue;
            }
            if (!Sweep_HitBox(sweep, &nodeBox, &t)) {
                continue;
            }
            CMtxF__FromMtxAffine(mtx, &other->joints[i]);
            if (big) {
                CMtxF__ScaleAxes(mtx, 1.5f, 1.5f, 1.5f);
            }
            CMtxF__TransformPoints(mtx, node->corners, out, 8);
            Sweep_TestBoxFaces(sweep, box, other, start, out, node, i);
        }
        CCache__Unlock(0, model);
        return;
    }
    anim = CCache__Request(0, other->animKey, other->animSize, 4, 0, 0, "anim object info", 1);
    if (anim != NULL) {
        animBox = CISet__GetBlock(*anim, 0);
        BoxCorners(in, animBox);
        CMtxF__TransformPoints(other->mtx, in, out, 8);
        CCache__Unlock(0, anim);
    } else {
        BoxCorners(out, &other->bounds);
    }
    Sweep_TestBoxFaces(sweep, box, other, start, out, NULL, -1);
}
