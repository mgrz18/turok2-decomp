/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8024BA7C_de_2.c, func_8024BC94_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

#include "structs/vector.h"
struct CGameObjectInstance;

/* Host body (docs/port/ABI.md, abicheck struct_class/struct_scalar): every caller passes
 * (CVector3 *out, actor, CVector3 v). The MIPS view took v as an IntPair and left the actor in
 * a1 for CGameObjectInstance__GetRotation, which returns its CQuaternion through a hidden
 * pointer there and in registers on the host. */
extern CQuaternion CGameObjectInstance__GetRotation(struct CGameObjectInstance *actor);
void CQuaternion__ToMatrix(void *q, void *m);
void CMtxF__TransformPoint(void *m, void *in, void *out);
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
