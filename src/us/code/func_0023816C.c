#include "common.h"
#include "structs/game_object.h"


typedef struct Player {
    u8 pad0[4];
    CVector3 pos;           /* 0x4 */
    u8 pad10[0x288 - 0x10];
    CGameObjectInstance actor;        /* 0x288 */
    u8 pad[0x996 - 0x288 - sizeof(CGameObjectInstance)];
    u16 state;          /* 0x996 */
} Player;

extern void func_0020EF8C(CVector3 *out, f32 t, CVector3 *a, CVector3 *b);
extern f32 func_002671B4(Player *player);
extern f32 D_800A65B0;

s32 func_0023816C(CGameObjectInstance *actor, CVector3 *pos) {
    s32 ret = 0;
    Player *player;
    u16 state;
    CVector3 a;
    CVector3 b;

    if (actor->header.m_Type == 1 && (actor->m_dwFlags & 0x300000)) {
        player = ((Player *)actor->m_AI.m_pParent);
        if (actor == &player->actor) {
            state = player->state;
            if (state < 2 || (s16) state == 0x22) {
                a = *pos;
                b = player->pos;
                func_0020EF8C(pos, 0.75f, &a, &b);
                pos->y += func_002671B4(player) * D_800A65B0;
                ret = 1;
            }
        }
    }
    return ret;
}
