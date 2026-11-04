#include "structs/game_object.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0024C1B4.c, func_0024C1B4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct B11Pair {
    s32 a;
    s32 b;
} B11Pair;
typedef struct B11Rec {
    u8 pad0[0x2];
    u16 type;
    u8 pad4[0x4];
    CVector3 pos;
    B11Pair unk14;
} B11Rec;
B11Rec *func_002199C8(s32 a);
s32 func_002199F4(s32 a);
void func_0020F85C(void *m, void *v, CVector3 *out);
void func_00236EA0(CGameObjectInstance *a, CGameObjectInstance *b, u16 c, s32 d, CVector3 e, B11Pair f);
void func_0021F4E8(CGameObjectInstance *a, s32 b) {
    B11Rec *list = func_002199C8(b);
    s32 n = func_002199F4(b);
    s32 i;
    s32 t;
    CVector3 v;
    for (i = 0; i < n; i++) {
        t = list[i].type;
        if (t < 8) {
            if (t >= 6) {
                func_0020F85C(a->m_mfOrient, &list[i].pos, &v);
                func_00236EA0(a, a, list[i].type, -1, v, list[i].unk14);
            }
        }
    }
}
