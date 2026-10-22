/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0022A4F4.c, func_0022A4F4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
typedef struct Plr Plr;
typedef struct PlrGroup PlrGroup;
/* Turok 2 player: only the fields used here (Rage Wars: pos 0x8, unk5E4, next 0x16E0). */
struct Plr {
    u8 pad0[0x4];
    Vec3f pos;      /* 0x4 */
    u8 pad10[0x514];
    s32 unk524;     /* 0x524 */
    u8 pad528[0xDF8];
    Plr *next;      /* 0x1320 */
};
struct PlrGroup {
                 s32* unk0;
                 Plr* unk4;
                 s32 unk8;
                 u8 padC[0x14];
                 Plr* head;
                 u8 pad24[0x1C];
                 s32 unk40;
};
extern f32 D_800A70D4;
Plr *func_00253370(PlrGroup *g, Vec3f p, Plr *skip) {
    Plr *a;
    Plr *best = ((void *)0);
    f32 min = D_800A70D4;
    f32 dx, dy, dz, d;
    for (a = g->head; a != ((void *)0); a = a->next) {
        if (a->unk524 != 0 && a != skip) {
            dx = a->pos.x - p.x;
            dy = a->pos.y - p.y;
            dz = a->pos.z - p.z;
            d = dx * dx + dy * dy + dz * dz;
            if (d < min) {
                min = d;
                best = a;
            }
        }
    }
    return best;
}
