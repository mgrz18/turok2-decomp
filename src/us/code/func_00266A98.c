#include "common.h"
#include "structs/game_object.h"

/* The object's intelligence (header.m_pIntelligence): two bytes here. */
struct CIntelligence {
    /* 0x00 */ u8 pad0[0xE];
    /* 0x0E */ u8 unkE;
    /* 0x0F */ u8 padF;
    /* 0x10 */ u8 unk10;
};

typedef struct {
    /* 0x0 */ u8 pad0[7];
    /* 0x7 */ u8 value;
} ByteArg;

void func_00266A98(CGameObjectInstance *arg0, s32 arg1, ByteArg *arg2) {
    arg0->header.m_pIntelligence->unkE = (u8) arg2->value;
    arg0->header.m_pIntelligence->unk10 = (u8) arg2->value;
}
