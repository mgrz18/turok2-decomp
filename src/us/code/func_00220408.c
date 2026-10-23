/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER/8028469C_22.CPP, func_80285F58_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;

typedef struct LevelActor {
    char pad0[0xD4];
    s32 flags;              /* 0xD4 (0x100 in Rage Wars) */
    char padD8[0x288 - 0xD8];
} LevelActor;               /* 0x288 bytes (0x2E8 in Rage Wars) */

typedef struct Shared_World {
    char pad0[0x6C];
    void *bitSections;      /* 0x6C (0x80) */
    char pad70[0xE0 - 0x70];
    LevelActor *actors;     /* 0xE0 (0x138) */
    s32 actorCapacity;
    s32 actorCount;         /* 0xE8 (0x140) */
    char padEC[0x18FA8 - 0xEC];
    s32 level;              /* 0x18FA8 (0x1B40C) */
} Shared_World;

void *func_002017D4(void *table, s32 index);
extern void func_00201818(s32 arg0, s32 arg1);

s32 func_00220408(void *arg0, void *arg1) {
    Shared_World *world = arg0;
    LevelActor *actor = arg1;
    void *field80;
    void *temp_v0;
    u8 *base;
    s32 field1B40C;
    s32 mask;
    s32 index;
    s32 i;
    s32 lastOffset;
    u32 first;
    if (!(actor->flags & 0x80000)) {
        first = ((u32)world->actors);
        index = -1;
        if ((u32) arg1 >= first) {
            lastOffset = world->actorCount * 0x288;
            lastOffset -= 0x288;
            if (first + lastOffset >= (u32) arg1) {
                index = ((u32) arg1 - first) / 0x288;
            }
        }
    } else {
        index = -1;
    }
    if (index == -1) {
        return 0;
    }
    field80 = world->bitSections;
    field1B40C = world->level;
    temp_v0 = func_002017D4(func_002017D4(func_002017D4(field80, 0), field1B40C), 0);
    func_002017D4(temp_v0, 0);
    func_00201818((s32) temp_v0, 1);
    base = (u8 *) func_002017D4(temp_v0, 1);
    mask = 1 << (index & 7);
    i = index;
    if (i < 0) {
        i += 7;
    }
    return (base[i >> 3] & mask) != 0;
}
