#include "common.h"
#include "structs/vector.h"

/* A blast: radius, damage, then the push (strength, x, y, z). */
typedef struct Blast {
    u16 radius;
    u16 damage;
    s8 strength;
    s8 x;
    s8 y;
    s8 z;
} __attribute__((aligned(4))) Blast;

typedef struct PushOwner {
    /* 0x000 */ u8 pad0[0xBB0];
    /* 0xBB0 */ s32 reduced;
} PushOwner;

typedef struct PushSource {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[0xD4 - 0x10];
    /* 0x0D4 */ u32 flags;
    /* 0x0D8 */ u8 padD8[0x1A8 - 0xD8];
    /* 0x1A8 */ PushOwner *owner;
} PushSource;

typedef struct PushTarget {
    /* 0x000 */ u8 kind;            /* 1: player, 2: object */
    /* 0x001 */ u8 pad1;
    /* 0x002 */ u16 type;
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ u8 pad10[0x8];
    /* 0x018 */ CVector3 dir;
    /* 0x024 */ u8 pad24[0x50 - 0x24];
    /* 0x050 */ f32 angle;
    /* 0x054 */ u8 pad54[0x17C - 0x54];
    /* 0x17C */ u32 playerFlags;
} PushTarget;

extern s32 D_8011001C;

void CQuaternion__FromDirection(CQuaternion *out, CVector3 *dir);
void CVector3__Subtract(CVector3 *out, CVector3 *a, CVector3 *b);
void CVector3__Scale(CVector3 *out, CVector3 *in, f32 scale);
void CMtxF__Identity(CMtxF *m);
void CMtxF__RotateVector(CMtxF *m, CVector3 *in, CVector3 *out);
void CMtxF__SetRotateY(CMtxF *m, f32 angle);
void CQuaternion__ToMatrix(CQuaternion *q, CMtxF *m);

/* The blast's push from `src` turned into a world vector, in the frame of `target`. */
void Blast_GetPush(PushSource *src, PushTarget *target, Blast blast, CVector3 *out, u32 flags) {
    CVector3 v;
    CVector3 dir;
    CQuaternion q;
    CMtxF m;
    PushTarget *obj;

    if (D_8011001C != 0) {
        out->x = 0.0f;
        out->y = 0.0f;
        out->z = 0.0f;
        return;
    }
    v.x = blast.x;
    v.y = blast.y;
    v.z = blast.z;
    CVector3__Scale(&v, &v, blast.strength * 0.80629915f);
    switch (target->kind) {
    case 1:
        if (target->playerFlags & 0x2000) {
            v.x = 0.0f;
            v.z = 0.0f;
            v.y = 133.12f;
        }
        CMtxF__SetRotateY(&m, target->angle + 3.1415927f);
        break;
    case 2:
        obj = target;
        switch (obj->type) {
        case 0xE:
            dir.x = 0.0f;
            dir.z = 0.0f;
            v.x = 0.0f;
            v.y = 0.0f;
            dir.y = 1.0f;
            v.z = 512.0f;
            break;
        case 0x68:
            v.x = 0.0f;
            v.y = 0.0f;
            v.z = 409.59998f;
            dir = obj->dir;
            dir.y = 0.0f;
            break;
        default:
            if (flags & 0x400000) {
                CVector3__Subtract(&dir, &src->pos, &obj->pos);
                dir.y = 7.68f;
            } else {
                dir.x = 0.0f;
                dir.y = 0.0f;
                dir.z = 1.0f;
            }
            break;
        }
        CQuaternion__FromDirection(&q, &dir);
        CQuaternion__ToMatrix(&q, &m);
        break;
    default:
        CMtxF__Identity(&m);
        break;
    }
    if (src->flags & 0x300000) {
        if (src->owner->reduced != 0) {
            CVector3__Scale(&v, &v, 0.1f);
        }
    }
    CMtxF__RotateVector(&m, &v, out);
}
