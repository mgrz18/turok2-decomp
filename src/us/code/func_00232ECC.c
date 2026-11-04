#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 unk14;
} AnimData;

/* The object as this function reads it: the word at 0xF8 (in m_BlendAnimation by the
 * LibTEngine layout) is a pointer here. */
typedef struct {
    /* 0x00 */ u8 pad0[0xF8];
    /* 0xF8 */ AnimData *anim;
} ObjAnimView;

f32 func_00232ECC(CGameObjectInstance *arg0) {
    f32 var_f0;

    var_f0 = 0.0f;
    if (((ObjAnimView *) arg0)->anim->unk14 == 0) {
        var_f0 = arg0->m_AI.m_vFlinchAxis.z;
    }
    return var_f0;
}
