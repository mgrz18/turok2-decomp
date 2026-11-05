#include "structs/vector.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0022E77C.c, func_0022E77C.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/intelligence.h"
#include "structs/game_object.h"
extern u8 D_8010FF34[];
void *func_00228AEC(void *tbl, s32 id);
void func_0021F00C(CVector3 *out, CGameObjectInstance *actor, CVector3 dir);
void func_0020EEF8(CVector3 *out, CVector3 *a, void *b);
f32 func_002671B4(CGameObjectInstance *actor);
void func_00256ED0(CGameObjectInstance *actor, void **slot, s32 id, f32 z)
{
    void *p;
    CVector3 dir;
    CVector3 out;
    p = *slot;
    if (p == ((void *)0)) {
        p = func_00228AEC(D_8010FF34, id);
        if (p == ((void *)0)) {
            return;
        }
    }
    *slot = p;
    dir.x = 0;
    dir.y = 0;
    dir.z = z;
    func_0021F00C(&out, actor, dir);
    func_0020EEF8(&out, &out, &actor->header.m_vPos);
    (*(s16 *)((u8 *)(p) + (0x16))) = 1;
    (*(s16 *)((u8 *)(p) + (0x10))) = out.x;
    (*(s16 *)((u8 *)(p) + (0x12))) = out.y + func_002671B4(actor) * 0.5f;
    (*(s16 *)((u8 *)(p) + (0x14))) = out.z;
    (*(f32 *)((u8 *)(p) + (0xC))) = 1.0f;
}
