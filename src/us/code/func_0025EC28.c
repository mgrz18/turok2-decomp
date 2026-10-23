#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x000 */ CGameObjectInstance obj;
    /* 0x288 */ u8 pad288[0xBB4 - 0x288];
    /* 0xBB4 */ s32 unkBB4;
    /* 0xBB8 */ u8 padBB8[0xBD0 - 0xBB8];
    /* 0xBD0 */ CVector3 savedScale;
} Player;

extern u8 D_800F7078[];
void func_0025BA80();
void func_0025C018();
void func_002241D8(void *level, CGameObjectInstance *obj, s32 a, s32 animType);
void func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);

void func_0025EC28(CGameObjectInstance *obj, CAI *ai, Player *player) {
    obj->m_AI.m_VarBuffer[2] = 0;
    obj->m_dwFlags |= 0x20000;
    obj->m_dwFlags |= 0x10000;
    ai->m_pfDraw = func_0025BA80;
    ai->m_pfCalculateOrientationMatrix = func_0025C018;
    obj->m_vScale = player->savedScale;
    obj->m_dwFlags |= 0x20000000;
    func_00243414(obj, ai, 10);
    func_002241D8(D_800F7078, obj, player->unkBB4, ai->m_AnimType);
}
