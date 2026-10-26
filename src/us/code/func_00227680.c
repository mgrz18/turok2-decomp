#include "common.h"

typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;

typedef struct Pickup {
    /* 0x000 */ u8 pad0[0x4];
    /* 0x004 */ Vec3f pos;
    /* 0x010 */ u8 pad10[0x4];
    /* 0x014 */ void *def;
    /* 0x018 */ Vec3f vel;
    /* 0x024 */ u8 pad24[0x14C - 0x24];
    /* 0x14C */ Vec3f boundsMin;
    /* 0x158 */ Vec3f boundsMax;
    /* 0x164 */ u8 pad164[0x19C - 0x164];
    /* 0x19C */ f32 timer;
} Pickup;

extern f32 D_800B6D28;              /* frame time scale */
extern u8 D_800F1360[];
extern u8 D_800F1EF0[];

void func_002156FC(Pickup *obj);
void func_00239F00(Pickup *obj, f32 x, f32 y, f32 z, void *arg4);

/* Counts the timer down, moves the object (func_002156FC), stops it unless its definition is
 * D_800F1360, places it at its position and refreshes its bounding box, 81.92 each way. */
void func_00227680(Pickup *obj) {
    Vec3f prev;

    if (obj->timer > 0.0f) {
        obj->timer -= D_800B6D28;
        prev = obj->pos;
    }
    func_002156FC(obj);
    if (obj->def != D_800F1360) {
        obj->vel.x = 0.0f;
        obj->vel.y = 0.0f;
        obj->vel.z = 0.0f;
    }
    func_00239F00(obj, obj->pos.x, obj->pos.y, obj->pos.z, D_800F1EF0);
    obj->boundsMin.x = obj->pos.x - 81.92f;
    obj->boundsMin.y = obj->pos.y - 81.92f;
    obj->boundsMin.z = obj->pos.z - 81.92f;
    obj->boundsMax.x = obj->pos.x + 81.92f;
    obj->boundsMax.y = obj->pos.y + 81.92f;
    obj->boundsMax.z = obj->pos.z + 81.92f;
}
