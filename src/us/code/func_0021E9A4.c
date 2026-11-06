/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80246E44_de_14.c, func_8024B6A0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern s32 CGameObjectInstance__SetAnim(void *arg0, s32 arg1, s32 arg2);
extern s32 CGameObjectInstance__PickRandomAnim(void *, s32, s32);
s32 CGameObjectInstance__SetAnimByAction(void *a, s32 c, s32 flag) {
    s32 temp_v0 = CGameObjectInstance__PickRandomAnim(a, c, -1);
    if (temp_v0 != -1) {
        return CGameObjectInstance__SetAnim(a, temp_v0, flag);
    }
    return 0;
}
s32 CGameObjectInstance__SetAnim(void *arg0, s32 arg1, s32 arg2);
extern s32 CGameObjectInstance__PickRandomAnim(void *arg0, s32 arg1, s32 arg2);
