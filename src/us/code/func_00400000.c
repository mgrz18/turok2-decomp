#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3_00400000;

/* A navigation region: a triangle with its three corners and the regions
 * across each edge (see src/us/code/func_002393D0.c). */
typedef struct Region {
    /* 0x00 */ union {
        u32 type;
        struct {
            u16 unk0;
            u16 flags; /* 0x400: registered, 0x8000: visited */
        } s;
    } u;
    /* 0x04 */ Vec3_00400000 *corner[3];
    /* 0x10 */ struct Region *neighbour[3];
} Region;

extern u8 D_800C894C; /* bottom of the stack */
extern u8 D_800F7078[];
extern Vec3_00400000 D_8011AAB4;
s32 func_002242B4(void *world, Region *region);
void func_0022441C(void *world, s32 index, s32 a2);

#define NEAR(p) (((p)->x - D_8011AAB4.x) * ((p)->x - D_8011AAB4.x) + ((p)->z - D_8011AAB4.z) * ((p)->z - D_8011AAB4.z) < 167772.14f)

/* Marks the regions around the camera, recursing into neighbours that have
 * a corner within ~410 units of it. */
void func_00400000(Region *region) {
    if (region == NULL || (u32) &region < (u32) &D_800C894C) {
        return;
    }
    if (!(region->u.s.flags & 0x400)) {
        func_0022441C(D_800F7078, func_002242B4(D_800F7078, region), 1);
    }
    region->u.s.flags |= 0x8400;
    if (!(NEAR(region->corner[0]) || NEAR(region->corner[1]) || NEAR(region->corner[2]))) {
        return;
    }
    if (region->neighbour[0] != NULL && !(region->neighbour[0]->u.s.flags & 0x8000) && (region->neighbour[0]->u.type & 6) != 2) {
        func_00400000(region->neighbour[0]);
    }
    if (region->neighbour[1] != NULL && !(region->neighbour[1]->u.s.flags & 0x8000) && (region->neighbour[1]->u.type & 6) != 2) {
        func_00400000(region->neighbour[1]);
    }
    if (region->neighbour[2] != NULL && !(region->neighbour[2]->u.s.flags & 0x8000) && (region->neighbour[2]->u.type & 6) != 2) {
        func_00400000(region->neighbour[2]);
    }
}
