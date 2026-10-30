/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/particle/80246E34_3.CPP, func_802484B0_de, with Turok 2's layouts.
 * Turok 2 poses the skeleton recursively, one bone per call, keeping its
 * scratch values in file statics (D_800E67B0-D_800E6878). */

#include "common.h"

typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;

typedef struct Quatf {
    f32 x, y, z, w;
} Quatf;

/* A bone's track references: -1 means "use the default frame". */
typedef struct JointRef {
    /* 0x0 */ s16 pos;
    /* 0x2 */ s16 rot;
} JointRef;

typedef struct DefaultFrame {
    /* 0x00 */ Vec3f pos;
    /* 0x0C */ s16 rot[4];
} DefaultFrame;

typedef struct KeyLookup {
    /* 0x00 */ JointRef *joints;
    /* 0x04 */ DefaultFrame *frames;
    /* 0x08 */ void *posTable;
    /* 0x0C */ void *rotTable;
    /* 0x10 */ s32 rotFrame0;
    /* 0x14 */ s32 rotFrame1;
    /* 0x18 */ s32 posFrame0;
    /* 0x1C */ s32 posFrame1;
    /* 0x20 */ f32 blend;
} KeyLookup;

typedef struct TwistPart {
    /* 0x00 */ u8 pad0[0x12];
    /* 0x12 */ s8 count;
    /* 0x13 */ u8 pad13;
    /* 0x14 */ s8 bones[4];
    /* 0x18 */ Vec3f axis;
} TwistPart;

typedef struct Surface {
    /* 0x0 */ u16 pad0;
    /* 0x2 */ u16 flags;
} Surface;

/* A bone's children. */
typedef struct BoneList {
    /* 0x0 */ s32 pad0;
    /* 0x4 */ u32 count;
    /* 0x8 */ s32 items[1];
} BoneList;

typedef struct PoseObject PoseObject;
typedef struct PoseContext PoseContext;
typedef void (*PoseCallback)(f32 (*)[4], PoseContext *);

struct PoseObject {
    /* 0x000 */ u8 pad0[4];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ Surface *surface;
    /* 0x014 */ s32 *def; /* type id first */
    /* 0x018 */ u8 pad18[0x3C];
    /* 0x054 */ f32 height;
    /* 0x058 */ u8 pad58[0x7C];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ u8 padD8[0x2C];
    /* 0x104 */ f32 blend;
    /* 0x108 */ u8 pad108[0x38];
    /* 0x140 */ TwistPart part;
    /* 0x164 */ u8 pad164[0xE4];
    /* 0x248 */ PoseCallback callback;
};

struct PoseContext {
    /* 0x00 */ PoseObject *obj;
    /* 0x04 */ u32 *out;
    /* 0x08 */ f32 (*bones)[4][4];
    /* 0x0C */ void *anims;
    /* 0x10 */ void *file;
    /* 0x14 */ s32 arg2;
    /* 0x18 */ void *channel;
    /* 0x1C */ void *blendChannel;
    /* 0x20 */ KeyLookup keys;
    /* 0x44 */ KeyLookup blendKeys;
    /* 0x68 */ s32 visible;
    /* 0x6C */ s32 blend;
    /* 0x70 */ s32 moving;
    /* 0x74 */ s32 f74;
    /* 0x78 */ s32 f78;
    /* 0x7C */ s32 f7C;
    /* 0x80 */ s32 f80;
    /* 0x84 */ s32 f84;
    /* 0x88 */ s32 f88;
    /* 0x8C */ u8 *boneData;
    /* 0x90 */ s32 bone;
};

/* Scratch shared by every level of the recursion. */
extern BoneList *D_800E67B0;   /* children list */
extern f32 D_800E67C0[4][4];   /* local matrix */
extern Vec3f D_800E6800;       /* translation */
extern Vec3f D_800E6810;       /* position, channel A */
extern Vec3f D_800E6820;       /* position, channel B */
extern Quatf D_800E6830;       /* rotation */
extern Quatf D_800E6840;       /* rotation, channel A */
extern Quatf D_800E6850;       /* rotation, channel B */
extern s32 D_800E6860;         /* object type */
extern u8 *D_800E6864;         /* bone data */
extern u8 D_800E6868;          /* bone flags */
extern Surface *D_800E686C;    /* surface */
extern f32 D_800E6870;         /* ground height */
extern f32 D_800E6874;         /* object height */
extern f32 D_800E6878;         /* depth below the ground */
extern f32 D_800ACB98[];

void *func_002017D4(void *file, s32 index);
void func_0020C618(f32 (*out)[4], f32 (*a)[4], f32 (*b)[4]);
void func_0020DAA4(Quatf *out, f32 amount, Quatf *a, Quatf *b);
void func_0020EF60(Vec3f *out, Vec3f *in, f32 scale);
void func_0020FCC4(f32 (*m)[4], f32 angle, f32 x, f32 y, f32 z);
void func_0020FF80(f32 (*dst)[4], f32 (*src)[4]);
void func_00210318(f32 (*m)[4], f32 x, f32 y, f32 z);
void func_0021034C(f32 (*m)[4], f32 x, f32 y, f32 z);
void func_0021B59C(PoseObject *obj, s32 flags, f32 (*m)[4]);
f32 func_002671B4(PoseObject *obj);
f32 func_00269C3C(Surface *surface, f32 x, f32 z);

static __inline__ void trackPos(KeyLookup *t, s32 i, Vec3f *out) {
    if (t->joints[i].pos == -1) {
        *out = t->frames[i].pos;
    } else {
        f32 *data = func_002017D4(t->posTable, t->joints[i].pos);
        f32 *a = &data[t->posFrame0];
        f32 *b = &data[t->posFrame1];
        f32 blend = t->blend;

        out->x = a[0] + blend * (b[0] - a[0]);
        out->y = a[1] + blend * (b[1] - a[1]);
        out->z = a[2] + blend * (b[2] - a[2]);
    }
}

static __inline__ void defaultRot(KeyLookup *t, s32 i, Quatf *out) {
    DefaultFrame *frame = &t->frames[i];

    out->x = frame->rot[0] * (1.0f / 32767.0f);
    out->y = frame->rot[1] * (1.0f / 32767.0f);
    out->z = frame->rot[2] * (1.0f / 32767.0f);
    out->w = frame->rot[3] * (1.0f / 32767.0f);
}

/* Spherical interpolation between the two keys. */
static __inline__ void slerpRot(KeyLookup *t, s32 i, Quatf *out) {
    if (t->joints[i].rot != -1) {
        f32 *data = func_002017D4(t->rotTable, t->joints[i].rot);

        func_0020DAA4(out, t->blend, (Quatf *) &data[t->rotFrame0], (Quatf *) &data[t->rotFrame1]);
    } else {
        defaultRot(t, i, out);
    }
}

/* Linear interpolation between the two keys. */
static __inline__ void lerpRot(KeyLookup *t, s32 i, Quatf *out) {
    if (t->joints[i].rot == -1) {
        defaultRot(t, i, out);
    } else {
        f32 *data = func_002017D4(t->rotTable, t->joints[i].rot);
        f32 *a = &data[t->rotFrame0];
        f32 *b = &data[t->rotFrame1];
        f32 blend = t->blend;

        out->x = a[0] + blend * (b[0] - a[0]);
        out->y = a[1] + blend * (b[1] - a[1]);
        out->z = a[2] + blend * (b[2] - a[2]);
        out->w = a[3] + blend * (b[3] - a[3]);
    }
}

static __inline__ void lerpVec(Vec3f *out, f32 t, Vec3f *a, Vec3f *b) {
    out->x = a->x + t * (b->x - a->x);
    out->y = a->y + t * (b->y - a->y);
    out->z = a->z + t * (b->z - a->z);
}

static __inline__ void quatToMtx(f32 (*m)[4], Quatf *q, Vec3f *t) {
    f32 xx = q->x * q->x;
    f32 yy = q->y * q->y;
    f32 zz = q->z * q->z;
    f32 ww = q->w * q->w;
    f32 xy = 2.0f * q->x * q->y;
    f32 wz = 2.0f * q->w * q->z;
    f32 xz = 2.0f * q->x * q->z;
    f32 wy = 2.0f * q->w * q->y;
    f32 yz = 2.0f * q->y * q->z;
    f32 wx = 2.0f * q->w * q->x;

    m[0][0] = ww + xx - yy - zz;
    m[0][1] = xy + wz;
    m[0][2] = xz - wy;
    m[0][3] = 0.0f;
    m[1][0] = xy - wz;
    m[1][1] = ww - xx + yy - zz;
    m[1][2] = yz + wx;
    m[1][3] = 0.0f;
    m[2][0] = xz + wy;
    m[2][1] = yz - wx;
    m[2][2] = ww - xx - yy + zz;
    m[2][3] = 0.0f;
    m[3][0] = t->x;
    m[3][1] = t->y;
    m[3][2] = t->z;
    m[3][3] = 1.0f;
}

static __inline__ void twistPart(TwistPart *part, f32 (*cur)[4], s32 n, s32 i) {
    Vec3f turn;
    Vec3f axis;
    f32 rotm[4][4];
    f32 inv[4][4];
    f32 angle;

    if (i != 0) {
        turn.x = cur[3][0];
        turn.y = cur[3][1];
        turn.z = cur[3][2];
        axis = part->axis;
        angle = D_800ACB98[0x19 - part->count];
        angle *= 0.3f;
        angle *= (n == 5 ? -2.0f : (f32) n);
        func_0020FF80(inv, cur);
        func_0020EF60(&turn, &turn, -1.0f);
        func_00210318(inv, turn.x, turn.y, turn.z);
        func_0020FCC4(rotm, angle, axis.x, axis.y, axis.z);
        func_0020C618(cur, inv, rotm);
        func_0020EF60(&turn, &turn, -1.0f);
        func_00210318(cur, turn.x, turn.y, turn.z);
        if (part->bones[0] == i) {
            part->count--;
        }
    }
}

/* guMtxF2L: 16.16 fixed point, integer halves first. */
static __inline__ void packMtx(u32 *out, f32 (*m)[4]) {
    u32 *hi = out;
    u32 *lo = out + 8;
    u32 e1;
    u32 e2;

    e1 = (u32) m[0][0];
    e2 = (u32) m[0][1];
    *hi++ = (e1 & 0xFFFF0000) | (e2 >> 16);
    *lo++ = (e1 << 16) | (e2 & 0xFFFF);
    e1 = (u32) m[0][2];
    e2 = 0;
    *hi++ = (e1 & 0xFFFF0000) | (e2 >> 16);
    *lo++ = (e1 << 16) | (e2 & 0xFFFF);
    e1 = (u32) m[1][0];
    e2 = (u32) m[1][1];
    *hi++ = (e1 & 0xFFFF0000) | (e2 >> 16);
    *lo++ = (e1 << 16) | (e2 & 0xFFFF);
    e1 = (u32) m[1][2];
    e2 = 0;
    *hi++ = (e1 & 0xFFFF0000) | (e2 >> 16);
    *lo++ = (e1 << 16) | (e2 & 0xFFFF);
    e1 = (u32) m[2][0];
    e2 = (u32) m[2][1];
    *hi++ = (e1 & 0xFFFF0000) | (e2 >> 16);
    *lo++ = (e1 << 16) | (e2 & 0xFFFF);
    e1 = (u32) m[2][2];
    e2 = 0;
    *hi++ = (e1 & 0xFFFF0000) | (e2 >> 16);
    *lo++ = (e1 << 16) | (e2 & 0xFFFF);
    e1 = (u32) m[3][0];
    e2 = (u32) m[3][1];
    *hi++ = (e1 & 0xFFFF0000) | (e2 >> 16);
    *lo++ = (e1 << 16) | (e2 & 0xFFFF);
    e1 = (u32) m[3][2];
    e2 = 65536;
    *hi++ = (e1 & 0xFFFF0000) | (e2 >> 16);
    *lo++ = (e1 << 16) | (e2 & 0xFFFF);
}

void func_0021B95C(PoseObject *obj, PoseContext *ctx, s32 bone, f32 (*parent)[4]) {
    void *node;
    u8 *boneData;
    u8 flags;
    f32 (*cur)[4];
    BoneList *children;
    s32 *items;
    u32 count;
    u32 i;
    s32 n;

    node = func_002017D4(ctx->anims, bone);
    boneData = func_002017D4(node, 1);
    D_800E6864 = boneData;
    flags = boneData[0x65];
    ctx->boneData = boneData;
    ctx->bone = bone;
    D_800E6868 = flags;

    trackPos(&ctx->keys, bone, &D_800E6810);
    if (ctx->moving) {
        slerpRot(&ctx->keys, bone, &D_800E6840);
    } else {
        lerpRot(&ctx->keys, bone, &D_800E6840);
    }
    if (ctx->blend) {
        trackPos(&ctx->blendKeys, bone, &D_800E6820);
        if (ctx->moving) {
            slerpRot(&ctx->blendKeys, bone, &D_800E6850);
        } else {
            lerpRot(&ctx->blendKeys, bone, &D_800E6850);
        }
        lerpVec(&D_800E6800, obj->blend, &D_800E6820, &D_800E6810);
        func_0020DAA4(&D_800E6830, obj->blend, &D_800E6850, &D_800E6840);
    } else {
        D_800E6800 = D_800E6810;
        D_800E6830 = D_800E6840;
    }
    if (bone == 0 && ctx->visible) {
        D_800E6800.x = D_800E6800.y = 0.0f;
    }
    quatToMtx(D_800E67C0, &D_800E6830, &D_800E6800);
    if (obj->callback != NULL) {
        obj->callback(D_800E67C0, ctx);
    }
    cur = ctx->bones[bone];
    func_0020C618(cur, D_800E67C0, parent);

    if (bone == 0) {
        switch (D_800E6860 = *obj->def) {
            case 4:
                obj->height = ((f32 *) obj->def)[0xE];
                break;
            case 1:
            case 11:
                D_800E6874 = func_002671B4(obj);
                obj->height = cur[3][1] * (1.0f / 65536.0f) - obj->pos.y - D_800E6874 * 0.5f;
                if (obj->height < 0.0f) {
                    obj->height = 0.0f;
                }
                if ((obj->flags & 0x20000000) && (D_800E686C = obj->surface) != NULL
                    && (D_800E686C->flags & 0x40)) {
                    D_800E6870 = func_00269C3C(D_800E686C, obj->pos.x, obj->pos.z);
                    D_800E6878 = obj->pos.y + obj->height + D_800E6874 - D_800E6870;
                    if (D_800E6878 > 0.0f) {
                        cur[3][1] -= D_800E6878 * 65536.0f;
                        obj->height -= D_800E6878;
                    }
                }
                break;
            default:
                obj->height = 0.0f;
                break;
        }
    }

    if (obj->part.count) {
        n = 0;
        if (obj->part.bones[0] == bone) {
            n = 4;
        }
        if (obj->part.bones[1] == bone) {
            n = 3;
        }
        if (obj->part.bones[2] == bone) {
            n = 2;
        }
        if (obj->part.bones[3] == bone) {
            n = 1;
        }
        if (n != 0 && obj->callback != NULL) {
            twistPart(&obj->part, cur, n, bone);
        }
    }
    if (obj->flags & 0x800000) {
        func_0021B59C(obj, D_800E6868, cur);
    }

    if (ctx->f78 && (D_800E6868 & 0xC7) == 0xC3) {
        ctx->f78 = 0;
        func_0021034C(cur, 2.0f, 2.0f, 2.0f);
    } else if (ctx->f7C && (D_800E6868 & 0xC7) == 0xC7) {
        ctx->f7C = 0;
        func_0021034C(cur, 2.0f, 2.0f, 2.0f);
    } else if (ctx->f80 && (D_800E6868 & 0xC7) == 0xC2) {
        ctx->f80 = 0;
        func_0021034C(cur, 2.0f, 2.0f, 2.0f);
    } else if (ctx->f84 && (D_800E6868 & 0xC7) == 0xC6) {
        ctx->f84 = 0;
        func_0021034C(cur, 2.0f, 2.0f, 2.0f);
    } else if (ctx->f74 && (D_800E6868 & 3) == 1) {
        ctx->f74 = 0;
        func_0021034C(cur, 3.0f, 3.0f, 3.0f);
    }

    if (ctx->f88) {
        func_0020FF80(D_800E67C0, cur);
        func_0021034C(D_800E67C0, 0.5f, 0.5f, 0.5f);
        packMtx(ctx->out + bone * 16, D_800E67C0);
    } else {
        packMtx(ctx->out + bone * 16, cur);
    }

    children = func_002017D4(node, 0);
    count = children->count;
    D_800E67B0 = children;
    items = children->items;
    for (i = 0; i < count; i++) {
        func_0021B95C(obj, ctx, items[i], cur);
    }
}
