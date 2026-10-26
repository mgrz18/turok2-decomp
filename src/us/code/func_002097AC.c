#include "common.h"

/* A vertex as the morph frames store it. */
typedef struct MorphVtx {
    /* 0x00 */ s16 ob[3];
    /* 0x06 */ u16 flag;
    /* 0x08 */ s16 tc[2];
    /* 0x0C */ s8 n[3];
    /* 0x0F */ u8 a;
} MorphVtx;

/* A morph frame: a vertex count and the vertices. */
typedef struct MorphFrame {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 count;
    /* 0x08 */ MorphVtx verts[1];
} MorphFrame;

extern s32 D_800B6D18;
extern s32 D_800B6D1C;

void *func_002017D4(void *resource, s32 index);

/* Blends the morph frames around time t into the output frame: t wraps into [0, frames - 2), and
 * the two frames either side of it are mixed, positions in 16.16 and normals in 8.8 fixed point. */
void func_002097AC(void *resource, f32 t) {
    MorphFrame *frames[2];
    s32 counts[2];
    MorphVtx *verts[2];
    void *anim;
    MorphFrame *outFrame;
    MorphVtx *out;
    s32 outCount;
    s32 *state;
    s32 numFrames;
    s32 i;
    s32 j;
    f32 frac;
    s32 n1;
    s32 n0;
    s32 p1;
    s32 p0;

    state = func_002017D4(resource, 3);
    state[0] = D_800B6D18;
    anim = func_002017D4(resource, 0);
    numFrames = *(s32 *)anim;
    if (numFrames < 3) {
        return;
    }
    outFrame = func_002017D4(anim, D_800B6D1C);
    out = outFrame->verts;
    outCount = outFrame->count;
    while (t < 0.0f) {
        t += numFrames - 2;
    }
    while (t >= numFrames - 2) {
        t -= numFrames - 2;
    }
    frac = t - (s32)t;
    ((f32 *)state)[1] = t;
    n1 = frac * 256.0f;
    n0 = 0x100 - n1;
    p1 = frac * 65536.0f;
    p0 = 0x10000 - p1;
    for (i = 0; i < 2; i++) {
        frames[i] = func_002017D4(anim, ((s32)t + i) % (numFrames - 2) + 2);
        counts[i] = frames[i]->count;
        verts[i] = frames[i]->verts;
    }
    if (counts[0] != outCount || counts[1] != outCount) {
        return;
    }
    for (j = 0; j < outCount; j++) {
        MorphVtx *a = &verts[0][j];
        MorphVtx *b = &verts[1][j];

        out[j].ob[0] = (a->ob[0] * p0 + b->ob[0] * p1) >> 16;
        out[j].ob[1] = (a->ob[1] * p0 + b->ob[1] * p1) >> 16;
        out[j].ob[2] = (a->ob[2] * p0 + b->ob[2] * p1) >> 16;
        out[j].n[0] = (a->n[0] * n0 + b->n[0] * n1) >> 8;
        out[j].n[1] = (a->n[1] * n0 + b->n[1] * n1) >> 8;
        out[j].n[2] = (a->n[2] * n0 + b->n[2] * n1) >> 8;
    }
}
