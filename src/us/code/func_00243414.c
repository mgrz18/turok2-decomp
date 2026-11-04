#include "structs/vector.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/WEAPONS/80213ED4.CPP, func_80214178_de.
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
struct LevelActor;
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
struct SoundSourceNode;
struct SoundSourceNode {
    s32 unk0;
    struct SoundSourceNode *next;
    s32 value;
    s32 state;
    CVector3 position;
    void *owner;
};
struct SoundSourceNode;
struct SoundSourceNode;
typedef struct SoundSourceNode SoundSourceNode;
struct StridedTable;
struct LevelActor;
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
struct LevelActor;
typedef struct Shared_Placed Shared_Placed;
struct Shared_Placed {
    CVector3 pos;
    f32 unkC;
    s16 kind;
    s16 pad12;
};
struct Shared_Placed;
struct Shared_Placed;
struct LevelActor;
struct Dest;
typedef struct Dest Dest;
struct Source110;
typedef struct Source110 Source110;
struct StateEntry;
typedef struct StateEntry StateEntry;
struct LevelActor;
struct Source110;
struct Source110 { /* Turok 2 layout */
    char pad0[0x18];
    CVector3 velocity;
    char pad24[0xD4 - 0x24];
    u32 flags;
    f32 limit;
    s16 current_state;
    s16 previous_state;
    s16 counter;
    s8 state_changed;
    u8 animationChanged;
    u8 padE4[0x13];
    u8 stateChanged;
    u8 padF8[0x27C - 0xF8];
    u32 renderFlags;
};
struct Dest;
struct Source110;
struct StateEntry;
struct Dest {
    u32 flags;
    s32 health;
    s32 maxHealth;
    s32 handle;
    s8 level;
    u8 pad11;
    u8 state12, state13;
    s8 state14[4];
    f32 initialScale;
    u8 pad1C[0x10];
    void *table;
    StateEntry *entry;
    s8 state;
    s8 old_state;
    s8 requestedState;
    char pad37;
    u32 old_entry_flags;
    u32 entry_flags;
    s32 stateTimer;
    u8 pad44[4];
    CVector3 initialPosition;
    CVector3 previousPosition;
    f32 initialHeight;
    f32 cueTimer;
    void *field68;
    u8 reserved6C[0x18];
    u8 reserved84[8];
    f32 height8C;
    f32 height90;
    u8 state94, state95;
    s16 state96;
    u8 reserved98[4];
    f32 height9C;
    CVector3 positionA0;
    void *surfaceAC;
    CVector3 positionB0;
    void *surfaceBC;
    f32 scale; /* Turok 2: no entry_scale, count at 0xC7 */
    s16 result;
    s8 state_result;
    u8 count;
    s8 secondaryLevel;
    u8 padCD[3];
    s32 fieldD0, fieldD4;
    u8 reservedD4[0x18];
    struct SoundSourceNode *sound; /* 0xEC in Turok 2 */
    u8 pending;
    u8 pad101;
    s16 channel;
    u8 pad104[4];
    void *enter;
    void *draw;
    void *hit;
    u8 pad114[8];
    void *callback11C;
    u8 reserved120[4];
    s32 field124;
    f32 charge;
    f32 field12C;
    s32 field130;
    f32 restPitch;
    s32 field138, field13C;
    u8 pad140[4];
    s32 field144, field148, field14C, field150;
    u8 reserved154[0x1C];
};
typedef void *( *StateCallback)(struct Source110 *, struct Dest *);
struct StateEntry {
    s32 id;
    void (*enter)(struct Source110 *, struct Dest *);
    char pad8[8];
    StateCallback callback;
    f32 scale;
    s32 threshold;
    u32 flags;
};
struct ActorStateTable {
    struct ActorStateTable *next;
    u8 pad4[0x1C];
    StateEntry entries[1];
};
struct LevelActor;
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
struct Shared_Pickup;
struct Record14;
struct LevelActor;
extern CWorld D_800F7078;
s32 func_00243414(void *arg0, void *arg1, s32 arg2);
extern void func_00246918(void *, void *);
s32 func_00243414(void *arg0, void *arg1, s32 arg2) {
    struct ActorStateTable *node;
    StateEntry *entry;
    StateEntry *scan;
    s32 flags;
    if (D_800F7078.m_Mode == 2) {
        return 1;
    }
    if (D_800F7078.m_Mode == 4) {
        ((Source110 *)arg0)->flags |= 0x100;
    }
    ((Dest *)arg1)->requestedState = ((Dest *)arg1)->state;
    ((Source110 *)arg0)->animationChanged = 1;
    ((Source110 *)arg0)->stateChanged = 1;
    if ((((Dest *)arg1)->entry != 0) && (((Dest *)arg1)->state == arg2)) {
        return 1;
    }
    node = ((Dest *)arg1)->table;
    entry = 0;
    ((Dest *)arg1)->state = arg2;
    ((Dest *)arg1)->stateTimer = 0;
    while (node != 0) {
        scan = node->entries;
        if (scan->id != -1) {
            while (scan->id != -1) {
                if (scan->id == arg2) {
                    entry = scan;
                    node = 0;
                    break;
                }
                scan = scan + 1;
            }
        }
        if (node != 0) {
            node = node->next;
        }
    }
    if (entry == 0) {
        return 0;
    }
    ((Dest *)arg1)->entry = entry;
    ((Source110 *)arg0)->renderFlags &= 0x1F80007F;
    if ((((Source110 *)arg0)->flags & 0x08000000) == 0) {
        ((Dest *)arg1)->count = 0;
        ((Dest *)arg1)->flags &= ~1U;
    }
    if (((Dest *)arg1)->sound != 0) {
        func_00246918(arg0, arg1);
    }
    if (entry->enter != 0) {
        entry->enter(arg0, arg1);
    }
    flags = entry->flags;
    ((Dest *)arg1)->entry_flags = flags;
    if (flags & 4) {
        ((Source110 *)arg0)->velocity.x = 0;
        ((Source110 *)arg0)->velocity.y = 0;
        ((Source110 *)arg0)->velocity.z = 0;
    }
    return ((Dest *)arg1)->state == arg2;
}
