/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_3/80268160_3.CPP, func_80268864_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "globals/game.h"
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
extern void AI_StartSound(u8 *, u8 *, s32);
void func_00239350(void *unused, u8 *object, s32 third, struct IntPair pair, s32 fifth, s32 sixth) {
    if (gEngineApp_m_Settings.paused == 0 && object != 0 && object[0] == 1) {
        do {
            AI_StartSound(object, object + 0x140, sixth);
        } while (0);
    }
}
