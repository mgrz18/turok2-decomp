#include "common.h"
#include "structs/player.h"

/* Releases the object the player is carrying (0xBB0) and throws it ahead of
 * the player: it is moved 102.4 units along the facing (angle 0x50 + pi),
 * raised 163.84, and given a velocity of 358.4 forward and 358.4 up. */

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ f32 x, y, z;
    /* 0x10 */ u8 pad10[8];
    /* 0x18 */ f32 vx, vy, vz;
    /* 0x24 */ u8 pad24[0x50 - 0x24];
    /* 0x50 */ f32 angle;
} Thrown;

extern void func_0025EC28(void *obj, void *ai, CPlayer *player);
extern void func_0024E5F0(CPlayer *player, Thrown *other, s32 mode);
extern f32 func_002982F0(f32 x);   /* sinf */
extern f32 func_002974C0(f32 x);   /* cosf */
extern void func_00206068(s32 sound);

void func_002553BC(CPlayer *player, Thrown *obj) {
    if (player->m_ModeFlags & 0x8000) {
        func_0025EC28(((u8 *)player->m_pOnEnemy), ((u8 *)player->m_pOnEnemy) + 0x140, player);
        player->m_pOnEnemy = NULL;
        player->m_Speed = 0;
        player->m_SideSpeed = 0;
        func_0024E5F0(player, obj, 5);
        obj->x -= func_002982F0(obj->angle + 3.1415927f) * 102.399994f;
        obj->y += 163.84f;
        obj->z -= func_002974C0(obj->angle + 3.1415927f) * 102.399994f;
        obj->vx = func_002982F0(obj->angle + 3.1415927f) * -358.4f;
        obj->vy = 358.4f;
        obj->vz = func_002974C0(obj->angle + 3.1415927f) * -358.4f;
        func_00206068(8);
    }
}
