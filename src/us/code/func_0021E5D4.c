#include "structs/vector.h"
#include "globals/game.h"
#include "globals/player.h"
#include "globals/object.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/80246E34_8.CPP, func_8024B2D0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/intelligence.h"
struct LevelActor;
typedef struct Shared_ParticleLighting Shared_ParticleLighting;
struct Shared_ParticleLighting {
    u8 ambient[3], reserved3;
    u8 ambientCopy[3], reserved7;
    u8 diffuse[3], reservedB;
    u8 diffuseCopy[3], reservedF;
    s8 direction[3];
    u8 reserved13[5];
} __attribute__((aligned(4)));
struct TypeEntry;
typedef struct TypeEntry TypeEntry;
struct TypeEntry;
struct TypeEntry {
    void *unk0;
    void (*callback)(void *, void *);
};
typedef struct LevelActor LevelActor;
struct LevelActor {
    u8 pad0[0x14];
    CIntelligence *definition; /* 0x14 */
    u8 pad18[0xD4 - 0x18];
    s32 flags;                         /* 0xD4 */
    u8 padD8[0x207 - 0xD8];
    u8 updateCount;                    /* 0x207 */
};
struct LevelActor;
struct LevelActor;
struct LevelActor;
struct LevelActor;
struct LevelActor;
struct LevelActor;
struct LevelActor;
struct LevelActor;
void func_0021E5D4(char *arg0)
{
    s32 type;
    s32 i;
    s32 count;
    s32 limit;
    char *object;
    TypeEntry *entry;
    type = ((struct LevelActor *)(arg0))->definition->m_Type;
    if ((u32)type < 13) {
        goto valid_type;
    }
zero_entry:
    entry = 0;
    goto selected;
special_entry:
    entry = (TypeEntry *) &gWeaponActorTypeInfo;
    goto selected;
valid_type:
    if (type == 11) {
        count = gEngineApp_m_Game_m_PlayerList_m_nPlayers;
        i = 0;
        if (count > 0) {
            limit = count;
            object = gEngineApp_m_Game_m_PlayerList_m_Players;
            do {
                if (object == arg0) {
                    goto zero_entry;
                }
                if (object + 0x288 == arg0) {
                    goto special_entry;
                }
                i++;
                object += 0x1328;
            } while (i < limit);
        }
    }
    entry = (TypeEntry *) gIntelligenceTypeInfo[type];
selected:
    if (entry != 0 && entry->callback != 0) {
        entry->callback(arg0, (char *)arg0 + 0x140);
    }
    if ((((struct LevelActor *)(arg0))->flags & 0x08000000) != 0) {
        (((struct LevelActor *)(arg0))->updateCount)++;
    }
}
