/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER/8028469C_22.CPP, func_80285DB0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

struct Def {
    s32 type;                  /* 0x00 */
    s32 flags;                 /* 0x04 */
};

/* Turok 2 LevelActor: 0x288 bytes (Rage Wars 0x2E8). */
typedef struct LevelActor {
    char pad0[0x14];
    struct Def *definition;    /* 0x14 (0x18) */
    char pad18[0xD4 - 0x18];
    s32 flags;                 /* 0xD4 (0x100) */
    char padD8[0x288 - 0xD8];
} LevelActor;

typedef struct Shared_World {
    s32 mode;                  /* 0x00 */
    char pad4[0x6C - 0x4];
    void *bitSections;         /* 0x6C (0x80) */
    char pad70[0xE0 - 0x70];
    LevelActor *actors;        /* 0xE0 (0x138) */
    s32 actorCapacity;         /* 0xE4 */
    s32 actorCount;            /* 0xE8 (0x140) */
    char padEC[0x18FA8 - 0xEC];
    s32 level;                 /* 0x18FA8 (0x1B40C) */
} Shared_World;

void *func_002017D4(void *table, s32 index);
extern void func_00201818(s32 arg0, s32 arg1);

/* Set or clear the actor's bit in the current level's saved-actor bitmap. */
void func_00220260(Shared_World *arg0, void *arg1, s32 arg2) {
    LevelActor *entry;
    struct Def *cond;
    void *field80;
    void *temp_v0;
    u8 *base1;
    u8 *base2;
    s32 level;
    s32 mask;
    s32 index;
    s32 idx;
    s32 idx2;
    s32 lastOffset;
    u32 first;
    if (!(((LevelActor *)arg1)->flags & 0x80000)) {
        first = (u32)arg0->actors;
        index = -1;
        if ((u32) arg1 >= first) {
            lastOffset = arg0->actorCount * 0x288;
            lastOffset -= 0x288;
            if (first + lastOffset >= (u32) arg1) {
                index = ((u32) arg1 - first) / 0x288;
            }
        }
    } else {
        index = -1;
    }
    if (index == -1) {
        return;
    }
    level = arg0->level;
    if (arg0->mode == 2) {
        return;
    }
    entry = &arg0->actors[index];
    cond = entry->definition;
    if ((u32) (cond->type - 9) < 2) {
        if (cond->flags & 0x200) {
            return;
        }
    }
    field80 = arg0->bitSections;
    temp_v0 = func_002017D4(func_002017D4(func_002017D4(field80, 0), level), 0);
    func_002017D4(temp_v0, 0);
    func_00201818((s32) temp_v0, 1);
    base1 = (u8 *) func_002017D4(temp_v0, 1);
    base2 = base1;
    mask = 1 << (index & 7);
    if (arg2 != 0) {
        idx = index;
        if (index < 0) {
            idx = index + 7;
        }
        base1[idx >> 3] |= mask;
        return;
    }
    idx2 = index;
    if (idx2 < 0) {
        idx2 += 7;
    }
    base2[idx2 >> 3] &= ~mask;
}
