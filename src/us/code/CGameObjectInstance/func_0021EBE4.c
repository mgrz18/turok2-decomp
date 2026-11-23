#include "common.h"
#include "structs/intelligence.h"

typedef struct {
    /* 0x000 */ u8 pad000[0x14];
    /* 0x014 */ CIntelligence *def;
    /* 0x018 */ u8 pad018[0x15C];
    /* 0x174 */ s8 mode;
    /* 0x175 */ u8 pad175;
    /* 0x176 */ s8 prevMode;
} Func0021EBE4Obj;

void CGameObjectInstance__GetDeathModels(Func0021EBE4Obj *obj, s32 offset, s32 *outA, s32 *outB) {
    CIntelligence *def;
    s32 mode;

    def = obj->def;
    if (def->m_Type == 1) {
        if (obj->mode == 60) {
            mode = obj->prevMode;
        } else {
            mode = obj->mode;
        }
        switch ((u32) (mode - 54)) {
            case 0:
                *outA = *outB = def->m_Enemy.m_DeathModels[0];
                return;
            case 1:
                *outA = *outB = def->m_Enemy.m_DeathModels[1];
                return;
            case 2:
                *outA = *outB = def->m_Enemy.m_DeathModels[2];
                return;
            case 3:
                *outA = *outB = def->m_Enemy.m_DeathModels[3];
                return;
            case 4:
                *outA = *outB = def->m_Enemy.m_DeathModels[4];
                return;
            case 5:
                *outA = *outB = def->m_Enemy.m_DeathModels[5];
                return;
        }
    }
    *outA = ((s8 *) def)[offset + 0xE];
    *outB = ((s8 *) def)[offset + 0x10];
}
