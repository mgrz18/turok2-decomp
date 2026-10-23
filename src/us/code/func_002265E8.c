/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/8028B64C_8.CPP, func_8028D74C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

/* Turok 2 layouts (Rage Wars offsets in comments). */
typedef struct LevelActor {
    u8 pad0[0xD4];
    s32 flags;                 /* 0xD4 (0x100) */
    u8 padD8[0x288 - 0xD8];
} LevelActor;                  /* 0x288 bytes (0x2E8) */

typedef struct Container {
    u8 pad0[0xE0];
    LevelActor *actors;        /* 0xE0 (0x138) */
    s32 unkE4;
    s32 actorCount;            /* 0xE8 (0x140) */
} Container;

/* Index of arg1 in the scene's actor array, or -1. */
s32 func_002265E8(void *arg0, void *arg1) {
    Container *scene = arg0;
    LevelActor *actor = arg1;
    void *base;
    if (actor->flags & 0x80000) {
        return -1;
    }
    base = scene->actors;
    if (arg1 >= base &&
        (char *)arg1 <= (char *)base + ((u32)scene->actorCount * sizeof(LevelActor) - sizeof(LevelActor))) {
        return ((u32)arg1 - (u32)base) / sizeof(LevelActor);
    }
    return -1;
}
