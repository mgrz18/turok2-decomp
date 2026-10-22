/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028784C.c, func_8028787C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
struct World_func_8028787C_de;
typedef struct World_func_8028787C_de World_func_8028787C_de;
struct World_func_8028787C_de;
struct World_func_8028787C_de {
    char pad0[0x6C];
    void *levels;                    /* 0x6C in Turok 2 (0x80 in Rage Wars) */
    char pad70[0x18FA8 - 0x70];
    s32 level;                       /* 0x18FA8 (0x1B40C in Rage Wars) */
};
struct Entity;
typedef struct Entity Entity;
struct Entity;
struct Entity {
    char pad0[0xF];
    unsigned char flag;
    char pad10[3];
    unsigned char index;
};
extern void *D_800F70E4;
extern void *func_002017D4(void *table, s32 index);
extern s32 func_00201818(void *table, s32 index);
extern void func_0026DFB0(Entity *entity);
void func_00220BF0(World_func_8028787C_de *world, Entity *entity) {
    s32 index;
    s32 level;
    void *mask;
    unsigned char *bits;
    void *flags;
    s32 flag;
    s32 bit;
    index = entity->index;
    level = world->level;
    mask = func_002017D4(func_002017D4(func_002017D4(world->levels, 0), level), 2);
    func_002017D4(mask, 0);
    func_00201818(mask, 1);
    bits = func_002017D4(mask, 1);
    bit = 1 << (index & 7);
    if (bits[index / 8] & bit) {
        if (entity->flag != 0) {
            flag = entity->flag;
            flags = func_002017D4(D_800F70E4, 1);
            func_002017D4(flags, 0);
            if (((unsigned char *)func_002017D4(flags, 1))[flag] != 0) {
                return;
            }
        }
        func_0026DFB0(entity);
    }
}
