#include "common.h"
#include "structs/game_object.h"

typedef struct Box {
    /* 0x00 */ CVector3 min;
    /* 0x0C */ CVector3 max;
} Box;

typedef struct SweepDef {
    /* 0x00 */ s32 flags;
} SweepDef;

typedef struct HitDef {
    /* 0x00 */ s32 kind;
} HitDef;

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
    /* 0x014 */ HitDef *def;
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
    /* 0x0BC */ Box bounds;
    /* 0x0D4 */ u8 padD4[0x78];
    /* 0x14C */ s32 hitMask;
} HitInst;

typedef struct Sweep {
    /* 0x00 */ HitInst *inst;
    /* 0x04 */ SweepDef *def;
} Sweep;

void *func_002017D4(void *table, s32 index);
void *func_0021F358(HitInst *inst);
void func_002051F4(s32 arg0, void *res);
void **func_0020367C(s32 unused, s32 key, s32 size, s32 mode, s32 owner, s32 tag, char *name, s32 direct);
void func_0020D908(f32 (*mtx)[4], Joint *joint);
void func_0021034C(f32 (*mtx)[4], f32 x, f32 y, f32 z);
void func_0020F908(f32 (*mtx)[4], CVector3 *in, CVector3 *out, s32 count);
s32 func_0023A640(Sweep *sweep, Box *box, f32 *t);
s32 func_0023F880(Sweep *sweep, Box *box, HitInst *other, CVector3 *start, CVector3 *corners, HitNode *node,
                  s32 index);

#define MAX(a, b) ({ f32 _m = (a); if (!((b) <= _m)) _m = (b); _m; })

/* The eight corners of a box, bottom face first. */
static __inline__ void BoxCorners(CVector3 *c, Box *b) {
    c[0].x = b->min.x; c[0].y = b->min.y; c[0].z = b->min.z;
    c[1].x = b->max.x; c[1].y = b->min.y; c[1].z = b->min.z;
    c[2].x = b->max.x; c[2].y = b->min.y; c[2].z = b->max.z;
    c[3].x = b->min.x; c[3].y = b->min.y; c[3].z = b->max.z;
    c[4].x = b->min.x; c[4].y = b->max.y; c[4].z = b->min.z;
    c[5].x = b->max.x; c[5].y = b->max.y; c[5].z = b->min.z;
    c[6].x = b->max.x; c[6].y = b->max.y; c[6].z = b->max.z;
    c[7].x = b->min.x; c[7].y = b->max.y; c[7].z = b->max.z;
}

/* Sweep against an actor's boxes: each hit volume of its animated model's
 * nodes, or else its animation's box, or else its bounds. */
void func_0023FDF0(Sweep *sweep, HitInst *other, CVector3 *start, Box *box) {
    CVector3 out[8];
    CVector3 in[8];
    CVector3 centre;
    f32 mtx[4][4];
    Box nodeBox;
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
    Box *animBox;

    enlarge = 0;
    if (sweep->def->flags < 0 && (other->def->kind == 1 || other->def->kind == 4)) {
        enlarge = 1;
    }
    big = 0;
    if (other->joints != NULL) {
        model = func_0021F358(other);
    } else {
        model = NULL;
    }
    if (model != NULL) {
        maxXY = MAX(other->scale.y, other->scale.x);
        scale = other->scale.z;
        if (!(maxXY <= scale)) {
            scale = maxXY;
        }
        nodes = func_002017D4(*(void **) model, 3);
        count = *(s32 *) nodes;
        for (i = 0; i < count; i++) {
            node = func_002017D4(func_002017D4(nodes, i), 1);
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
            nodeBox.min.x = centre.x - radius;
            nodeBox.min.y = centre.y - radius;
            nodeBox.min.z = centre.z - radius;
            nodeBox.max.x = centre.x + radius;
            nodeBox.max.y = centre.y + radius;
            nodeBox.max.z = centre.z + radius;
            if (!(box->min.x < nodeBox.max.x) || !(nodeBox.min.x < box->max.x) ||
                !(box->min.z < nodeBox.max.z) || !(nodeBox.min.z < box->max.z) ||
                !(box->min.y < nodeBox.max.y) || !(nodeBox.min.y < box->max.y)) {
                continue;
            }
            if (!func_0023A640(sweep, &nodeBox, &t)) {
                continue;
            }
            func_0020D908(mtx, &other->joints[i]);
            if (big) {
                func_0021034C(mtx, 1.5f, 1.5f, 1.5f);
            }
            func_0020F908(mtx, node->corners, out, 8);
            func_0023F880(sweep, box, other, start, out, node, i);
        }
        func_002051F4(0, model);
        return;
    }
    anim = func_0020367C(0, other->animKey, other->animSize, 4, 0, 0, "anim object info", 1);
    if (anim != NULL) {
        animBox = func_002017D4(*anim, 0);
        BoxCorners(in, animBox);
        func_0020F908(other->mtx, in, out, 8);
        func_002051F4(0, anim);
    } else {
        BoxCorners(out, &other->bounds);
    }
    func_0023F880(sweep, box, other, start, out, NULL, -1);
}
