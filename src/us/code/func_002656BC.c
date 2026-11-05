#include "structs/vector.h"
#include "structs/game_object.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00204D2C.c, func_00204D2C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct B01Pair {
    s32 a;
    s32 b;
} B01Pair;
extern s32 D_800F7078[];
extern u8 D_801102B8[];
void func_00237EE4(CGameObjectInstance *a, CGameObjectInstance *b, s32 kind, CVector3 pos, B01Pair extra);
void func_00220260(void *mgr, CGameObjectInstance *actor, s32 flag);
void func_0026E110(CGameObjectInstance *actor, s32 flags, CGameObjectInstance *actor2);
void func_002365F8(void *p, CGameObjectInstance *actor);
void func_002656BC(CGameObjectInstance *actor) {
    B01Pair extra;
    extra.a = 0;
    func_00220260(D_800F7078, actor, 0);
    func_0026E110(actor, 0x8000, actor);
    func_002365F8(D_801102B8, actor);
    if (D_800F7078[0] != 4) {
        func_00237EE4(actor, actor, 7, actor->header.m_vPos, extra);
        actor->m_dwFlags |= 0x8000000;
    }
}
