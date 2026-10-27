#include "common.h"

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

typedef struct {
    /* 0x000 */ u8 pad0[0xA1C];
    /* 0xA1C */ s32 flags;
    /* 0xA20 */ u8 padA20[0xA78 - 0xA20];
    /* 0xA78 */ s32 unkA78;
    /* 0xA7C */ s32 unkA7C;
    /* 0xA80 */ u8 padA80[0xBB0 - 0xA80];
    /* 0xBB0 */ u8 *carried;
} Player;

extern void func_0025EC28(void *obj, void *ai, Player *player);
extern void func_0024E5F0(Player *player, Thrown *other, s32 mode);
extern f32 func_002982F0(f32 x);   /* sinf */
extern f32 func_002974C0(f32 x);   /* cosf */
extern void func_00206068(s32 sound);

void func_002553BC(Player *player, Thrown *obj) {
    if (player->flags & 0x8000) {
        func_0025EC28(player->carried, player->carried + 0x140, player);
        player->carried = NULL;
        player->unkA78 = 0;
        player->unkA7C = 0;
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
