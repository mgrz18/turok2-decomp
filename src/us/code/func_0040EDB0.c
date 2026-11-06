#include "common.h"
#include "structs/game_object.h"
#include "structs/boss.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
} Matrix18;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[0x18 - 0x10];
    /* 0x18 */ s32 flag;
} DrawInfo;

extern ScriptAnimator D_8013023C[];   /* D_80130200.faders (structs/boss.h): the ROM uses this symbol */
extern s32 D_800B6D1C;
extern s32 D_800AB8B0;

extern void ScriptAnimator_Apply(ScriptAnimator *light);
extern void ScreenTint_UpdateMode(void);
extern void Render_DrawModel(s32 a, s32 model, s32 c, Matrix18 *mtx, s32 e, s32 f, s32 g);

void BlindOne_Draw(CGameObjectInstance *actor, CAI *ai, DrawInfo *info) {
    ScriptAnimator_Apply(&D_8013023C[ai->m_BlindOne.m_nFader]);
    ScreenTint_UpdateMode();
    switch (actor->m_nTypeFlag) {
        case 0x450:
            if (ai->m_Mode == 9) {
                info->flag = 1;
            }
            break;
        case 0x455:
            if (ai->m_Mode == 6) {
                info->flag = 1;
            }
            break;
    }
    Render_DrawModel(info->unkC, actor->m_pmtDrawMtxs, 1, (Matrix18 *) actor->m_Lights[D_800B6D1C], 0, info->flag, actor->m_AI.m_nTexture);
    D_800AB8B0 = 0;
}
