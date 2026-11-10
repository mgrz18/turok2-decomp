/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_3/80268160_3.CPP, func_802688AC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/game_object.h"
struct IntPair;
struct IntPair {
    int field_0;
    int field_4;
};
struct IntPair;
struct IntPair;
struct IntPair;
struct IntPair;
struct IntPair;
extern void AI_StopSound(u8 *, u8 *);
#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md section 5, abicheck indirect struct_class/struct_scalar): the dispatcher
 * (ImpactEvent_Send/Call) passes (actor, actor, kind, CVector3 pos, 8-byte event value). On MIPS
 * the position and the value fill a3 and the stack words this definition names pair; on the host
 * the vector travels in SSE/FP registers and the value in one general register, so the host
 * definition takes them as they are passed: the value's first word is what MIPS read. */
void func_00239398(void *unused, u8 *object, s32 third, CVector3 pos, s32 value) {
    if (object != 0 && object[0] == 1) {
        AI_StopSound(object, object + T2_OFFSETOF(CGameObjectInstance, m_AI));
    }
}
#else
void func_00239398(void *unused, u8 *object, s32 third, struct IntPair pair) {
    if (object != 0 && object[0] == 1) {
        AI_StopSound(object, object + T2_OFFSETOF(CGameObjectInstance, m_AI));
    }
}
extern void AI_StopSound(u8 *, u8 *);
#endif
