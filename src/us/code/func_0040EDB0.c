#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x18];
} Matrix18;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 unkC;
    /* 0x10 */ u8 pad10[0x18 - 0x10];
    /* 0x18 */ s32 flag;
} DrawInfo;

typedef struct {
    /* 0x00 */ u8 pad0[0x64];
} Light;

extern Light D_8013023C[];
extern s32 D_800B6D1C;
extern s32 D_800AB8B0;

extern void func_00236324(Light *light);
extern void func_00235898(void);
extern void func_0020A33C(s32 a, s32 model, s32 c, Matrix18 *mtx, s32 e, s32 f, s32 g);

void func_0040EDB0(CGameObjectInstance *actor, CAI *ai, DrawInfo *info) {
    func_00236324(&D_8013023C[ai->m_VarS32[2]]);
    func_00235898();
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
    func_0020A33C(info->unkC, actor->m_pmtDrawMtxs, 1, (Matrix18 *) actor->m_Lights[D_800B6D1C], 0, info->flag, actor->m_AI.m_nTexture);
    D_800AB8B0 = 0;
}
