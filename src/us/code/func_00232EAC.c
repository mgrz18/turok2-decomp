#include "common.h"
#include "structs/game_object.h"

/* The pointer this reads at 0xF8 of the instance (see func_0023417C). */
typedef struct EmitCfg {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 unk14;
} EmitCfg;

typedef struct EmitActor {
    /* 0x00 */ u8 pad0[0xF8];
    /* 0xF8 */ EmitCfg *cfg;
} EmitActor;

f32 func_00232EAC(CGameObjectInstance *arg0) {
    f32 var_f0;

    var_f0 = 0.0f;
    if (((EmitActor *) arg0)->cfg->unk14 == 0) {
        var_f0 = arg0->m_AI.m_vFlinchAxis.y;
    }
    return var_f0;
}
