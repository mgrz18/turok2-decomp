#include "common.h"
#include "gfx.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Drawable {
    u8 type;                    /* 0x00: 0 box, 1 point, 3 ... */
    u8 pad1[3];
    Vec3 pos;                   /* 0x04 */
    u8 pad10[0x68 - 0x10];
    Vec3 min;                   /* 0x68 */
    Vec3 max;                   /* 0x74 */
} Drawable;

typedef struct SortEntry {
    Drawable *item;             /* 0x00 */
    f32 dist;                   /* 0x04: squared, doubled */
} SortEntry;

typedef struct DrawList {
    u8 pad0[0x1064];
    s32 count;                  /* 0x1064 */
    Drawable *items[1];         /* 0x1068 */
} DrawList;

Vec3 func_00280E40(void *cam);  /* camera position */
void func_00226F90(void *base, s32 count, s32 size, s32 (*cmp)(), s32 (*swap)());
s32 func_00226D30();
s32 func_00226CD0();
void func_0020B5D4(void);
void func_0020B624(void);
void func_00214844(Drawable *item, void *cam);
void func_0021D39C(Drawable *item, void *cam);
void func_002152AC(Drawable *item);

/* Sorts the list by distance from the camera, then draws it. */
void func_0022077C(DrawList *list, void *cam) {
    SortEntry entries[0x800];
    s32 count;
    s32 i;
    f32 cx, cy, cz;
    f32 dx, dy, dz;
    Drawable *item;

    count = list->count;
    if (count == 0) {
        return;
    }
    {
        Gfx *g = D_800D8E24++;
        g->w0 = 0xDB040004;
        g->w1 = 1;
    }
    {
        Gfx *g = D_800D8E24++;
        g->w0 = 0xDB04000C;
        g->w1 = 1;
    }
    {
        Gfx *g = D_800D8E24++;
        g->w0 = 0xDB040014;
        g->w1 = 0xFFFF;
    }
    {
        Gfx *g = D_800D8E24++;
        g->w0 = 0xDB04001C;
        g->w1 = 0xFFFF;
    }
    cx = func_00280E40(cam).x * -2.0f;
    cy = func_00280E40(cam).y * -2.0f;
    cz = func_00280E40(cam).z * -2.0f;
    for (i = 0; i < count; i++) {
        item = list->items[i];
        switch (item->type) {
        case 0:
            dx = item->min.x + item->max.x + cx;
            dy = item->min.y + item->max.y + cy;
            dz = item->min.z + item->max.z + cz;
            break;
        case 1:
            dx = item->pos.x * 2.0f + cx;
            dy = item->pos.y * 2.0f + cy;
            dz = item->pos.z * 2.0f + cz;
            break;
        case 3:
        default:
            dx = item->pos.x * 2.0f + cx;
            dy = item->pos.y * 2.0f + cy;
            dz = item->pos.z * 2.0f + cz;
            break;
        }
        entries[i].item = item;
        entries[i].dist = dx * dx + dy * dy + dz * dz;
    }
    func_00226F90(entries, count, sizeof(SortEntry), func_00226D30, func_00226CD0);
    func_0020B5D4();
    for (i = 0; i < count; i++) {
        item = entries[i].item;
        switch (item->type) {
        case 0:
            func_00214844(item, cam);
            break;
        case 3:
            func_002152AC(item);
            break;
        case 1:
            func_0021D39C(item, cam);
            break;
        }
    }
    func_0020B624();
}
