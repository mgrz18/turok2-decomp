/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER/80206258.CPP, func_802063EC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;
struct LevelActor;
struct CVector3_t;
struct Triple;
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
struct Triple;
struct Triple {
    s32 x;
    s32 y;
    s32 z;
};
struct Triple;
typedef struct Triple Triple;
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
struct ActorMotionState;
struct LevelActor;
typedef struct ActorMotionHooks { void *reserved[2]; void (*onStep)(void *, void *); u8 padC[0x10]; s32 flags; } ActorMotionHooks;
typedef struct ActorMotionState {
    s32 flags;
    u8 pad4[0x28];
    void * table;
    ActorMotionHooks * hooks;
    s8 mode;
    s8 action;
    u8 pad36;
    s8 state;
    u8 pad38[0x8];
    f32 timer;
    u8 pad44[0x4];
    CVector3 position;
    u8 pad54[0xC];
    f32 height;
    f32 speed;
    u8 pad68[0x2C];
    s8 routeMode;
    u8 route;
    u16 start;
    u16 destination;
    u8 pad9A[0x6];
    CVector3 base;
    u8 padAC[0x1F];
    s8 actionPending;
    u8 padCC[0x3C];
    void * update;
    void * secondaryUpdate;
    void * event;
    u8 pad114[8];
    void * transform;
    u8 pad120[0x4];
    s32 unk124;
    s32 unk128;
    s32 unk12C;
    s32 unk130;
    f32 yaw;
    f32 pitch;
    f32 progress;
    s32 unk140;
    s32 unk144;
    s32 unk148;
    s32 unk14C;
    u8 pad150[0x18];
    s32 unk168;
    s32 unk16C;
} ActorMotionState;
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
struct LevelActor;
struct ActorDefinitionHeader;
struct Triple;
struct Triple;
struct Triple;
struct LevelActor;
/* Turok 2 layouts (Rage Wars offsets in comments). */
typedef struct T2Actor {
    u8 pad0[0x18];
    CVector3 position;         /* 0x18 (0x1C) */
    u8 pad24[0x50 - 0x24];
    f32 yaw;                   /* 0x50 (0x6C) */
} T2Actor;

typedef struct T2MotionState {
    u8 pad0[0x10C];
    s32 unk10C;                /* 0x10C (0x124) */
    s32 counter;               /* 0x110 (0x128) */
} T2MotionState;

extern char D_8010ABA0;
extern s32 func_00228108(void *arg0, s32 arg1, s32 arg2, s32 arg3,
                         s32 arg4, Triple arg5, f32 arg6, CVector3 arg7,
                         Triple arg8, void *arg9);
void func_002638A0(T2Actor *arg0, T2MotionState *arg1, Triple arg2, Triple arg3,
                   s32 arg4, s32 arg5, s32 arg6) {
    if (func_00228108(&D_8010ABA0, arg5, 0, arg6,
                      arg4, arg2,
                      arg0->yaw,
                      arg0->position, arg3,
                      &arg1->unk10C) != 0) {
        arg1->counter -= 1;
    }
}
