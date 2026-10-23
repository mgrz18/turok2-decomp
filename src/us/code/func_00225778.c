/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/CODE_8028C490.CPP, func_8028CAE0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned short u16;
typedef signed int s32;
typedef float f32;

/* Turok 2 LevelActor: 0x288 bytes (Rage Wars 0x2E8). */
typedef struct LevelActor {
    char pad0[0x4];
    f32 x;                     /* 0x04 (Rage Wars 0x08) */
    f32 y;                     /* 0x08 */
    f32 z;                     /* 0x0C */
    char pad10[0x4];
    s32 *kind;                 /* 0x14 def (Rage Wars 0x18) */
    char pad18[0xB8 - 0x18];
    u16 team;                  /* 0xB8 type id (Rage Wars 0xE4) */
    char padBA[0x144 - 0xBA];
    s32 active;                /* 0x144 health (Rage Wars 0x174) */
    char pad148[0x288 - 0x148];
} LevelActor;

typedef struct World {
    char pad0[0xE0];
    LevelActor *objects;       /* 0xE0 (Rage Wars 0x138) */
    char padE4[0x4];
    s32 count;                 /* 0xE8 (Rage Wars 0x140) */
} World;

/* Nearest other actor matching team/kind (-1 = any), optionally alive only. */
LevelActor *func_00225778(World *world, LevelActor *self, s32 team, s32 kind, s32 activeOnly) {
    s32 i;
    LevelActor *obj;
    LevelActor *best;
    f32 min;
    f32 dx, dy, dz, distSq;
    s32 count;
    i = 0;
    min = 3.4028235e38f;
    count = world->count;
    best = 0;
    for (; i < count; i++) {
        obj = &world->objects[i];
        if (obj == self) {
            continue;
        }
        if (team != -1 && obj->team != team) {
            continue;
        }
        if (kind != -1 && *obj->kind != kind) {
            continue;
        }
        if (activeOnly && obj->active == 0) {
            continue;
        }
        dx = self->x - obj->x;
        dy = self->y - obj->y;
        dz = self->z - obj->z;
        distSq = dx * dx + dy * dy + dz * dz;
        if (distSq < min) {
            min = distSq;
            best = obj;
        }
    }
    return best;
}
