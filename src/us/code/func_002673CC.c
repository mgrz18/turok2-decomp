/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/8024D018_3.CPP, func_8024D4AC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef float f32;
struct LevelActor;
extern f32 func_0021153C(f32 arg0);
struct CVector3_t;
extern float func_002974C0(float x);
extern float func_002982F0(float x);
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
struct Actor_func_80214310_de;
typedef struct Actor_func_80214310_de Actor_func_80214310_de;
struct Actor_func_80214310_de;
struct Actor_func_80214310_de {
    char pad0[4];
    CVector3 position; /* Turok 2 layout (0x8/0x70 in Rage Wars) */
    char pad10[0x54 - 0x10];
    f32 eye;
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
    /* no lighting[2] in Turok 2: lockOn is at 0x1C0 */
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
struct LevelActor;
struct LevelActor;
struct LevelActor;
struct LevelActor;
extern void func_0020F040(f32 *arg0);
extern void func_0020F85C(void *arg0, void *arg1, void *arg2);
extern char D_80119870;
extern f32 D_800DD78C;
extern CQuatern func_0021AE6C(char *);
extern void func_00211114(CQuatern *, f32 *);
extern char *func_002532EC(void *, CVector3);
extern f32 func_002671B4(char *);
extern void func_0020EF2C(CVector3 *, CVector3 *, CVector3 *);
extern void func_0020EFDC(CVector3 *, CVector3 *, CVector3 *);
extern void func_00210F68(CQuatern *, CQuatern *, CQuatern *);
extern void func_0020EFDC(CVector3 *out, CVector3 *a, CVector3 *b);
CQuatern func_002673CC(char *actor, CVector3 point) {
    CVector3 origin;
    CVector3 dir;
    CVector3 forward;
    CVector3 local;
    CVector3 axis;
    CQuatern turn;
    CQuatern aim;
    CQuatern result;
    f32 matrix[16];
    char *target;
    char *found;
    f32 angle;
    f32 scale;
    target = 0;
    if (actor != 0 && *(u8 *)actor == 1) {
        target = ((LevelActor *)(actor))->lockOn;
        aim = func_0021AE6C(actor);
        func_00211114(&aim, matrix);
        local.x = 0.0f;
        local.y = 0.0f;
        local.z = 1.0f;
        func_0020F85C(matrix, &local, &forward);
    } else {
        forward.x = 0.0f;
        forward.y = 0.0f;
        forward.z = 1.0f;
    }
    if (target == 0) {
        found = func_002532EC(&D_80119870, point);
        if (found != 0) {
            target = found;
        }
        if (target == 0) {
            result.x = result.y = result.z = 0.0f;
            result.w = 1.0f;
            return result;
        }
    }
    origin = ((Actor_func_80214310_de *)(target))->position;
    origin.y += ((Actor_func_80214310_de *)(target))->eye;
    origin.y += func_002671B4(target) * 0.75f;
    func_0020EF2C(&dir, &origin, &point);
    func_0020F040(&dir);
    func_0020EFDC(&axis, &forward, &dir);
    func_0020F040(&axis);
    angle = func_0021153C(forward.x * dir.x + forward.y * dir.y + forward.z * dir.z) * 0.5f;
    scale = func_002982F0(angle);
    turn.x = axis.x * scale;
    turn.y = axis.y * scale;
    turn.z = axis.z * scale;
    D_800DD78C = scale;
    turn.w = func_002974C0(angle);
    if (actor != 0 && *(u8 *)actor == 1) {
        func_00210F68(&result, &aim, &turn);
    } else {
        result = turn;
    }
    return result;
}
