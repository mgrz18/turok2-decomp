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
void func_00264C8C(Player *player, CAI *ai, CGameObjectInstance *obj);
void func_002241D8(void *level, CGameObjectInstance *obj, s32 a, s32 animType);
void func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);

void func_0025EB80(CGameObjectInstance *obj, CAI *ai, Player *player) {
    ai->m_VarBuffer[2] = (u32) player;
    if (player != NULL) {
        func_00264C8C(player, &player->obj.m_AI, obj);
    }
    player->savedScale = obj->m_vScale;
    obj->m_dwFlags &= ~0x20000000;
    func_002241D8(D_800F7078, obj, player->unkBB4 + 1, ai->m_AnimType);
    func_00243414(obj, ai, 0x2C);
}
