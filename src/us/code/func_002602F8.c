#include "common.h"
#include "structs/game_object.h"

/* The actor definition (header.m_pIntelligence): from its info block at 0x14,
 * the mode mask word, 5 bits per mode slot. */
typedef struct {
    /* 0x00 */ u8 pad0[0x74];
    /* 0x74 */ u16 modeMask;
} ActorInfo;

typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ u8 pad4[0x14 - 0x4];
    /* 0x14 */ ActorInfo info;
} ActorDef;

/* The AI mode scratch (CAI::m_VarBuffer at 0x10C): three slot flags. */
typedef struct {
    /* 0x000 */ u8 pad0[0x130];
    /* 0x130 */ s8 slot[3];
} AI;

s32 func_002602F8(CGameObjectInstance *arg0, AI *arg1) {
    ActorInfo *temp_a0;

    temp_a0 = &((ActorDef *) arg0->header.m_pIntelligence)->info;
    if (((arg1->slot[0] != 0) && (temp_a0->modeMask & 0x1F)) || ((arg1->slot[1] != 0) && (temp_a0->modeMask & 0x3E0)) || ((arg1->slot[2] != 0) && (temp_a0->modeMask & 0x7C00))) {
        return 1;
    }
    return 0;
}
