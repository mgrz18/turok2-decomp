/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00281A70.c, func_00281A70.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;
typedef struct Vec3f {
    f32 x, y, z;
} Vec3f;
typedef struct B18Ent B18Ent;
/* Turok 2 layout (Rage Wars offsets in parentheses). */
struct B18Ent {
    u8 pad0[2];
    u16 type;           /* 0x002 (0x004) */
    Vec3f pos;          /* 0x004 (0x008) */
    u8 pad10[0xFC];
    B18Ent *owner;      /* 0x10C (0x12C) */
    u8 pad110[0x18];
    s16 unk128;         /* 0x128 (0x14C) */
    u8 pad12A[0x1A];
    s32 health;         /* 0x144 (0x174) */
    u8 pad148[0x3DC];
    s32 unk524;         /* 0x524 (0x5E4) */
    u8 pad528[0xDF8];
    B18Ent *next;       /* 0x1320 (0x16E0) */
};
typedef struct B18Mgr {
    u8 pad0[0xDFC];
    B18Ent *ents[0x40]; /* 0xDFC (0xE54) */
    s32 count;          /* 0xEFC */
} B18Mgr;
extern B18Mgr D_800F7078[];
extern B18Ent *D_80119890;
void func_0020EF2C(Vec3f *out, Vec3f *a, Vec3f *b);
void func_00230AB0(s32 unused, B18Ent *o, B18Ent **arr, s32 n) {
    Vec3f d;
    s32 i;
    s32 cnt;
    cnt = D_800F7078->count;
    for (i = 0; i < cnt; i++) {
        B18Ent *e = D_800F7078->ents[i];
        if (e->health > 0) {
            func_0020EF2C(&d, &e->pos, &o->pos);
            if (d.x * d.x + d.y * d.y + d.z * d.z <= 65536.0f) {
                o->unk128 = 1;
                return;
            }
        }
    }
    {
        B18Ent *p;
        for (p = D_80119890; p != ((void *)0); p = p->next) {
            if (o->owner != p && p->unk524 > 0) {
                func_0020EF2C(&d, &p->pos, &o->pos);
                if (d.x * d.x + d.y * d.y + d.z * d.z <= 65536.0f) {
                    o->unk128 = 1;
                    return;
                }
            }
        }
    }
    for (i = 0; i < n; i++) {
        B18Ent *e = arr[i];
        if (e != o && (e->type == 0xC || e->type == 0x22)) {
            func_0020EF2C(&d, &e->pos, &o->pos);
            if (d.x * d.x + d.y * d.y + d.z * d.z <= 262144.0f) {
                o->unk128 = 1;
                return;
            }
        }
    }
}
