#include "common.h"
#include "structs/swoosh.h"

s32 CMtxF__ClampEntries(sptr);
sptr Swoosh_NewNode(sptr, sptr, sptr);

s32 CMtxF__Copy(s32 *, sptr);              /* extern */
s32 CMtxF__ScaleAxes(s32 *, f32, f32, f32);    /* extern */


/* Adds a point to every live swoosh of object arg1, at its record bone of matrices arg2. */
void SwooshSystem_AddBonePoints(CSwooshSystem *arg0, sptr arg1, sptr arg2) {
    u8 sp10[0x40];
    Swoosh *var_s0;

    var_s0 = arg0->m_SwooshPool.used.head;
    if (var_s0 != NULL) {
        do {
            if (((sptr) var_s0->owner == arg1) && (var_s0->time > 0.0f)) {
                CMtxF__Copy(sp10, arg2 + (var_s0->info->bone << 6));
                CMtxF__ScaleAxes(sp10, 0.00000015258789f, 0.00000015258789f, 0.00000015258789f);
                CMtxF__ClampEntries((sptr) sp10);
                Swoosh_NewNode((sptr) arg0, (sptr) var_s0, (sptr) sp10);
            }
            var_s0 = var_s0->next;
        } while (var_s0 != NULL);
    }
}
