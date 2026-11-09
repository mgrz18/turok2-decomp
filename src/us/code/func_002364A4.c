#include "structs/vector.h"
#include "globals/particle.h"
#include "protos/list.h"
#include "structs/swoosh.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002A6BD4.c, func_002A6BD4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

void CQuaternion__FromDirection(void *a, CVector3 *b);
void CQuaternion__ToMatrix(void *a, void *b);
void CMtxF__PreRotateX(void *a, f32 f);
void CVector3__Normalize(CVector3 *v);
void CVector3__Scale(CVector3 *a, CVector3 *b, f32 s);
void CVector3__Add(CVector3 *a, CVector3 *b, CVector3 *c);
void CMtxF__Translate(void *m, f32 x, f32 y, f32 z);
void CMtxF__ScaleAxes(void *m, f32 a, f32 b, f32 c);
void CMtxF__ToMtxAffine(void *a, void *b);
/* Places a ground decal of kind idx at pos facing dir (a free one, else the oldest of its kind). */
SwooshDecal *func_002364A4(CSwooshSystem *o, CVector3 pos, CVector3 dir, s32 idx, sptr val) {
    u8 m10[0x40];
    u8 m50[0x10];
    SwooshDecal *n;
    CList *list;
    CQuaternion__FromDirection(m50, &dir);
    if (sSwooshSpawned != 0) {
        return 0;
    }
    {
        sSwooshSpawned = 1;
        n = o->m_FreeDecals.m_pHead;
        list = &o->m_DecalLists[idx];
        if (n != 0) {
            CList__Remove(&o->m_FreeDecals, n);
            goto have;
        }
        n = list->m_pHead;
        if (n != 0) {
            CList__Remove(list, n);
have:
            CList__AddTail(list, n);
            if (n != 0) {
                n->owner = val;
                CQuaternion__ToMatrix(m50, m10);
                CMtxF__PreRotateX(m10, 1.5707965f);
                CVector3__Normalize(&dir);
                CVector3__Scale(&dir, &dir, 2.048f);
                CVector3__Add(&pos, &pos, &dir);
                CMtxF__Translate(m10, pos.x, pos.y, pos.z);
                CMtxF__ScaleAxes(m10, 6.144f, 1.0f, 6.144f);
                CMtxF__ToMtxAffine(m10, &n->mtx);
            }
        }
    }
    return n;
}
