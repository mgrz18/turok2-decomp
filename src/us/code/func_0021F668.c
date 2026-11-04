#include "structs/game_object.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0024C33C.c, func_0024C33C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct B11Box B11Box;
void func_00215AE0(f32 *p, s32 a, f32 t, f32 *out);
void func_002105A4(f32 *m, s32 a);
void func_0020F85C(f32 *m, f32 *v, f32 *out);
void func_002688C8(CGameObjectInstance *a, CVector3 v, CVector3 *out, B11Box **box, s32 e, s32 f);
void func_0021F668(CGameObjectInstance *a, f32 *p) {
    f32 t0[3];
    f32 t1[3];
    f32 d[3];
    f32 r[3];
    CVector3 w;
    f32 m[16];
    func_00215AE0(p, 0, 0.0f, t0);
    func_00215AE0(p, 0, *p, t1);
    d[0] = t1[0] - t0[0];
    d[1] = 0.0f;
    d[2] = t1[1] - t0[1];
    func_002105A4(m, *(s32 *)&a->m_RotY);
    func_0020F85C(m, d, r);
    w.x = a->header.m_vPos.x + r[0] * a->m_vScale.x;
    w.y = a->header.m_vPos.y;
    w.z = a->header.m_vPos.z + r[2] * a->m_vScale.z;
    func_002688C8(a, w, &a->header.m_vPos, (B11Box **)&a->header.m_pCurrentRegion, 0, 0);
}
