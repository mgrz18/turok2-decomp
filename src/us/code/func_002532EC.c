/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0022A470.c, func_0022A470.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef float f32;
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
/* Turok 2 layout: pos at 0x4, next at 0x1320 (Rage Wars: 0x8 / 0x16E0). */
typedef struct Plr Plr;
struct Plr {
    u8 pad0[0x4];
    Vec3f pos;          /* 0x0004 */
    u8 pad10[0x1310];
    Plr *next;          /* 0x1320 */
};
typedef struct PlrGroup {
    u8 pad0[0x20];
    Plr *head;          /* 0x20 */
} PlrGroup;
extern f32 D_800A70D0;
Plr *func_002532EC(PlrGroup *g, Vec3f p) {
    Plr *a;
    Plr *best = ((void *)0);
    f32 min = D_800A70D0;
    f32 dx, dy, dz, d;
    for (a = g->head; a != ((void *)0); a = a->next) {
        dx = a->pos.x - p.x;
        dy = a->pos.y - p.y;
        dz = a->pos.z - p.z;
        d = dx * dx + dy * dy + dz * dz;
        if (d < min) {
            min = d;
            best = a;
        }
    }
    return best;
}
