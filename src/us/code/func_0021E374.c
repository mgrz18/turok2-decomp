#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Box {
    Vec3 corners[8];            /* 0x00 */
    u8 pad60[0x68 - 0x60];
    f32 unk68;                  /* 0x68 */
    s32 mask;                   /* 0x6C: parts it belongs to */
} Box;                          /* 0x70 bytes */

typedef struct Mtx44 {
    f32 m[4][4];
} Mtx44;                        /* 0x40 bytes */

typedef struct Actor {
    u8 pad0[0x98];
    Mtx44 *matrices;            /* 0x98 */
    Mtx44 *prevMatrices;        /* 0x9C */
    u8 padA0[0xD4 - 0xA0];
    s32 flags;                  /* 0xD4 */
    u8 padD8[0x14C - 0xD8];
    s32 partMask;               /* 0x14C */
} Actor;

extern u8 D_8011AAF1;

void **func_0021F358(Actor *actor);
void *func_002017D4(void *table, s32 index);
void func_0020EEF8(Vec3 *dst, Vec3 *a, Vec3 *b);    /* add */
void func_0020EF60(Vec3 *dst, Vec3 *src, f32 scale);
void func_0020D908(Mtx44 *dst, Mtx44 *src);
void func_0020F85C(Mtx44 *m, Vec3 *in, Vec3 *out);
void func_0020EF2C(Vec3 *dst, Vec3 *a, Vec3 *b);    /* subtract */
void func_002051F4(s32 arg0, void **handle);

/* Copies the actor's bounding box whose center is nearest to pos into out. */
s32 func_0021E374(Actor *actor, Vec3 pos, Box *out) {
    Vec3 center;
    Vec3 d;
    Mtx44 mtx;
    void **handle;
    void *boxes;
    Mtx44 *matrices;
    Box *box;
    Box *best;
    s32 count;
    s32 i;
    s32 j;
    f32 dist;
    f32 bestDist;

    matrices = actor->matrices;
    if (matrices == NULL) {
        matrices = actor->prevMatrices;
        if (matrices == NULL) {
            return 0;
        }
    }
    handle = func_0021F358(actor);
    if (handle == NULL) {
        return 0;
    }
    boxes = func_002017D4(*handle, 3);
    best = NULL;
    count = *(s32 *)boxes;
    bestDist = 3.4028235e38f;
    for (i = 0; i < count; i++) {
        box = func_002017D4(func_002017D4(boxes, i), 1);
        if (!(box->mask & actor->partMask)) {
            continue;
        }
        center = box->corners[0];
        for (j = 1; j < 8; j++) {
            func_0020EEF8(&center, &center, &box->corners[j]);
        }
        func_0020EF60(&center, &center, 0.125f);
        func_0020D908(&mtx, &matrices[i]);
        func_0020F85C(&mtx, &center, &d);
        func_0020EF2C(&d, &d, &pos);
        dist = d.x * d.x + d.y * d.y + d.z * d.z;
        if (dist < bestDist) {
            best = box;
            bestDist = dist;
        }
    }
    if (best != NULL) {
        *out = *best;
        if ((actor->flags & 0x300000) && D_8011AAF1 == 0) {
            out->unk68 = 1.0f;
        }
    }
    func_002051F4(0, handle);
    return best != NULL;
}
