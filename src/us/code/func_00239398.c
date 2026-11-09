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
void func_00239398(void *unused, u8 *object, s32 third, struct IntPair pair) {
    if (object != 0 && object[0] == 1) {
        AI_StopSound(object, object + T2_OFFSETOF(CGameObjectInstance, m_AI));
    }
}
extern void AI_StopSound(u8 *, u8 *);
