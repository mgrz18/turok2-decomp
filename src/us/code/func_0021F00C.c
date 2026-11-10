/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8024BA7C_de_2.c, func_8024BC94_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

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
