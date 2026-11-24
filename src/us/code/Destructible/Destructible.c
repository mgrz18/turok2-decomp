/* Boundary fixed in versions/cuts.us.txt: splat's AI_AnimReached (0x12C bytes) holds two functions:
 *   0x00265954  AI_AnimReached, the animation check (0xD0 bytes), then a nop;
 *   0x00265A28  two stray words (beq $18, $16, ...; sll $2, $18, 3), never run;
 *   0x00265A30  Destructible_Setup, an init callback (0x50 bytes; its pointer is in
 *               the data as 0x00265A30).
 * The check is the same as the inlined r01_check of Door_CloseUpdate and
 * func_002651FC, here out of line. */
/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0020486C.c, r01_check, with Turok 2's layouts. */
#include "common.h"
#include "structs/game_object.h"
#include "globals/ai.h"

void Destructible_Step(void *arg0, CAI *arg1);
extern void TotemDefense_Init();
#include "globals/scene.h"
void Destructible_Hurt(CGameObjectInstance *arg0, CAI *arg1, DamageEvent *arg2);

/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS TotemDefense_Init receives Destructible_Setup's own argument
 * registers (pass-through); the host passes them. */
void Destructible_Setup(CGameObjectInstance *actor, CAI *state) {
    state->m_pModeTable = gDestructibleModeTable;
    state->m_pfStep = Destructible_Step;
    state->m_pfHurt = Destructible_Hurt;
    if (actor->m_nTypeFlag == 0x64D) {
        TotemDefense_Init(actor, state);
    }
}

#include "common.h"
#include "protos/scene.h"
#include "protos/ai.h"

void Destructible_Advance(CGameObjectInstance *arg0, CAI *arg1) {
    if (CScene__GetInstanceSavedBit(&gScene, arg0) == 1) {
        CAI__SetMode(arg0, arg1, 1);
        return;
    }
    CAI__SetMode(arg0, arg1, 0);
    arg0->m_dwFlags = (s32) (arg0->m_dwFlags | 0x10000);
}

/* ABI (docs/port/ABI.md section 5, abicheck indirect missing_args): MIPS calls the mode callback
 * with this function's own a0/a1 (pass-through: the actor and its AI); the host passes them, and
 * the actor stays a full pointer (the callbacks take CGameObjectInstance *). */
void Destructible_Step(void *arg0, CAI *arg1) {
    s32 (*cb)(void *, CAI *);

    if (arg1->m_pModeCurrent != NULL) {
        cb = (s32 (*)(void *, CAI *))arg1->m_pModeCurrent->m_pfCallback;
        if (cb != NULL) {
            cb(arg0, arg1);
        }
    }
}

/* The definition value at 0x24, or 0x5334 when it is zero. */
s32 Destructible_GetIdleAnim(CGameObjectInstance *arg0) {
    s32 v = arg0->header.m_pIntelligence->m_Destructible.m_IdleAnim;
    if (v != 0) {
        return v;
    }
    return 0x5334;
}

s32 Destructible_GetGoAnim(CGameObjectInstance *arg0) {
    return arg0->header.m_pIntelligence->m_Destructible.m_GoAnim;
}

#include "structs/vector.h"
#include "globals/swoosh.h"
#include "protos/game.h"
#include "structs/word.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_00205324.c, func_00205324.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct M1Info M1Info;
typedef struct M1Def M1Def;
typedef struct M1Actor M1Actor;
typedef struct M1State M1State;
typedef struct M1Ref M1Ref;
struct M1Info {
                u32 flags;
                CWord unk4;
                s8 b8;
                u8 pad9[0x3];
                s32 unkC;
                u8 pad10[0x8];
                s32 unk18;
                s32 unk1C;
                CWord unk20;
                CWord unk24;
                CWord unk28;
                f32 f2C;
                f32 f30;
                f32 f34;
                u8 pad38[0x4];
                f32 f3C;
                CWord unk40;
                f32 f44;
                CWord unk48;
                CWord unk4C;
                u8 b50[4];
                f32 f54;
                f32 f58;
                f32 f5C;
                f32 f60;
                f32 f64;
                f32 f68;
                u8 pad6C[0x4];
                u8 b70;
                u8 b71;
};
struct M1Def {
                s32 kind;
                u32 unk4;
                u8 pad8[0xC];
                M1Info info;
};
#ifdef TARGET_PORT
#include "structs/game_object.h"
/* The port (round 11 C26): the members at their host offsets. With the N64 pads the flags (0xD4)
 * fell 8 bytes late, after the pointer-wide def, on m_rpGeometryHeaderSize: a destroyed object's
 * geometry size lost bit 0x100 (0x160 -> 0x60), and the next model request of that object (the
 * assembly plant of level 122) loaded 0x60 of its 0x2E0 bytes: Model_Decompress read past them
 * and crashed on the decompressor thread. */
struct M1Actor {
    union {
        struct { u8 _hpPos[__builtin_offsetof(CGameObjectInstance, header.m_vPos)]; CVector3 pos; };
        struct { u8 _hpDef[__builtin_offsetof(CGameObjectInstance, header.m_pIntelligence)]; M1Def *def; };
        struct { u8 _hpFlags[__builtin_offsetof(CGameObjectInstance, m_dwFlags)]; u32 flags; };
    };
};
#else
struct M1Actor { /* Turok 2 layout */
                u8 pad0[0x4];
                CVector3 pos;          /* 0x04 */
                u8 pad10[0x4];
                M1Def *def;         /* 0x14 */
                u8 pad18[0xBC];
                u32 flags;          /* 0xD4 (Rage Wars: 0x100) */
};
#endif

struct M1State {
                u8 pad0[0x4];
                s32 unk4;
                s32 unk8;
                u8 padC[0x28];
                s8 unk34;
                s8 unk35;
                s8 unk36;
                s8 unk37;
                u8 pad38[0x4];
                u32 flags3C;
                f32 f40;
                f32 f44;
                CVector3 v48;
                CVector3 v54;
                f32 f60;
                f32 f64;
                u8 pad68[0x18];
                M1Actor *unk80;
                u8 pad84[0x18];
                f32 f9C;
                CVector3 vA0;
                u8 padAC[0x1E];
                u8 unkCA;
                s8 unkCB;
                u8 padCC[0x1];
                u8 unkCD;
                u8 padCE[0x2E];
                M1Ref *unkFC;
                u8 pad100[0x8];
                void (*fn108)();
                u8 pad10C[0x18];
                CWord unk124;
                CWord unk128;
                CWord unk12C;
                CVector3 vec130;
                f32 f13C;
};
struct M1Ref {
                u8 pad0[0xC];
                s32 unkC;
};
s32 Sound__Play(s16, CVector3, sptr, sptr);
void Actor_SpawnParticle(void *, s32, CVector3, s32); /* the definition (AI) takes three; these callers pass a fourth, 0 */
void AI_DropPickupsOnce(void *, void *, u32, u32, u32);
void SwooshSet_FreeOwner(u8 *p, M1Actor *a);

void Destructible_GoEnter(M1Actor *actor, M1State *state) {
    M1Info *info = &actor->def->info;
    s32 mode;
    CScene__SetInstanceSavedBit(&gScene, actor, 1);
    GameObject_FireTriggers(actor, 0x40000, actor);
    if (state->unk4 == 0) {
        actor->flags &= ~0x10000;
    }
    if (info->unk18 == 0) {
        actor->flags &= ~0x2000;
        actor->flags &= ~0x100;
    }
    mode = gScene.m_Mode;
    if (mode == 4) {
        if (info->unk20.i != -1) {
            Sound__Play(info->unk20.h[1], actor->pos, 0, -1);
        }
        if (info->unk1C != -1) {
            Actor_SpawnParticle(actor, info->unk1C, actor->pos, 0);
        }
        AI_DropPickupsOnce(actor, state, 4, info->unk24.i, info->unk28.i);
        SwooshSet_FreeOwner(&gEngineApp_m_SwooshSystem, actor);
        if (gScene.m_Mode != mode) goto L;
    } else {
L:
        actor->flags &= ~0x100;
        actor->flags |= 0x8000000;
    }
}

typedef struct Descriptor {
    char pad0[0x14];
    s32 damageFlags;            /* 0x14 */
} Descriptor;
#ifdef TARGET_PORT
/* The port (round 11 C26): the members at their host offsets (the N64 pads put the flags on
 * m_rpGeometryHeaderSize, as in Destructible_GoEnter, func_00265B58.c; the CAI byte below read
 * another member) */
typedef struct BreakableHitContext {
    union {
        struct { char _hpDef[__builtin_offsetof(CGameObjectInstance, header.m_pIntelligence)]; Descriptor *def; };
        struct { char _hpFlags[__builtin_offsetof(CGameObjectInstance, m_dwFlags)]; s32 flags; };
    };
} BreakableHitContext;
#else
typedef struct BreakableHitContext {
    char pad0[0x14];
    Descriptor *def;            /* 0x14 (Rage Wars 0x18) */
    char pad18[0xD4 - 0x18];
    s32 flags;                  /* 0xD4 (Rage Wars 0x100) */
} BreakableHitContext;
#endif

typedef struct ObjectStateCC {
#ifdef TARGET_PORT
    char pad0[__builtin_offsetof(CAI, m_AnimRepeatedCount)];
#else
    char pad0[0xC7];
#endif
    s8 unk_C7;                  /* 0xC7 (Rage Wars 0xCB) */
} ObjectStateCC;

void Destructible_GoUpdate(BreakableHitContext *arg0, ObjectStateCC *arg1) {
    Descriptor *def = arg0->def;
    if (arg1->unk_C7 != 0 && (def->damageFlags & 0x4) != 0) {
        unsigned int flags = arg0->flags;
        flags &= ~0x2000;
        flags &= ~0x100;
        arg0->flags = flags;
    }
}

s32 Destructible_GetGoDeathAnim(CGameObjectInstance *arg0) {
    return arg0->header.m_pIntelligence->m_Destructible.m_GoDeathAnim;
}

#include "structs/world.h"

struct Shared_Actor;
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
struct Triple {
    s32 x;
    s32 y;
    s32 z;
};
typedef struct Triple Triple;
#include "structs/triple.h"

struct CQuatern_t;
typedef struct CQuatern_t CQuatern;
struct CQuatern_t {
    float x;
    float y;
    float z;
    float w;
};
typedef struct Shared_Actor Shared_Actor;
struct Info;
typedef struct Info Info;
struct Info {
    unsigned char raw[0x14];
    f32 scale;
};
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
struct Player;
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
#ifdef TARGET_PORT
#include "structs/game_object.h"
/* The port (round 11 C26): the members at their host offsets (the N64 pads put the flags on
 * m_rpGeometryHeaderSize, as in Destructible_GoEnter, func_00265B58.c). */
typedef struct {
    union {
        struct { char _hpPos[__builtin_offsetof(CGameObjectInstance, header.m_vPos)]; Pos54E0 pos; };
        struct { char _hpHolder[__builtin_offsetof(CGameObjectInstance, header.m_pIntelligence)]; Hold54E0 *holder; };
        struct { char _hpFlags[__builtin_offsetof(CGameObjectInstance, m_dwFlags)]; s32 flags; };
    };
} Obj54E0;
#else
typedef struct {
    char pad0[0x4];
    Pos54E0 pos;       /* 0x04 (0x08 in Rage Wars) */
    char pad10[0x4];
    Hold54E0 *holder;  /* 0x14 (0x18) */
    char pad18[0xBC];
    s32 flags;         /* 0xD4 (0x100) */
} Obj54E0;
#endif

struct Object_func_8028B274_de {
    char pad0[4];
    s16 index;
    char pad6[0xDE];
    u16 id;
};
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
typedef struct Player Player;
struct Object_func_8028B274_de;
typedef struct Object_func_8028B274_de Object_func_8028B274_de;

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
            Actor_SpawnParticle(arg0, rec->unk30, arg0->pos.v, 0);
        }
        AI_DropPickupsOnce(arg0, arg1, 8, rec->unk38, rec->unk3C);
    }
}

/* Turok 2: def at 0x14, flags at 0xD4 (Rage Wars 0x18 / 0x100). */
/* Turok 2: this state byte is at 0xC7 (Rage Wars 0xCB). */
typedef struct ObjectStateC7 {
#ifdef TARGET_PORT /* CAI m_AnimRepeatedCount at its host offset (round 11 C26: 0xC7 read another member) */
    char pad0[__builtin_offsetof(CAI, m_AnimRepeatedCount)];
#else
    char pad0[0xC7];
#endif
    s8 unk_C7;
} ObjectStateC7;

void Destructible_GoDeathUpdate(void *arg0, void *arg1) {
    CGameObjectInstance *object = arg0;
    CDestructibleIntelligence *nested = &object->header.m_pIntelligence->m_Destructible;
    ObjectStateC7 *state = arg1;
    if (state->unk_C7 != 0 &&
        (nested->m_dwFlags & 0x20) != 0) {
        unsigned int flags = object->m_dwFlags;
        flags &= ~0x2000;
        flags &= ~0x100;
        object->m_dwFlags = flags;
    }
}

void Destructible_EventGo(sptr arg0, sptr arg1) {
    CAI__SetMode(arg0, arg1, 1);
    CScene__SetInstanceSavedBit(&gScene, arg0, 1);
}

#include "structs/event.h"

void Destructible_EventGoRemote(LevelEvent *arg0) {
    CScene__SetInstanceSavedBitInLevel(&gScene, arg0->level, arg0->actor, 1);
}

#include "structs/collision.h"

s32 Destructible_IsHitExplosive(DamageEvent *arg0) {
    return arg0->flags & 0x400000;
}

/* Only the word read here. */
typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ u32 flags;
} FlagsRecord;

/* Whether flag bit 22 is clear. */
s32 Destructible_IsHitNotExplosive(FlagsRecord *arg0) {
    return (((u32) arg0->flags >> 0x16) ^ 1) & 1;
}

void Destructible_Hurt(CGameObjectInstance *arg0, CAI *arg1, DamageEvent *arg2) {
    s32 temp_v1;
    s32 var_a2;
    s32 var_a3;
    s32 var_v1;

    temp_v1 = arg0->header.m_pIntelligence->m_dwTypeFlags;
    var_a3 = 0;
    if (temp_v1 & 1) {
        var_a3 = (((u32) arg2->flags >> 0x16) ^ 1) & 1;
    }
    if ((temp_v1 & 2) && (arg2->flags & 0x400000)) {
        var_a3 = 1;
    }
    if (var_a3 != 0) {
        var_v1 = arg1->m_Health - arg2->damage;
        if (var_v1 < 0) {
            var_v1 = 0;
        }
        arg1->m_Health = var_v1;
        if (var_v1 == 0) {
            var_a2 = 1;
            if (arg0->header.m_pIntelligence->m_dwTypeFlags & 0x40) {
                var_a2 = 2;
            }
            CAI__SetMode(arg0, arg1, var_a2);
        }
    }
}
