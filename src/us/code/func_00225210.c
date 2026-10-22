/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_4/8028B64C_4.CPP, func_8028C424_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
struct LevelActor;
struct Shared_World;
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
struct StridedTable;
struct StridedTable {
    s32 stride;
    s32 count;
    char data[1];
};
struct StridedTable;
typedef struct StridedTable StridedTable;
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
    u8 kind;
    s8 team;
    char pad2;
    s8 field3;
    s16 modelIndex;
    u16 pad6;
    CVector3 position0;
    s32 model;
    ActorDefinitionHeader *definition;
    CVector3 position1;
    CVector3 motionAngles;
    void *attached;
    s32 motionState;
    s32 motionIndex;
    f32 floorHeight;
    CVector3 floorDirection;
    CVector3 scale;
    CQuatern rotation;
    f32 yaw;
    f32 eyeHeight;
    f32 transform[16];
    void *fieldB4;
    s32 fieldB8;
    void *shadow;
    u8 padC0[4];
    s32 resource;
    s32 resourceKey;
    s32 animationKey;
    s32 resourceFlags;
    s32 resourceAlignment;
    s32 resourceHandle;
    s32 resourceState;
    s32 resourceWindow;
    u16 type;
    u8 stateE6;
    u8 stateE7;
    Box bounds;
    s32 flags;
    f32 animationTime;
    s16 animation;
    s16 nextAnimation;
    u8 pad10C[2];
    s8 animationPending;
    s8 animationChanged;
    u8 pad110[0x138 - 0x110];
    u8 state138;
    u8 state139;
    u8 shadowAlpha;
    u8 state13B;
    u8 pad13C[4];
    Shared_ParticleLighting lighting[2];
    s32 behaviorFlags;
    s32 active;
    u8 pad178[4];
    s32 mask;
    u8 pad180[0x20];
    s32 collisionEnabled;
    u8 pad1A4;
    s8 animationState;
    u8 pad1A6[0xA];
    f32 motionThreshold;
    u8 pad1B4[4];
    CVector3 aimPosition;
    u8 pad1C4[0x14];
    void *controller;
    u8 pad1DC[0x14];
    char *lockOn;
    u8 pad1F4[0x23A - 0x1F4];
    s8 lean;
    u8 updateCount;
    s32 actionReady;
    u8 pad240[0x30];
    u8 effectMode;
    u8 pad271[0xB];
    void *update;
    u8 pad280[0xC];
    void (*controllerUpdate)(void *, void *);
    u8 pad290[4];
    f32 pitch;
    u8 pad298[0xC];
    f32 restPitch;
    u8 pad2A8[0x38];
    s32 renderFlags;
    s32 flags2E4;
};
struct LevelActor;
struct TableRecAt8;
typedef struct TableRecAt8 TableRecAt8;
struct Shared_World;
struct TableRecAt8;
struct TableRecAt8 {
    s32 unused;
    s32 count;
    u16 entries[1];
};
struct TableRecAt8;
typedef struct Shared_Placed Shared_Placed;
struct Shared_Placed {
    CVector3 pos;
    f32 unkC;
    s16 kind;
    s16 pad12;
};
struct ActorDefinitionHeader;
struct WorldRecAt3C08;
struct EffectSlot {
    s8 active;
    char pad1[3];
    s16 model;
    char pad6[2];
    CVector3 position;
    s32 owner;
    struct ActorDefinitionHeader *definition;
    CVector3 rotation;
    char pad28[0x28];
    s32 resource;
    s32 animation;
    s32 frame;
    s32 frameState;
    char pad60[0x108];
    s32 collisionState;
    f32 collisionScale;
    s32 collisionFlags;
    s32 angle;
    s32 angleState;
    f32 minX;
    f32 minY;
    f32 minZ;
    f32 maxX;
    f32 maxY;
    f32 maxZ;
    s32 boundsState;
    char pad198[4];
    u16 flags;
    char pad19E[0x26];
    s32 effectState;
    f32 lifetime;
    f32 lifetimeState;
    s32 state;
    s32 *counter;
    struct EffectSlot *prev;
    struct EffectSlot *next;
};
struct WorldRecAt3C08 {
    struct EffectSlot slots[32];
    struct EffectSlot *free;
    struct EffectSlot *head;
    struct EffectSlot *tail;
};
struct WorldRecAt3C08;
typedef struct WorldRecAt3C08 WorldRecAt3C08;
struct Shared_Pickup;
struct Record14;
struct LevelActor;
typedef struct WorldSector {
    void *resource;
    f32 *bounds;
    u16 ids[2];
} WorldSector;
typedef struct Shared_World {
    u8 unknown0[0x1168];
    s32 recordCount;                       /* 0x1168 */
    s32 alternateRecordCount;              /* 0x116C */
    s32 * recordIndex;                     /* 0x1170 */
    s32 * alternateRecordIndex;            /* 0x1174 */
    struct Record14 * recordArray;         /* 0x1178 */
    struct Record14 * alternateRecordArray; /* 0x117C */
} Shared_World;
struct Record14;
typedef struct Record14 Record14;
struct Record14;
struct Record14 {
    u32 coordinates[3];
    u16 lifetime;
    u8 flags;
    u8 kind;
    u8 state;
    u8 active;
    u8 mode;
    u8 reserved13;
};
struct LevelActor;
s32 func_00225210(char *object, s32 type) {
    s32 index;
    Record14 *record;
    u8 *flags;
    index = ((Shared_World *)object)->recordCount;
    record = ((Shared_World *)object)->recordArray;
    index--;
    if (index != -1) {
        flags = &record->flags;
        do {
            if (flags[1] == type &&
                (*flags & 0x40) == 0) {
                return 0;
            }
            index--;
            flags += sizeof(Record14);
        } while (index != -1);
    }
    index = ((Shared_World *)object)->alternateRecordCount;
    record = ((Shared_World *)object)->alternateRecordArray;
    index--;
    if (index != -1) {
        do {
            if (record->kind == type &&
                (record->flags & 0x40) == 0) {
                return 0;
            }
            index--;
            record++;
        } while (index != -1);
    }
    return 1;
}
