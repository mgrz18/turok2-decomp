#include "common.h"
#include "structs/game_object.h"
#include "structs/collision.h"
#include "globals/collision.h"
#include "structs/particle.h"
#include "structs/player.h"
/* An instance that can be hit, or the one sweeping (a particle: type 2). */
#ifdef TARGET_PORT
/* the host view: the sweeping particle's members (CParticle), its owner's player link
 * (CGameObjectInstance.m_AI.m_pParent) and the player word at N64 0x4F4 (m_WeaponActor's AI
 * variables + 0x20), each at its host offset (one union) */
typedef struct HitInst {
    union {
        struct { u8 _hp_type[__builtin_offsetof(CParticle, header.m_Type)]; u8 type; };
        struct { u8 _hp_kind[__builtin_offsetof(CParticle, header.m_nObjType)]; u16 kind; };
        struct { u8 _hp_pos[__builtin_offsetof(CParticle, header.m_vPos)]; CVector3 pos; };
        struct { u8 _hp_def[__builtin_offsetof(CParticle, header.m_pIntelligence)]; CIntelligence *def; };
        struct { u8 _hp_colour[__builtin_offsetof(CParticle, m_vVelocity)]; f32 colour[3]; };
        struct { u8 _hp_owner[__builtin_offsetof(CParticle, m_pOwner)]; struct HitInst *owner; };
        struct { u8 _hp_target[__builtin_offsetof(CParticle, m_pTarget)]; struct HitInst *target; };
        struct { u8 _hp_player[__builtin_offsetof(CGameObjectInstance, m_AI.m_pParent)];
                 struct { u8 pad[__builtin_offsetof(CPlayer, m_WeaponActor.m_AI.m_VarU8) + 0x20]; s32 hitByLaser; } *player; };
    };
} HitInst;
#else
typedef struct HitInst {
    /* 0x000 */ u8 type;
    /* 0x002 */ u16 kind;
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[4];
    /* 0x014 */ CIntelligence *def;
    /* 0x018 */ f32 colour[3];
    /* 0x024 */ u8 pad24[0xE8];
    /* 0x10C */ struct HitInst *owner;
    /* 0x110 */ u8 pad110[4];
    /* 0x114 */ struct HitInst *target;
    /* 0x118 */ u8 pad118[0x90];
    /* 0x1A8 */ struct { u8 pad[0x4F4]; s32 hitByLaser; } *player;
} HitInst;
#endif

typedef struct EffectParams {
    /* 0x0 */ s16 type;
    /* 0x2 */ s16 scale;
    /* 0x4 */ u8 count;
    /* 0x5 */ s8 r;
    /* 0x6 */ s8 g;
    /* 0x7 */ s8 b;
} __attribute__((aligned(4))) EffectParams;
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
void CVector3__Lerp(CVector3 *out, f32 t, CVector3 *a, CVector3 *b);
s32 Sweep_HitPlane(Sweep *sweep, CVector3 *normal, CVector3 *hit, s32 arg3, s32 arg4, f32 arg5, s32 arg6);
s32 GameObject_IsInanimate(HitInst *inst);
void ImpactEvent_Send(HitInst *a, HitInst *b, s32 type, s32 arg3, CVector3 pos, EffectParams params);
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define Sound__Play Sound__Play_f3
#endif

void Sound__Play(s32 sound, f32 x, f32 y, f32 z, s32 arg4, s32 arg5);
#define DOT(a, b) ((a).x * (b).x + (a).y * (b).y + (a).z * (b).z)

/* Sweep against the six faces of a box given by its eight corners. */
s32 Sweep_TestBoxFaces(Sweep *sweep, sptr unused, HitInst *other, CVector3 *start, CVector3 *corners,
                  HitFrame *frame, s32 hitKind) {
    CVector3 *quad[4];
    CVector3 normal;
    CVector3 rel;
    CVector3 point;
    CVector3 edge;
    CVector3 toPoint;
    CVector3 pos;
    EffectParams params;
    s32 result;
    f32 denom;
    f32 num;
    f32 t;
    s32 face;
    s32 inside;
    s32 j;
    s32 next;
    CVector3 *cur;
    HitInst *inst;
    CCollisionResults *target;

    result = 0;
    for (face = 0; face < 6; face++) {
        switch (face) {
            case 0:
                quad[0] = &corners[7];
                quad[1] = &corners[6];
                quad[2] = &corners[5];
                quad[3] = &corners[4];
                CVector3__Subtract(&normal, &corners[4], &corners[0]);
                break;
            case 1:
                quad[0] = &corners[4];
                quad[1] = &corners[5];
                quad[2] = &corners[1];
                quad[3] = &corners[0];
                CVector3__Subtract(&normal, &corners[0], &corners[3]);
                break;
            case 2:
                quad[0] = &corners[7];
                quad[1] = &corners[4];
                quad[2] = &corners[0];
                quad[3] = &corners[3];
                CVector3__Subtract(&normal, &corners[3], &corners[2]);
                break;
            case 3:
                quad[0] = &corners[6];
                quad[1] = &corners[7];
                quad[2] = &corners[3];
                quad[3] = &corners[2];
                CVector3__Subtract(&normal, &corners[2], &corners[1]);
                break;
            case 4:
                quad[0] = &corners[5];
                quad[1] = &corners[6];
                quad[2] = &corners[2];
                quad[3] = &corners[1];
                CVector3__Subtract(&normal, &corners[1], &corners[0]);
                break;
            case 5:
                quad[0] = &corners[0];
                quad[1] = &corners[1];
                quad[2] = &corners[2];
                quad[3] = &corners[3];
                CVector3__Subtract(&normal, &corners[3], &corners[7]);
                break;
        }
        denom = DOT(normal, sweep->delta);
        if (denom == 0.0f) {
            continue;
        }
        CVector3__Subtract(&rel, start, quad[0]);
        num = DOT(rel, normal);
        if (num == 0.0f) {
            continue;
        }
        t = -num / denom;
        if (!(t < sweep->hit.fraction)) {
            continue;
        }
        point.x = start->x + t * sweep->delta.x;
        point.y = start->y + t * sweep->delta.y;
        point.z = start->z + t * sweep->delta.z;
        inside = 1;
        for (j = 0; j < 4; j++) {
            next = 0;
            if (j != 3) {
                next = j + 1;
            }
            cur = quad[j];
            CVector3__Subtract(&edge, quad[next], cur);
            CVector3__Subtract(&toPoint, &point, cur);
            if (DOT(edge, toPoint) < 0.0f) {
                inside = 0;
                break;
            }
        }
        if (!inside) {
            continue;
        }
        if (!Sweep_HitPlane(sweep, &normal, quad[0], 0,
                           GameObject_IsInanimate(other) ? sweep->def->m_WallBehavior : sweep->def->m_InstanceBehavior,
                           sweep->def->m_GroundFriction, 0)) {
            continue;
        }
        target = gCollisionResultsP;
        sweep->hit.kind = hitKind;
        sweep->hit.posOut = &target->m_vInstancePos;
        sweep->hit.regionOut = &target->m_pInstanceRegion;
        sweep->hit.kindOut = &target->m_nInstanceNode;
        sweep->hit.dataOut = (void **)&target->m_pInstance;
        sweep->hit.data = other;
        if (frame != NULL) {
            target->m_Frame = *frame;
        }
        inst = (HitInst *)sweep->inst;
        result = 1;
        if (inst->type != 2) {
            continue;
        }
        if (inst->kind != 0x68) {
            continue;
        }
        if (inst->target == other) {
            continue;
        }
        inst->target = other;
        CVector3__Lerp(&pos, sweep->hit.fraction, &sweep->inst->m_vPos, &sweep->end);
        params.type = 5;
        params.scale = 0x6400;
        params.count = 5;
        params.r = ((HitInst *)sweep->inst)->colour[0];
        params.g = ((HitInst *)sweep->inst)->colour[1];
        params.b = ((HitInst *)sweep->inst)->colour[2];
        ImpactEvent_Send(other, (HitInst *)sweep->inst, 2, -1, pos, params);
        Sound__Play(0x1B4, pos.x, pos.y, pos.z, 0, -1);
        if (other->def->m_Type == result) {
            ((HitInst *)sweep->inst)->owner->player->hitByLaser = result;
        }
    }
    return result;
}

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
#ifdef TARGET_PORT
typedef struct HitNodeInst {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_HitInst(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_type[T2_HOFF_HitInst(header.m_Type)]; u8 type; }; /* N64 0x0 */
        struct { u8 _hp_pos[T2_HOFF_HitInst(header.m_vPos)]; CVector3 pos; }; /* N64 0x4 */
        struct { u8 _hp_def[T2_HOFF_HitInst(header.m_pIntelligence)]; CIntelligence *def; }; /* N64 0x14 */
        struct { u8 _hp_scale[T2_HOFF_HitInst(m_vScale)]; CVector3 scale; }; /* N64 0x34 */
        struct { u8 _hp_mtx[T2_HOFF_HitInst(m_mfOrient)]; f32 mtx[4][4]; }; /* N64 0x58 */
        struct { u8 _hp_joints[T2_HOFF_HitInst(m_pmtDrawMtxs)]; Joint *joints; }; /* N64 0x98 */
        struct { u8 _hp_animKey[T2_HOFF_HitInst(m_rpObject)]; sptr animKey; }; /* N64 0xA4 */
        struct { u8 _hp_animSize[T2_HOFF_HitInst(m_rpObjectHeader)]; s32 animSize; }; /* N64 0xB0 */
        struct { u8 _hp_bounds[T2_HOFF_HitInst(m_Bounds)]; CBox bounds; }; /* N64 0xBC: m_vMin, m_vMax */
        struct { u8 _hp_hitMask[T2_HOFF_HitInst(m_AI.m_VisBits)]; s32 hitMask; }; /* N64 0x14C */
    };
} HitNodeInst;
#else
typedef struct HitNodeInst {
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
} HitNodeInst;
#endif

void *CGameObjectInstance__LoadGeometry(HitNodeInst *inst);
void CMtxF__FromMtxAffine(f32 (*mtx)[4], Joint *joint);
void CMtxF__ScaleAxes(f32 (*mtx)[4], f32 x, f32 y, f32 z);
void CMtxF__TransformPoints(f32 (*mtx)[4], CVector3 *in, CVector3 *out, s32 count);
s32 Sweep_HitBox(Sweep *sweep, CBox *box, f32 *t);
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
void Sweep_TestBox(Sweep *sweep, HitNodeInst *other, CVector3 *start, CBox *box) {
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
            /* the integer halves of the fixed-point Mtx's translation (round 8 w2): the host keeps
             * the Mtx as host-order u32 words (CMtxF__FromMtxAffine reads them so), so the s16
             * view at 0x18 read y, x, w: every node box was in the wrong place and no shot hit an
             * enemy. Word 6 is x << 16 | y, word 7 z << 16 | w. */
            centre.x = (s16) (((u32 *) joint)[6] >> 16);
            centre.y = (s16) ((u32 *) joint)[6];
            centre.z = (s16) (((u32 *) joint)[7] >> 16);
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

#include "structs/world.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/scene.h"

typedef struct InstanceHit {
    /* 0x00 */ CGameObjectInstance *inst;
    /* 0x04 */ f32 t;
} InstanceHit;
void Sort_Quick(void *base, s32 count, s32 size, s32 (*cmp)(), s32 (*swap)());
void Sweep_GetVolumeBounds(Sweep *sweep, SweepBox *box);
void Sweep_TestInstance(Sweep *sweep, SweepBox *box, CGameObjectInstance *other);
#define MIN(a, b) ({ f32 _m = (a); if (!(_m <= (b))) _m = (b); _m; })
#define MAX(a, b) ({ f32 _m = (a); if (!((b) <= _m)) _m = (b); _m; })
/* the records sorted are {pointer, f32} (ModelHit, InstanceHit): the key after a host pointer */
typedef struct {
    void *unk0;
    f32 key;
} SortItem;
s32 Collision_CompareInstanceHits(SortItem *arg0, SortItem *arg1);
/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8023F43C_de_7.c, func_802405FC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
void Collision_SwapInstanceHits(HitSortRecord *left, HitSortRecord *right);

/* Sweep against the actors whose bounds the swept segment's box touches,
 * nearest first, until the sweep's range runs out. */
void Collision_SweepActors(Sweep *sweep) {
    CBox box;
    SweepBox volume;
    InstanceHit hits[0x80];
    f32 t;
    CVector3 *start;
    CCollisionInfo *def;
    CGameObjectInstance *inst;
    CGameObjectInstance *other;
    CScene *world;
    InstanceHit *hit;
    GameSettings *multi;
    s32 count;
    s32 nHits;
    s32 i;
    s32 j;

    def = sweep->def;
    if (def->m_InstanceBehavior == 0 || !(def->m_dwFlags & 0x40000000)) {
        return;
    }
    inst = (CGameObjectInstance *)sweep->inst;
    start = &inst->header.m_vPos;
    box.m_vMin.x = MIN(sweep->end.x, inst->header.m_vPos.x);
    box.m_vMax.x = MAX(sweep->end.x, inst->header.m_vPos.x);
    box.m_vMin.y = MIN(sweep->end.y, start->y);
    box.m_vMax.y = MAX(sweep->end.y, start->y);
    box.m_vMin.z = MIN(sweep->end.z, start->z);
    box.m_vMax.z = MAX(sweep->end.z, start->z);
    hit = hits;
    nHits = 0;
    world = &gScene;
    count = world->m_nMovers;
    for (i = 0; i < count && nHits < 0x80; i++) {
        other = (CGameObjectInstance *)world->m_pMovers[i];
        if (inst == other) {
            continue;
        }
        if (def->m_InstanceBehavior == 7 && (CGameObjectInstance *)gCollisionResultsP->m_pInstance == other) {
            continue;
        }
        if ((def->m_dwFlags & 0x20000000) && other->m_nTypeFlag == 0x64F) {
            continue;
        }
        if (inst->header.m_Type == 2 && ((CParticle *)inst)->m_pOwner == other) {
            if (((CParticle *)inst)->m_pEffect->m_dwFlags & 0x40) {
                continue;
            }
            if (gEngineApp_m_Settings.multiplayer) {
                continue;
            }
            switch (other->header.m_pIntelligence->m_Type) {
                case 1:
                    continue;
                case 4:
                    continue;
            }
        }
        if (!(box.m_vMax.x > other->m_Bounds.m_vMin.x) || !(box.m_vMin.x < other->m_Bounds.m_vMax.x) ||
            !(box.m_vMax.z > other->m_Bounds.m_vMin.z) || !(box.m_vMin.z < other->m_Bounds.m_vMax.z) ||
            !(box.m_vMax.y > other->m_Bounds.m_vMin.y) || !(box.m_vMin.y < other->m_Bounds.m_vMax.y)) {
            continue;
        }
        if (Sweep_HitBox(sweep, &other->m_Bounds, &t)) {
            hit->inst = other;
            hit->t = t;
            hit++;
            nHits++;
        }
    }
    if (nHits != 0) {
        Sort_Quick(hits, nHits, sizeof(InstanceHit), Collision_CompareInstanceHits, Collision_SwapInstanceHits);
        hit = hits;
        for (j = 0; j < nHits; j++, hit++) {
            if (!(hit->t < sweep->hit.fraction)) {
                break;
            }
            other = hit->inst;
            if ((other->m_dwFlags & 0x300000) && !(multi = &gEngineApp_m_Settings)->multiplayer) {
                Sweep_GetVolumeBounds(sweep, &volume);
                Sweep_TestInstance(sweep, &volume, other);
            } else {
                Sweep_TestBox(sweep, other, start, &box);
            }
        }
    }
}

#include "structs/vector.h"

/* The eight corners of the box: bottom face (y = min) then top face. */
void Collision_BoxCorners(CBox *arg0, CVector3 *arg1) {
    arg1[0].x = (f32) arg0->m_vMin.x;
    arg1[0].y = (f32) arg0->m_vMin.y;
    arg1[0].z = (f32) arg0->m_vMin.z;
    arg1[1].x = (f32) arg0->m_vMax.x;
    arg1[1].y = (f32) arg0->m_vMin.y;
    arg1[1].z = (f32) arg0->m_vMin.z;
    arg1[2].x = (f32) arg0->m_vMax.x;
    arg1[2].y = (f32) arg0->m_vMin.y;
    arg1[2].z = (f32) arg0->m_vMax.z;
    arg1[3].x = (f32) arg0->m_vMin.x;
    arg1[3].y = (f32) arg0->m_vMin.y;
    arg1[3].z = (f32) arg0->m_vMax.z;
    arg1[4].x = (f32) arg0->m_vMin.x;
    arg1[4].y = (f32) arg0->m_vMax.y;
    arg1[4].z = (f32) arg0->m_vMin.z;
    arg1[5].x = (f32) arg0->m_vMax.x;
    arg1[5].y = (f32) arg0->m_vMax.y;
    arg1[5].z = (f32) arg0->m_vMin.z;
    arg1[6].x = (f32) arg0->m_vMax.x;
    arg1[6].y = (f32) arg0->m_vMax.y;
    arg1[6].z = (f32) arg0->m_vMax.z;
    arg1[7].x = (f32) arg0->m_vMin.x;
    arg1[7].y = (f32) arg0->m_vMax.y;
    arg1[7].z = (f32) arg0->m_vMax.z;
}

void Collision_SwapInstanceHits(HitSortRecord *left, HitSortRecord *right) {
    HitSortRecord temporary = *left;
    *left = *right;
    *right = temporary;
}

/* qsort-style comparator on the f32 key at 0x4. */
s32 Collision_CompareInstanceHits(SortItem *arg0, SortItem *arg1) {
    s32 var_v0;

    var_v0 = 1;
    if (arg0->key < arg1->key) {
        var_v0 = -1;
    }
    return var_v0;
}
