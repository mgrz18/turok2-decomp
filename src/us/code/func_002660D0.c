#include "common.h"
#include "structs/game_object.h"

/* The actor definition (header.m_pIntelligence): its flags. */
typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ u8 pad4[0x14 - 0x4];
    /* 0x14 */ s32 flags;
} ActorDef;

s32 func_00266570(s32, s32, s32);
extern struct CAIModeInfo D_800B2410[];
extern void func_0026609C(void);

/* AI init: mode table, step/draw callbacks, cleared mode scratch. */
void func_002660D0(CGameObjectInstance *arg0, CAI *arg1) {
    arg1->m_pModeTable = D_800B2410;
    arg1->m_pfStep = func_0026609C;
    arg1->m_pfDraw = func_00266570;
    arg1->m_VarBuffer[0] = 0;
    arg1->m_VarBuffer[1] = 0;
    arg1->m_VarBuffer[2] = 0;
    if (((ActorDef *) arg0->header.m_pIntelligence)->flags & 1) {
        arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x10000);
        return;
    }
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags & 0xFFFEFFFF);
}
