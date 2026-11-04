#include "common.h"
#include "structs/game_object.h"

/* The actor definition the object points at (header.m_pIntelligence): what
 * these callbacks read of it. animFlags are per-mode animation-flag masks
 * handed to func_00245A98 to pick an animation. */
typedef struct {
    /* 0x00 */ u8 pad0[0x6A];
    /* 0x6A */ u16 animFlags[25];
} ActorDef;

s32 func_00245A98(s32, s32, s32, s32);

extern s32 D_800AF720;

void func_00259E8C(CGameObjectInstance *arg0, s32 arg1) {
    func_00245A98((s32) arg0, arg1, (s32) &D_800AF720, (s32) ((ActorDef *) arg0->header.m_pIntelligence)->animFlags[6]);
}
