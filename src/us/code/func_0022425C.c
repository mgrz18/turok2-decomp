#include "structs/vector.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SCENE/80286050_8.CPP, func_8028B2F8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/intelligence.h"
#include "structs/world.h"
struct LevelActor;
struct Box;
typedef struct Box Box;
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
    CIntelligence *definition;
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
struct WorldRecAt3C08;
struct EffectSlot {
    s8 active;
    char pad1[3];
    s16 model;
    char pad6[2];
    CVector3 position;
    s32 owner;
    struct CIntelligence *definition;
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
void *CISet__GetBlock(void *table, s32 index);
void *CScene__GetRegionInfo(CScene *arg0, u16 *arg1) {
    s32 base;
    s32 idx;
    if (arg1 == 0) {
        return 0;
    }
    base = ((char *)CISet__GetBlock(arg0->m_pRegionData, 0)) + 8;
    idx = *arg1;
    return (void *)(base + idx * 0x64);
}
