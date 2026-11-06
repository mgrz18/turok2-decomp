#include "structs/vector.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00284E70.c, func_00284E70.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct B19Obj B19Obj;
struct B19Obj {
    u8 pad0[0x2];
    u16 id;          /* 0x2 */
    CVector3 pos;     /* 0x4 */
    u8 pad10[0x118];
    s16 unk128;      /* 0x128 */
};
extern const f32 D_800A6430;
void CVector3__Subtract(CVector3 *, CVector3 *, CVector3 *);
void CParticle__CheckProximity(void *, B19Obj *, B19Obj **, s32);
void CParticleSystem__ResolveInteractions(void *owner, B19Obj **arr, s32 n) {
    s32 i, j;
    CVector3 d;
    B19Obj *o;
    B19Obj *e;
    for (i = 0; i < n; i++) {
        o = arr[i];
        switch (o->id) {
        case 0xC:
            break;
        case 0x22:
            CParticle__CheckProximity(owner, o, arr, n);
            break;
        case 0x57:
            for (j = 0; j < n; j++) {
                e = arr[j];
                if (e->id != 0x57) {
                    CVector3__Subtract(&d, &e->pos, &o->pos);
                    if (d.x * d.x + d.y * d.y + d.z * d.z <= D_800A6430) e->unk128 = 1;
                }
            }
            break;
        }
    }
}
