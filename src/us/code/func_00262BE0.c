#include "common.h"
#include "structs/game_object.h"

/* A hit, as func_00245828 fills it in (source, damage, radius, flags, ...). */
typedef struct {
    /* 0x00 */ s32 source;
    /* 0x04 */ s32 damage;
    /* 0x08 */ s32 radius;
    /* 0x0C */ s32 flags;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ u8 pad14[4];
} DamageEvent; /* size 0x18 */

s32 func_00243414(s32, s32, s32);

void func_00262BE0(s32 arg0, CAI *arg1, DamageEvent *arg2) {
    s32 var_v1;

    var_v1 = arg1->m_Health - arg2->damage;
    if (var_v1 < 0) {
        var_v1 = 0;
    }
    arg1->m_Health = var_v1;
    if (var_v1 == 0) {
        func_00243414(arg0, (s32) arg1, 0x40);
    }
}
