/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80246E44_de_13.c, func_8024B3A8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "structs/world.h"

extern s32 CScene__GetRegion(void *arg0, s32 arg1);
extern void CGameObjectInstance__ConstructFromRecord(void *obj, InputRecAt27 *rec, s32 mode, s32 index);
extern f32 D_800A5CEC[];
extern CWorld D_800F7078;
extern f32 D_800A5CEC[];
extern void *CScene__GetIntelligence(void *, s32);
extern s32 CScene__GetInstanceSavedBit(void *, void *);
extern void CGameObjectInstance__Construct(void *, u16, u16, s32, void *, s32, s32, f32,
                          CVector3, u8, CVector3, CVector3, s32);
void CGameObjectInstance__ConstructFromRecord(void *arg0, InputRecAt27 *arg1, s32 mode, s32 index) {
    void *resource0;
    s32 resource1;
    s32 lookup;
    s32 amountRaw;
    f32 fzero;
    f32 amount;
    CVector3 zero;
    resource0 = CScene__GetIntelligence(&D_800F7078, arg1->resource22);
    if (arg1->resource20 == 0xFFFF) {
        resource1 = 0;
    } else {
        resource1 = CScene__GetRegion(&D_800F7078, arg1->resource20);
    }
    amountRaw = arg1->amount24;
    fzero = 0.0f;
    amount = amountRaw * D_800A5CEC[1];
    zero.x = zero.y = zero.z = fzero;
    lookup = CScene__GetInstanceSavedBit(&D_800F7078, arg0);
    CGameObjectInstance__Construct(arg0, arg1->unk1C, arg1->unk1E, arg1->unk0,
                  resource0, arg1->unk27 == 0xFF ? -1 : arg1->unk27,
                  resource1, amount, *(CVector3 *)arg1->vec4, arg1->unk26,
                  *(CVector3 *)arg1->vec10, zero, lookup);
}
