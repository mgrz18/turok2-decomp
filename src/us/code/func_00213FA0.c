#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x0 */ Vec3f mn;
    /* 0xC */ Vec3f mx;
} Box3;

typedef struct {
    /* 0x00 */ u8 pad0[0xA0];
    /* 0xA0 */ Box3 box;
    /* 0xB8 */ u8 padB8[2];
    /* 0xBA */ u8 bBA;
    /* 0xBB */ u8 padBB[5];
} WRec; /* size 0xC0 */

typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ s32 count;
    /* 0x8 */ WRec r[1];
} WTab;

void *func_002017D4(s32, s32);
s32 func_002132B8(Box3 *q, WRec **out, s32 *cnt);

/* Collects up to 64 records of `model` (or of the world grid when there is
 * none) whose boxes overlap `q`; returns whether any of them has bBA set. */
s32 func_00213FA0(Box3 *q, s32 model, WRec **out, s32 *cnt) {
    WTab *tab;
    WRec *rec;
    s32 n;
    s32 ret;
    s32 j;

    n = 0;
    ret = 0;
    if (model == 0) {
        return func_002132B8(q, out, cnt);
    }
    tab = func_002017D4(model, 2);
    j = tab->count;
    j--;
    rec = tab->r;
    for (; j != -1; j--) {
        if (q->mx.x > rec->box.mn.x && q->mn.x < rec->box.mx.x && q->mx.z > rec->box.mn.z &&
            q->mn.z < rec->box.mx.z && q->mx.y > rec->box.mn.y && q->mn.y < rec->box.mx.y) {
            out[n] = rec;
            n++;
            if (rec->bBA != 0) {
                ret = 1;
            }
            if (n == 64) {
                break;
            }
        }
        rec++;
    }
    *cnt = n;
    return ret;
}
