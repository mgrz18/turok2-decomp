/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BB6C0_de_12.c, func_802BB6C0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void *D_800B8930;
extern void func_0029BE0C(u64 tim);
extern u64 func_0029BE6C(OSTimer_s *t);
int func_0029BA80(Node802C07B0 *node, u64 arg1, u64 arg2, void *arg3, void *arg4) {
    u64 value;
    node->field10 = arg1;
    node->field0 = 0;
    node->field4 = 0;
    node->field8 = arg2;
    if (arg1 == 0) {
        node->field10 = arg2;
    }
    node->field18 = arg3;
    node->field1C = arg4;
    value = func_0029BE6C(node);
    if (*(void **)D_800B8930 == node) {
        func_0029BE0C(value);
    }
    return 0;
}
