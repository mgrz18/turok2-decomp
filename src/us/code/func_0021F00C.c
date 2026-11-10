/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8024BA7C_de_2.c, func_8024BC94_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

#ifdef TARGET_PORT
#include "structs/vector.h"

/* Host body (docs/port/ABI.md, abicheck struct_class/struct_scalar): every caller passes
 * (CVector3 *out, actor, CVector3 v). The MIPS view took v as an IntPair and left the actor in
 * a1 for CGameObjectInstance__GetRotation, which returns its CQuaternion through a hidden
 * pointer there and in registers on the host. */
extern CQuaternion CGameObjectInstance__GetRotation(void *actor);
extern void CQuaternion__ToMatrix(f32 *q, f32 *m);
extern void CMtxF__TransformPoint(void *mf, CVector3 *in, CVector3 *out);
CVector3 *CGameObjectInstance__LocalToWorld(CVector3 *out, void *actor, CVector3 v) {
    CQuaternion q;
    f32 m[16];
    CVector3 r;

    q = CGameObjectInstance__GetRotation(actor);
    CQuaternion__ToMatrix(&q.x, m);
    CMtxF__TransformPoint(m, &v, &r);
    *out = r;
    return out;
}
#else
extern void CGameObjectInstance__GetRotation(void *arg0);
extern void CQuaternion__ToMatrix(void *arg0, void *arg1);
extern void CMtxF__TransformPoint(void *, void *, void *);
void *CGameObjectInstance__LocalToWorld(void *arg0, sptr unused1, struct IntPair arg2) {
    s32 sp10[4];
    s32 sp20[16];
    Triple sp60;
    CGameObjectInstance__GetRotation(sp10);
    CQuaternion__ToMatrix(sp10, sp20);
    CMtxF__TransformPoint(sp20, &arg2, &sp60);
    *(Triple *)arg0 = sp60;
    return arg0;
}
#endif
