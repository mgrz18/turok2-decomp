#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"
#include "protos/player.h"

/* Releases the object the player is carrying (0xBB0) and throws it ahead of
 * the player: it is moved 102.4 units along the facing (angle 0x50 + pi),
 * raised 163.84, and given a velocity of 358.4 forward and 358.4 up. */


extern void EnemyAI_Dismount(void *obj, void *ai, CPlayer *player);
extern f32 func_002982F0(f32 x);   /* sinf */
extern f32 func_002974C0(f32 x);   /* cosf */

void CPlayer__Dismount(CPlayer *player, CGameObjectInstance *obj) {
    if (player->m_ModeFlags & 0x8000) {
        EnemyAI_Dismount(((u8 *)player->m_pOnEnemy), (u8 *)&((CGameObjectInstance *)player->m_pOnEnemy)->m_AI, player); /* N64 +0x140 */
        player->m_pOnEnemy = NULL;
        player->m_Speed = 0;
        player->m_SideSpeed = 0;
        CPlayer__SetState(player, obj, 5);
        obj->header.m_vPos.x -= func_002982F0(obj->m_RotY + 3.1415927f) * 102.399994f;
        obj->header.m_vPos.y += 163.84f;
        obj->header.m_vPos.z -= func_002974C0(obj->m_RotY + 3.1415927f) * 102.399994f;
        obj->m_vVelocity.x = func_002982F0(obj->m_RotY + 3.1415927f) * -358.4f;
        obj->m_vVelocity.y = 358.4f;
        obj->m_vVelocity.z = func_002974C0(obj->m_RotY + 3.1415927f) * -358.4f;
        CCache__SetFrameDelay(8);
    }
}
