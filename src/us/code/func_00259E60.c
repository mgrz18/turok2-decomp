#include "common.h"
#include "structs/game_object.h"

s32 func_00245A98(s32, s32, s32, s32);

extern s32 D_800AF6F0;

/* The actor's intelligence block (header.m_pIntelligence): a run of
 * halfword parameters from 0x76 handed to func_00245A98. */
typedef struct {
    /* 0x00 */ u8 pad0[0x76];
    /* 0x76 */ u16 unk76;
    /* 0x78 */ u16 unk78;
    /* 0x7A */ u16 unk7A;
    /* 0x7C */ u16 unk7C;
    /* 0x7E */ u16 unk7E;
    /* 0x80 */ u16 unk80;
    /* 0x82 */ u16 unk82;
    /* 0x84 */ u16 unk84;
    /* 0x86 */ u16 unk86;
    /* 0x88 */ u16 unk88;
    /* 0x8A */ u16 unk8A;
    /* 0x8C */ u16 unk8C;
    /* 0x8E */ u16 unk8E;
    /* 0x90 */ u16 unk90;
    /* 0x92 */ u16 unk92;
    /* 0x94 */ u16 unk94;
    /* 0x96 */ u16 unk96;
    /* 0x98 */ u16 unk98;
    /* 0x9A */ u16 unk9A;
} IntelligenceTable;

void func_00259E60(CGameObjectInstance *arg0, s32 arg1) {
    func_00245A98((s32) arg0, arg1, (s32) &D_800AF6F0, (s32) ((IntelligenceTable *)arg0->header.m_pIntelligence)->unk76);
}
