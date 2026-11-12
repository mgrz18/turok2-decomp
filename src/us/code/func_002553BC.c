#include "common.h"
#include "structs/player.h"
#include "protos/cache.h"
#include "protos/player.h"

/* Releases the object the player is carrying (0xBB0) and throws it ahead of
 * the player: it is moved 102.4 units along the facing (angle 0x50 + pi),
 * raised 163.84, and given a velocity of 358.4 forward and 358.4 up. */

#ifdef TARGET_PORT
typedef struct Thrown {
/* the view at the host offsets of CGameObjectInstance (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Thrown(m) __builtin_offsetof(CGameObjectInstance, m)
    union {
        struct { u8 _hp_x[T2_HOFF_Thrown(header.m_vPos.x)]; f32 x; }; /* N64 0x4 */
        struct { u8 _hp_y[T2_HOFF_Thrown(header.m_vPos.y)]; f32 y; }; /* N64 0x8 */
        struct { u8 _hp_z[T2_HOFF_Thrown(header.m_vPos.z)]; f32 z; }; /* N64 0xC */
        struct { u8 _hp_vx[T2_HOFF_Thrown(m_vVelocity.x)]; f32 vx; }; /* N64 0x18 */
        struct { u8 _hp_vy[T2_HOFF_Thrown(m_vVelocity.y)]; f32 vy; }; /* N64 0x1C */
        struct { u8 _hp_vz[T2_HOFF_Thrown(m_vVelocity.z)]; f32 vz; }; /* N64 0x20 */
        struct { u8 _hp_angle[T2_HOFF_Thrown(m_RotY)]; f32 angle; }; /* N64 0x50 */
    };
} Thrown;
#else
typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ f32 x;
    /* 0x08 */ f32 y;
    /* 0x0C */ f32 z;
    /* 0x10 */ u8 pad10[8];
    /* 0x18 */ f32 vx;
    /* 0x1C */ f32 vy;
    /* 0x20 */ f32 vz;
    /* 0x24 */ u8 pad24[0x50 - 0x24];
    /* 0x50 */ f32 angle;
} Thrown;
#endif

extern void EnemyAI_Dismount(void *obj, void *ai, CPlayer *player);
extern f32 func_002982F0(f32 x);   /* sinf */
extern f32 func_002974C0(f32 x);   /* cosf */

void CPlayer__Dismount(CPlayer *player, Thrown *obj) {
    if (player->m_ModeFlags & 0x8000) {
#ifdef TARGET_PORT
        EnemyAI_Dismount(((u8 *)player->m_pOnEnemy), (u8 *)&((CGameObjectInstance *)player->m_pOnEnemy)->m_AI, player); /* N64 +0x140 */
#else
        EnemyAI_Dismount(((u8 *)player->m_pOnEnemy), ((u8 *)player->m_pOnEnemy) + 0x140, player);
#endif
        player->m_pOnEnemy = NULL;
        player->m_Speed = 0;
        player->m_SideSpeed = 0;
        CPlayer__SetState(player, obj, 5);
        obj->x -= func_002982F0(obj->angle + 3.1415927f) * 102.399994f;
        obj->y += 163.84f;
        obj->z -= func_002974C0(obj->angle + 3.1415927f) * 102.399994f;
        obj->vx = func_002982F0(obj->angle + 3.1415927f) * -358.4f;
        obj->vy = 358.4f;
        obj->vz = func_002974C0(obj->angle + 3.1415927f) * -358.4f;
        CCache__SetFrameDelay(8);
    }
}
