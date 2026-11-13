/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028784C.c, func_8028787C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */
#include "globals/scene.h"
#include "protos/iset.h"

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
extern void LevelEvent_Fire(Entity *entity);
#ifdef TARGET_PORT
/* The port (round 13 b2): the world by member. The N64 view reads the bit sections at 0x6C and the
 * level at 0x18FA8, other members of the host CScene: no fired event was ever restored when a level
 * loaded, so what they had done (a used key slot s collision off, ...) was lost on a later visit. */
#include "structs/world.h"
#include "globals/scene.h"
void CScene__RestoreFiredEvent(World_func_8028787C_de *world_, Entity *entity) {
    CScene *world = (CScene *) world_;
    s32 index = entity->index;
    void *mask;
    unsigned char *bits;
    void *flags;

    mask = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(world->m_pBitSections, 0), world->m_nLevel), 2);
    bits = (unsigned char *) CISet__GetBlock(mask, 1);
    if (bits[index / 8] & (1 << (index & 7))) {
        if (entity->flag != 0) {
            flags = CISet__GetBlock(gScene.m_pBitSections, 1);
            if (((unsigned char *) CISet__GetBlock(flags, 1))[entity->flag] != 0) {
                return;
            }
        }
        LevelEvent_Fire(entity);
    }
}
#else
void CScene__RestoreFiredEvent(World_func_8028787C_de *world, Entity *entity) {
    s32 index;
    s32 level;
    void *mask;
    unsigned char *bits;
    void *flags;
    s32 flag;
    s32 bit;
    index = entity->index;
    level = world->level;
    mask = CISet__GetBlock(CISet__GetBlock(CISet__GetBlock(world->levels, 0), level), 2);
    CISet__GetBlock(mask, 0);
    CISet__GetBlockSize(mask, 1);
    bits = CISet__GetBlock(mask, 1);
    bit = 1 << (index & 7);
    if (bits[index / 8] & bit) {
        if (entity->flag != 0) {
            flag = entity->flag;
            flags = CISet__GetBlock(gScene.m_pBitSections, 1);
            CISet__GetBlock(flags, 0);
            if (((unsigned char *)CISet__GetBlock(flags, 1))[flag] != 0) {
                return;
            }
        }
        LevelEvent_Fire(entity);
    }
}
#endif
