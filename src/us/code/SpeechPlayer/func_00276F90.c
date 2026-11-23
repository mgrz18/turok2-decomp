#include "common.h"
#include "structs/game_object.h"
#include "protos/iset.h"

/* The block the instance's first word points at; 0x2B50 is a table
 * handle read through CISet__GetBlock. */
typedef struct {
    /* 0x0000 */ u8 pad0[0x2B50];
    /* 0x2B50 */ s32 table;
} Owner00276F90;

typedef struct {
    /* 0x0 */ u32 value;
    /* 0x4 */ u16 unk4;
} Entry00276F90;


void SpeechPlayer_ReadHeader(CGameObjectInstance *arg0, s32 arg1) {
    f64 var_f2;
    s32 temp_s1;
    s32 temp_v0_2;
    Entry00276F90 *temp_v0;

    temp_s1 = (arg1 << 1);
    temp_v0 = CISet__GetBlock((*(Owner00276F90 **)arg0)->table, temp_s1);
    temp_v0_2 = temp_v0->value;
    var_f2 = (f64) temp_v0_2;
    if (temp_v0_2 < 0) {
        var_f2 += 4294967296.0;
    }
    arg0->m_vVelocity.z = (f32) ((f32) var_f2 * 0.01f);
    arg0->m_vWaterDisplacement.x = (f32) temp_v0->unk4;
    CISet__GetBlock((*(Owner00276F90 **)arg0)->table, temp_s1 | 1);
}
