#include "common.h"

typedef struct Floor {
    u8 data[0x20];
} Floor;                        /* 0x20 bytes */

typedef struct FloorCell {
    f32 minX, minZ;             /* 0x00 */
    f32 maxX, maxZ;             /* 0x08 */
    u32 count;                  /* 0x10: floors in this cell, in order */
} FloorCell;                    /* 0x14 bytes */

typedef struct FloorGrid {
    s32 pad0;
    s32 count;                  /* 0x04 */
    FloorCell cells[1];         /* 0x08 */
} FloorGrid;

typedef struct World {
    u8 pad0[0x58];
    void *data;                 /* 0x58 */
} World;

extern s32 D_800B6D54;

void *func_002017D4(void *table, s32 index);
s32 func_0026BB9C(Floor *floor, f32 x, f32 z);      /* point over floor */
f32 func_0026BE60(Floor *floor, f32 x, f32 z);      /* floor height at */

/* The floor under pos whose height is closest to it, or NULL. */
Floor *func_00220A1C(World *world, f32 *pos) {
    Floor *floors;
    Floor *floor;
    Floor *best;
    FloorGrid *grid;
    FloorCell *cell;
    FloorCell *cells;
    s32 cellCount;
    s32 k;
    s32 i;
    u32 j;
    f32 d;
    f32 bestDist;

    if (D_800B6D54 == 0) {
        return NULL;
    }
    floors = (Floor *)((u8 *)func_002017D4(world->data, 2) + 8);
    k = 0;
    i = 0;
    bestDist = 0.0f;
    best = NULL;
    grid = func_002017D4(world->data, 3);
    cellCount = grid->count;
    cells = grid->cells;
    for (; i < cellCount; i++) {
        cell = &cells[i];
        if (pos[0] >= cell->minX && pos[0] <= cell->maxX && pos[2] >= cell->minZ && pos[2] <= cell->maxZ) {
            j = 0;
            if (cell->count != 0) {
                floor = (Floor *)(k * sizeof(Floor) + (u32)floors);
                do {
                    if (func_0026BB9C(floor, pos[0], pos[2])) {
                        d = pos[1] - func_0026BE60(floor, pos[0], pos[2]);
                        if (d < 0.0f) {
                            d = -d;
                        }
                        if (best == NULL || d < bestDist) {
                            best = floor;
                            bestDist = d;
                        }
                    }
                    floor++;
                    k++;
                    j++;
                } while (j < cell->count);
            }
        } else {
            k += cell->count;
        }
    }
    return best;
}
