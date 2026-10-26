/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_00247F08.c, func_00247F08, with Turok 2's layouts. */
#include "common.h"

/* Start a 4-step chain on `b` from entry `idx` (following each entry's next
 * link), unless the entry is flagged 3, and give `b` a direction: v1 relative
 * to c's position, made perpendicular to v2. */

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    u8 pad0[0x12];
    s8 b12;             /* 0x12 */
    u8 pad13;
    s8 b14[4];          /* 0x14 */
    Vec3f v18;          /* 0x18 */
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

s32 **func_0021F358(void);
void func_002051F4(s32, s32 **);
void *func_002017D4(void *, s32);
void func_0020EF2C(Vec3f *o, Vec3f *a, Vec3f *b);
void func_0020EFDC(Vec3f *o, Vec3f *a, Vec3f *b);
void func_0020EF60(Vec3f *o, Vec3f *a, f32 s);
void func_0020F040(Vec3f *v);

void func_0021B39C(s32 a, B10Pb *b, B10Pc *c, Vec3f v1, Vec3f v2, s32 idx) {
    Vec3f r;
    Vec3f t;
    Vec3f x;
    s32 **n;
    void *node;
    B10Ent2 *e;
    s32 i;
    s32 cur;

    if (b->b34 != 0x3C && b->b12 < 0x17 && (n = func_0021F358()) != 0) {
        node = func_002017D4(*n, 3);
        e = func_002017D4(func_002017D4(node, idx), 1);
        cur = idx;
        if ((e->b65 & 3) == 3) {
            func_002051F4(0, n);
            return;
        }
        for (i = 0; i < 4; i++) {
            b->b14[i] = cur;
            cur = e->b64;
            if (cur != -1) {
                e = func_002017D4(func_002017D4(node, cur), 1);
            }
        }
        func_002051F4(0, n);
        b->b12 = 0x19;
        t.x = c->f30;
        t.y = c->f34;
        t.z = c->f38;
        func_0020EF2C(&v1, &v1, &t);
        func_0020EFDC(&r, &v1, &v2);
        func_0020F040(&r);
        func_0020F040(&v1);
        func_0020F040(&v2);
        func_0020EFDC(&x, &r, &v1);
        func_0020EF60(&r, &r, x.x * v2.x + x.y * v2.y + x.z * v2.z);
        func_0020F040(&r);
        b->v18 = r;
    }
}
