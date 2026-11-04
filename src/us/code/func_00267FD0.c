#include "common.h"
#include "structs/game_object.h"

/* The object's intelligence (header.m_pIntelligence): its type and the
 * flags at 0x48. */
struct CIntelligence {
    /* 0x00 */ u32 type;
    /* 0x04 */ u8 pad4[0x48 - 0x4];
    /* 0x48 */ s32 flags48;
};

/* For type-1 objects: whether dead (or flagged 0x800). */
s32 func_00267FD0(CGameObjectInstance *arg0) {
    s32 var_v1;
    struct CIntelligence *temp_a1;

    temp_a1 = arg0->header.m_pIntelligence;
    if (temp_a1->type == 1) {
        var_v1 = 0;
        if ((arg0->m_AI.m_Health <= 0) || (temp_a1->flags48 & 0x800)) {
            var_v1 = 1;
        }
        return var_v1;
    }
    return 0;
}
