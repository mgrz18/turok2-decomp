#include "structs/vector.h"
#include "globals/particle.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002A6BD4.c, func_002A6BD4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct Node6BD4 {
    u8 pad0[8];
    u8 sub[0x40];
    s32 unk48;
} Node6BD4;
typedef struct Pool6BD4 {
    u8 pad0[0x94D8];
    u8 lists[0xC8];     /* Turok 2: 0x94D8 (Rage Wars: 0x94E0) */
    Node6BD4 *unk95A0;
} Pool6BD4;
extern f32 D_800A6524;
void CQuaternion__FromDirection(void *a, CVector3 *b);
void CList__Remove(void *list, Node6BD4 *n);
void CList__AddTail(void *list, Node6BD4 *n);
void CQuaternion__ToMatrix(void *a, void *b);
void CMtxF__PreRotateX(void *a, f32 f);
void CVector3__Normalize(CVector3 *v);
void CVector3__Scale(CVector3 *a, CVector3 *b, f32 s);
void CVector3__Add(CVector3 *a, CVector3 *b, CVector3 *c);
void CMtxF__Translate(void *m, f32 x, f32 y, f32 z);
void CMtxF__ScaleAxes(void *m, f32 a, f32 b, f32 c);
void CMtxF__ToMtxAffine(void *a, void *b);
Node6BD4 *func_002364A4(Pool6BD4 *o, CVector3 pos, CVector3 dir, s32 idx, s32 val) {
    u8 m10[0x40];
    u8 m50[0x10];
    Node6BD4 *n;
    u8 *list;
    u8 *l;
    CQuaternion__FromDirection(m50, &dir);
    if (sSwooshSpawned != 0) {
        return 0;
    }
    {
        sSwooshSpawned = 1;
        n = o->unk95A0;
        list = o->lists + idx * 0x14;
        if (n != 0) {
            CList__Remove((u8 *)o + 0x95A0, n);
            goto have;
        }
        n = *(Node6BD4 **)list;
        if (n != 0) {
            CList__Remove(list, n);
have:
            CList__AddTail(list, n);
            if (n != 0) {
                n->unk48 = val;
                CQuaternion__ToMatrix(m50, m10);
                CMtxF__PreRotateX(m10, 1.5707965f);
                CVector3__Normalize(&dir);
                CVector3__Scale(&dir, &dir, 2.048f);
                CVector3__Add(&pos, &pos, &dir);
                CMtxF__Translate(m10, pos.x, pos.y, pos.z);
                CMtxF__ScaleAxes(m10, D_800A6524, 1.0f, D_800A6524);
                CMtxF__ToMtxAffine(m10, n->sub);
            }
        }
    }
    return n;
}
