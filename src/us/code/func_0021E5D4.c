/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/80246E34_8.CPP, func_8024B2D0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
struct LevelActor;
struct CVector3_t;
struct Box;
typedef struct Box Box;
struct CVector3_t;
typedef struct CVector3_t CVector3;
struct CVector3_t;
struct CVector3_t {
    f32 x;
    f32 y;
    f32 z;
};
struct Box;
struct Box {
    CVector3 min;
    CVector3 max;
};
struct CQuatern_t;
typedef struct CQuatern_t CQuatern;
struct CQuatern_t;
struct CQuatern_t {
    float x;
    float y;
    float z;
    float w;
};
typedef struct Shared_ParticleLighting Shared_ParticleLighting;
struct Shared_ParticleLighting {
    u8 ambient[3], reserved3;
    u8 ambientCopy[3], reserved7;
    u8 diffuse[3], reservedB;
    u8 diffuseCopy[3], reservedF;
    s8 direction[3];
    u8 reserved13[5];
} __attribute__((aligned(4)));
extern int D_80119878;
struct TypeEntry;
typedef struct TypeEntry TypeEntry;
struct TypeEntry;
struct TypeEntry {
    void *unk0;
    void (*callback)(void *, void *);
};
typedef struct ActorDefinitionHeader {
    s32 kind;
    s32 gate;
    u8 pad8[4];
    s16 character;
    s8 first;
    u8 padF;
    s8 second;
    u8 pad11;
    s8 send;
    u8 pad13;
    s32 flags;
} ActorDefinitionHeader;
typedef struct LevelActor LevelActor;
struct LevelActor {
    u8 pad0[0x14];
    ActorDefinitionHeader *definition; /* 0x14 */
    u8 pad18[0xD4 - 0x18];
    s32 flags;                         /* 0xD4 */
    u8 padD8[0x207 - 0xD8];
    u8 updateCount;                    /* 0x207 */
};
struct LevelActor;
struct LevelActor;
struct ActorDefinitionHeader;
struct LevelActor;
struct LevelActor;
struct LevelActor;
struct LevelActor;
struct LevelActor;
struct LevelActor;
extern char D_800AE994[];
extern TypeEntry *D_800ACB60[];
extern char *D_80119874;
void func_0021E5D4(char *arg0)
{
    s32 type;
    s32 i;
    s32 count;
    s32 limit;
    char *object;
    TypeEntry *entry;
    type = ((struct LevelActor *)(arg0))->definition->kind;
    if ((u32)type < 13) {
        goto valid_type;
    }
zero_entry:
    entry = 0;
    goto selected;
special_entry:
    entry = (TypeEntry *)D_800AE994;
    goto selected;
valid_type:
    if (type == 11) {
        count = D_80119878;
        i = 0;
        if (count > 0) {
            limit = count;
            object = D_80119874;
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
    entry = D_800ACB60[type];
selected:
    if (entry != 0 && entry->callback != 0) {
        entry->callback(arg0, (char *)arg0 + 0x140);
    }
    if ((((struct LevelActor *)(arg0))->flags & 0x08000000) != 0) {
        (((struct LevelActor *)(arg0))->updateCount)++;
    }
}
