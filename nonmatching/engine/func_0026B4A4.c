/* NON-MATCHING: 3 of 237 words differ: only the schedule of `li $a1, 2` (the first func_002017D4 argument) against the zero inits; uses a FAKEMATCH-style duplicated branch to get the ROM's register allocation. */
#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x00 */ u8 pad0[2];
    /* 0x02 */ u16 flags;
    /* 0x04 */ Vec3f *vertices[3];
    /* 0x10 */ u8 pad10[0x20 - 0x10];
} Triangle; /* 0x20 bytes */

typedef struct {
    /* 0x00 */ f32 minX;
    /* 0x04 */ f32 minZ;
    /* 0x08 */ f32 maxX;
    /* 0x0C */ f32 maxZ;
    /* 0x10 */ u32 count; /* triangles in this group, consecutive in the list */
} TriGroup; /* 0x14 bytes */

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ s32 count;
    /* 0x08 */ TriGroup groups[1];
} TriGroupList;

typedef struct {
    /* 0x00 */ u8 pad0[0x58];
    /* 0x58 */ void *level;
} World;

extern s32 D_800B6D54;          /* level loaded */
extern World D_800F7078;
extern Triangle *D_800B2608;    /* triangle of the cached raw normal */
extern Vec3f D_800F2020;        /* cached raw normal */

extern void *func_002017D4(void *file, s32 index); /* a block of a file */
extern void func_0020EF2C(Vec3f *out, Vec3f *a, Vec3f *b);   /* out = a - b */
extern void func_0020EFDC(Vec3f *out, Vec3f *a, Vec3f *b);   /* out = a x b */

/* func_0026BB9C, inlined: is (x, z) inside the XZ projection of the triangle? */
static __inline__ s32 InTriangle(Triangle *tri, f32 x, f32 z) {
    s32 i, next;
    Vec3f *a, *b;

    if (tri == NULL) {
        return 1;
    }
    for (i = 0; i < 3; i = next) {
        next = i + 1;
        a = tri->vertices[i];
        b = tri->vertices[next % 3];
        if ((b->z - a->z) * (x - a->x) + (a->x - b->x) * (z - a->z) < 0.0f) {
            return 0;
        }
    }
    return 1;
}

static __inline__ void cross_edges(Vec3f *out, Triangle *tri) {
    Vec3f edge0;
    Vec3f edge1;

    func_0020EF2C(&edge0, tri->vertices[1], tri->vertices[0]);
    func_0020EF2C(&edge1, tri->vertices[2], tri->vertices[1]);
    func_0020EFDC(out, &edge0, &edge1);
}

/* func_0026BE60, inlined: height of a triangle's plane at (x, z). */
static __inline__ f32 TriHeight(Triangle *tri, f32 x, f32 z) {
    Vec3f normal;
    Vec3f point;

    if (tri == NULL) {
        return -14336.0f;
    }
    if (tri != D_800B2608) {
        cross_edges(&D_800F2020, tri);
    }
    normal = D_800F2020;
    D_800B2608 = tri;
    if (normal.y == 0.0f) {
        return (tri->vertices[0]->y + tri->vertices[1]->y + tri->vertices[2]->y) * (1.0f / 3);
    }
    point = *tri->vertices[0];
    return ((point.z - z) * normal.z + (point.x - x) * normal.x + point.y * normal.y) / normal.y;
}

/* The level triangle nearest below pos (other than skip), NULL if none. */
Triangle *func_0026B4A4(Triangle *skip, Vec3f pos) {
    World *world;
    Triangle *tris;
    TriGroupList *list;
    TriGroup *groups;
    TriGroup *group;
    Triangle *tri;
    Triangle *best;
    f32 bestDist;
    f32 h;
    f32 d;
    s32 index;
    s32 n;
    s32 i;
    u32 j;

    if (!D_800B6D54) {
        return NULL;
    }
    index = 0;
    i = 0;
    best = NULL;
    bestDist = 0.0f;
    world = &D_800F7078;
    tris = (Triangle *) ((u8 *) func_002017D4(world->level, 2) + 8);
    list = func_002017D4(world->level, 3);
    n = list->count;
    groups = list->groups;
    for (; i < n; i++) {
        group = &groups[i];
        if (pos.x >= group->minX && group->maxX >= pos.x && pos.z >= group->minZ && group->maxZ >= pos.z) {
            for (j = 0; j < group->count; j++) {
                tri = &tris[index];
                if (InTriangle(tri, pos.x, pos.z) && tri != skip) {
                    h = TriHeight(tri, pos.x, pos.z);
                    if (h < pos.y) {
                        d = pos.y - h;
                        if (best == NULL || d < bestDist) {
                            /* FAKEMATCH: identical branches, merged by
                             * cross-jumping; the extra uses of pos.z and
                             * group->count only reproduce the ROM's register
                             * allocation (skip in $fp, tris spilled). */
                            if (pos.z != 0.0f || group->count != 0) {
                                best = tri;
                                bestDist = d;
                            } else {
                                best = tri;
                                bestDist = d;
                            }
                        }
                    }
                }
                index++;
            }
        } else {
            index += group->count;
        }
    }
    return best;
}
