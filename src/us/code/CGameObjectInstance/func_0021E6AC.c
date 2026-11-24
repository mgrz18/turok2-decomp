/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80246E44_de_13.c, func_8024B3A8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#include "structs/world.h"
#include "globals/scene.h"
#include "protos/scene.h"
struct CGameObjectInstance;

extern void CGameObjectInstance__ConstructFromRecord(void *obj, InputRecAt27 *rec, s32 mode, s32 index);
/* The call as the ROM makes it (u16 ids, a u8 flag byte, one argument more than the definition,
 * func_00219BC0.c, takes): the definition under its own name on the N64, cast on the port. */
#ifdef TARGET_PORT
void CGameObjectInstance__Construct(struct CGameObjectInstance *obj, s32 model, s32 type, s32 unused, void *intelligence,
                   s32 sound, void *region, f32 rotY, CVector3 pos, s32 flags, CVector3 scale, CVector3 velocity);
#define CGameObjectInstance__Construct_rec ((void (*)(void *, u16, u16, s32, void *, s32, sptr, f32, CVector3, u8, \
                                                      CVector3, CVector3, s32)) CGameObjectInstance__Construct)
#else
extern void CGameObjectInstance__Construct_rec(void *, u16, u16, s32, void *, s32, sptr, f32,
                          CVector3, u8, CVector3, CVector3, s32) __asm__("CGameObjectInstance__Construct");
#endif
void CGameObjectInstance__ConstructFromRecord(void *arg0, InputRecAt27 *arg1, s32 mode, s32 index) {
    void *resource0;
    sptr resource1;
    s32 lookup;
    s32 amountRaw;
    f32 fzero;
    f32 amount;
    CVector3 zero;
    resource0 = CScene__GetIntelligence(&gScene, arg1->resource22);
    if (arg1->resource20 == 0xFFFF) {
        resource1 = 0;
    } else {
        resource1 = CScene__GetRegion(&gScene, arg1->resource20);
    }
    amountRaw = arg1->amount24;
    fzero = 0.0f;
    amount = amountRaw * 9.587673e-5f;
    zero.x = zero.y = zero.z = fzero;
    lookup = CScene__GetInstanceSavedBit(&gScene, arg0);
    CGameObjectInstance__Construct_rec(arg0, arg1->unk1C, arg1->unk1E, arg1->unk0,
                  resource0, arg1->unk27 == 0xFF ? -1 : arg1->unk27,
                  resource1, amount, *(CVector3 *)arg1->vec4, arg1->unk26,
                  *(CVector3 *)arg1->vec10, zero, lookup);
}
