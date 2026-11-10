#include "structs/vector.h"
#include "globals/scene.h"

/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/WEAPONS/80204E78_5.CPP, func_802054E0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/world.h"
#include "protos/game.h"

struct Shared_Actor;
#include "common.h"
#include "structs/intelligence.h"
struct AiController;
struct Triple;
typedef struct Shared_ParticleLighting Shared_ParticleLighting;
struct Shared_ParticleLighting {
    u8 ambient[3], reserved3;
    u8 ambientCopy[3], reserved7;
    u8 diffuse[3], reservedB;
    u8 diffuseCopy[3], reservedF;
    s8 direction[3];
    u8 reserved13[5];
} __attribute__((aligned(4)));
struct Triple;
struct Triple {
    s32 x;
    s32 y;
    s32 z;
};
struct Triple;
typedef struct Triple Triple;
#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck struct_class): the position is a float struct on the host.
 * MIPS passes the Rage Wars Triple (three s32) and a CVector3 in the same registers; x86-64
 * and arm64 do not (integer vs SSE/FP registers), so Triple means three floats here. */
typedef struct { float x, y, z; } T2HostTriple;
#define Triple T2HostTriple
#endif

struct CQuatern_t;
typedef struct CQuatern_t CQuatern;
struct CQuatern_t;
struct CQuatern_t {
    float x;
    float y;
    float z;
    float w;
};
struct Shared_Actor;
typedef struct Shared_Actor Shared_Actor;
struct Info;
typedef struct Info Info;
struct Info;
struct Info {
    unsigned char raw[0x14];
    f32 scale;
};
struct Shared_Actor;
struct Shared_Entity;
struct Shared_Variant;
struct Shared_Actor {
    u8 pad0;
    u8 color;
    u8 pad1;
    s8 subtype;
    u8 pad2[20];
    struct Shared_Variant * variant;
    u8 pad3[152];
    s32 action;
    u8 pad4[0x100 - 0xB8];
    s32 weaponFlags;
    u8 pad104[0x140 - 0x104];
    Info effects[6];
    u8 pad5[8];
    struct Shared_Entity * entity;
};
extern void Actor_SpawnParticle(void *, s32, Triple, s32);
extern s32 Sound__Play(s16, CVector3, sptr, s32);
struct Player;
extern void AI_DropPickupsOnce(struct Player *, s32 *, s32, s32, s32);
typedef struct {
    s16 hi;
    s16 id;
} Half54E0;
typedef union {
    s32 whole;
    Half54E0 half;
} Word54E0;
typedef struct {
    char pad0[0x2C];
    s32 unk2C;
    s32 unk30;
    Word54E0 unk34;
    s32 unk38;
    s32 unk3C;
} Rec54E0;
typedef struct {
    char pad0[0x14];
    Rec54E0 r;
} Hold54E0;
typedef union {
    CVector3 v;
    Triple t;
} Pos54E0;
typedef struct {
    char pad0[0x4];
    Pos54E0 pos;       /* 0x04 (0x08 in Rage Wars) */
    char pad10[0x4];
    Hold54E0 *holder;  /* 0x14 (0x18) */
    char pad18[0xBC];
    s32 flags;         /* 0xD4 (0x100) */
} Obj54E0;
struct Object_func_8028B274_de {
    char pad0[4];
    s16 index;
    char pad6[0xDE];
    u16 id;
};
struct Player;
struct RangeNode;
typedef struct AiProfile {
    u8 pad0[0x7A];
    u8 sensitivity;
    s8 controlMode;
    u8 remap;
    u8 pitchMode;
    u8 pad7E[2];
    s8 character;
    s8 variant;
    u8 pad82[0xD];
    u8 bonus;
    s8 state90;
    u8 pad91[2];
    u8 difficulty;
    u8 enabled;
} AiProfile;
struct Player {
    u8 kind;
    u8 pad1[2];
    u8 playerId;
    u8 pad4[4];
    CVector3 pos;
    u8 pad14[0x4];
    void * definition;
    u8 pad1C[0x1C];
    s32 movementFlags;
    u8 pad3C[0x14];
    CVector3 scale;
    CQuatern orientation;
    f32 heading;
    u8 pad70[0x4];
    f32 matrix[16];
    u8 padB4[0x30];
    u16 objectId;
    u8 padE6[0x1A];
    s32 weaponFlags;
    u8 pad104[0xA];
    s8 animationDone;
    u8 pad10F[0x61];
    u8 motion[0x170];
    u8 pad2E0[8];
    struct Object_func_8028B274_de weaponModel;
    u8 pad3CE[0xB6];
    struct AiBehaviourTable *weaponAnimationTable;
    u8 pad488[0xF4];
    f32 weaponSpinStep;
    f32 weaponSpin;
    u8 pad584[0xC];
    s32 weaponReset;
    s32 gear;
    u8 pad598[0x3C];
    s32 slot;
    AiProfile * profile;
    void *owner;
    s32 state;
    s32 health;
    u8 pad5E8[4];
    s32 model;
    u8 pad5F0[4];
    s16 ammo[3];
    u8 pad5FA[8];
    s8 actions[22][2];
    s16 selectedAction;
    u8 pad630[0x20];
    s16 pose;
    u8 pad652[6];
    f32 poseTimer;
    u8 pad65C[4];
    s32 flags660;
    u8 pad664[0xC];
    f32 damageCooldown;
    u8 pad674[4];
    f32 rangeTimer;
    struct RangeNode *rangeNodes[3];
    u8 tuning[0x10];
    void *weaponAnimation;
    f32 steer;
    f32 pitch;
    f32 strafe;
    f32 forward;
    s32 inputFlags;
    s32 actionFlags;
    u8 pad6B4[0xC];
    f32 stick;
    s32 state6C4;
    f32 landing;
    s32 state6CC;
    s32 active;
    s32 state6D4;
    s32 state6D8;
    s32 state6DC;
    f32 movementScale;
    s32 state6E4;
    CVector3 previousPosition;
    f32 heightDelta;
    CVector3 previousTarget;
    u8 pad704[4];
    f32 state708;
    f32 state70C;
    f32 state710;
    f32 state714;
    f32 crouchDepth;
    s32 forceCrouch;
    f32 eyeOffset;
    f32 pitchOffset;
    f32 state728;
    f32 lean;
    f32 state730;
    f32 state734;
    f32 state738;
    s32 state73C;
    f32 baseHeight;
    f32 verticalOffset;
    CVector3 velocity;
    f32 velocityScale;
    s32 state758;
    s32 state75C;
    u8 pad760[0x10];
    s16 weapon;
    u8 pad772[2];
    CVector3 requestedPosition;
    f32 cameraOffset;
    f32 cameraScale;
    s32 controllerMode;
    f32 controllerTimer;
    u8 pad790[0x58];
    s32 state7E8;
    f32 state7EC;
    u8 pad7F0[0x1C];
    s32 controllerEnabled;
    u8 pad810[0x5C];
    s32 animation;
    u8 pad870[0xC8];
    s32 weaponBusy;
    u8 pad93C[0x37C];
    s32 weaponLocked;
    u8 padCBC[0x4F8];
    s32 locked;
    s32 lockState;
    void *firstHandle;
    void *secondHandle;
    u8 pad11C4[0x14];
    f32 cooldown;
    u8 pad11DC[8];
    f32 pending;
    u8 pad11E8[4];
    f32 glow;
    u8 pad11F0[4];
    f32 fireSpin;
    s32 fireFrame;
    f32 zoom;
    CVector3 aimPosition;
    s32 zoomMode;
    s32 overlay;
    u8 pad1214[0x18];
    s32 reactionFlags;
    u8 pad1230[0xC];
    u8 color[3];
    u8 pad123F;
    f32 opacity;
    u8 pad1244[0xF8];
    s32 countdown;
    s32 resetState;
    u8 pad1344[0x70];
    void *animationTable;
    u8 pad13B8[0x10];
    s32 overkill;
    u8 pad13CC[8];
    s32 toggle;
    void *reactionOwner;
    u8 pad13DC[0x74];
    s32 computer;
    struct AiController * controller;
};
struct Player;
typedef struct Player Player;
struct Object_func_8028B274_de;
struct Triple;
struct Triple;
struct Triple;
struct Triple;
struct Object_func_8028B274_de;
struct Object_func_8028B274_de;
typedef struct Object_func_8028B274_de Object_func_8028B274_de;
struct Shared_Actor;
void Destructible_GoDeathEnter(Obj54E0 *arg0, void *arg1) {
    Rec54E0 *rec;
    rec = &arg0->holder->r;
    arg0->flags = arg0->flags & 0xFFFEFFFF;
    GameObject_FireTriggers(arg0, 0x40000, arg0);
    if (rec->unk2C == 0) {
        arg0->flags &= ~0x2000;
        arg0->flags &= ~0x100;
    }
    if (gScene.m_Mode == 4) {
        if (rec->unk34.whole != -1) {
            Sound__Play(rec->unk34.half.id, arg0->pos.v, 0, -1);
        }
        if (rec->unk30 != -1) {
            Actor_SpawnParticle(arg0, rec->unk30, arg0->pos.t, 0);
        }
        AI_DropPickupsOnce(arg0, arg1, 8, rec->unk38, rec->unk3C);
    }
}
