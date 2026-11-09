/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00247F08.c, func_00247F08, with Turok 2's layouts. */
#include "common.h"
#include "structs/vector.h"
#include "protos/iset.h"

/* Start a 4-step chain on `b` from entry `idx` (following each entry's next
 * link), unless the entry is flagged 3, and give `b` a direction: v1 relative
 * to c's position, made perpendicular to v2. */

typedef struct {
    u8 pad0[0x12];
    s8 b12;             /* 0x12 */
    u8 pad13;
    s8 b14[4];          /* 0x14 */
    CVector3 v18;          /* 0x18 */
    u8 pad24[0x10];
    s8 b34;             /* 0x34 */
} B10Pb;

typedef struct {
    u8 pad0[0x30];
    f32 f30;            /* 0x30 */
    f32 f34;            /* 0x34 */
    f32 f38;            /* 0x38 */
} B10Pc;

typedef struct {
    u8 pad0[0x64];
    s8 b64;             /* 0x64: next entry */
    u8 b65;             /* 0x65 */
} B10Ent2;

s32 **CGameObjectInstance__LoadGeometry(void);
void CCache__Unlock(s32, s32 **);
void CVector3__Subtract(CVector3 *o, CVector3 *a, CVector3 *b);
void CVector3__Cross(CVector3 *o, CVector3 *a, CVector3 *b);
void CVector3__Scale(CVector3 *o, CVector3 *a, f32 s);
void CVector3__Normalize(CVector3 *v);

void func_0021B39C(s32 a, B10Pb *b, B10Pc *c, CVector3 v1, CVector3 v2, s32 idx) {
    CVector3 r;
    CVector3 t;
    CVector3 x;
    s32 **n;
    void *node;
    B10Ent2 *e;
    s32 i;
    s32 cur;

    if (b->b34 != 0x3C && b->b12 < 0x17 && (n = CGameObjectInstance__LoadGeometry()) != 0) {
        node = CISet__GetBlock(*n, 3);
        e = CISet__GetBlock(CISet__GetBlock(node, idx), 1);
        cur = idx;
        if ((e->b65 & 3) == 3) {
            CCache__Unlock(0, n);
            return;
        }
        for (i = 0; i < 4; i++) {
            b->b14[i] = cur;
            cur = e->b64;
            if (cur != -1) {
                e = CISet__GetBlock(CISet__GetBlock(node, cur), 1);
            }
        }
        CCache__Unlock(0, n);
        b->b12 = 0x19;
        t.x = c->f30;
        t.y = c->f34;
        t.z = c->f38;
        CVector3__Subtract(&v1, &v1, &t);
        CVector3__Cross(&r, &v1, &v2);
        CVector3__Normalize(&r);
        CVector3__Normalize(&v1);
        CVector3__Normalize(&v2);
        CVector3__Cross(&x, &r, &v1);
        CVector3__Scale(&r, &r, x.x * v2.x + x.y * v2.y + x.z * v2.z);
        CVector3__Normalize(&r);
        b->v18 = r;
    }
}
