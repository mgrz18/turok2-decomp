/* FAKEMATCH: the shared -1 is pinned to $v1 (register asm, see the body): local-alloc gives it $v0, which keeps sched2 from hoisting the load of actor->flags above the stores. */
/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/AI/NAV/80212C90_18.CPP, ACTOR_SETUP, with Turok 2's layouts. */

#include "common.h"

typedef struct {
    f32 x, y, z;
} SetupVec;

typedef struct {
    /* 0x0 */ u32 type;
    /* 0x4 */ u32 flags;
} SetupDesc;

/* The object the AI belongs to (CGameObjectInstance). */
typedef struct SetupSource {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ SetupVec pos;
    /* 0x10 */ void *region;
    /* 0x14 */ SetupDesc *desc;
    /* 0x18 */ u8 pad18[0x38];
    /* 0x50 */ f32 scale;
    /* 0x54 */ u8 pad54[0x80];
    /* 0xD4 */ u32 flags;
} SetupSource;

/* The object's AI (CAI, 0x13C bytes). */
typedef struct SetupActor {
    /* 0x000 */ u32 flags;
    /* 0x004 */ u8 pad04[0xE];
    /* 0x012 */ u8 field12;
    /* 0x013 */ u8 field13;
    /* 0x014 */ s8 sentinel14[4];
    /* 0x018 */ f32 field18;
    /* 0x01C */ s32 words1C[4];
    /* 0x02C */ u8 pad2C[4];
    /* 0x030 */ s32 field30;
    /* 0x034 */ s8 field34;
    /* 0x035 */ s8 field35;
    /* 0x036 */ u8 pad36[0xE];
    /* 0x044 */ s32 field44;
    /* 0x048 */ SetupVec pos48;
    /* 0x054 */ SetupVec pos54;
    /* 0x060 */ f32 field60;
    /* 0x064 */ s32 field64;
    /* 0x068 */ s32 field68;
    /* 0x06C */ s32 words6C[3];
    /* 0x078 */ s32 words78[5];
    /* 0x08C */ f32 field8C;
    /* 0x090 */ f32 field90;
    /* 0x094 */ u8 field94;
    /* 0x095 */ u8 field95;
    /* 0x096 */ s16 field96;
    /* 0x098 */ f32 field98;
    /* 0x09C */ SetupVec pos9C;
    /* 0x0A8 */ void *regionA8;
    /* 0x0AC */ SetupVec posAC;
    /* 0x0B8 */ void *regionB8;
    /* 0x0BC */ f32 fieldBC;
    /* 0x0C0 */ f32 fieldC0;
    /* 0x0C4 */ s16 fieldC4;
    /* 0x0C6 */ s8 fieldC6;
    /* 0x0C7 */ u8 fieldC7;
    /* 0x0C8 */ u8 padC8[4];
    /* 0x0CC */ s32 fieldCC;
    /* 0x0D0 */ s32 fieldD0;
    /* 0x0D4 */ u8 padD4[0x18];
    /* 0x0EC */ s32 fieldEC;
    /* 0x0F0 */ u8 padF0[4];
    /* 0x0F4 */ void *callbackF4;
    /* 0x0F8 */ u8 padF8[0xC];
    /* 0x104 */ void *callback104;
    /* 0x108 */ u8 pad108[0x34];
} SetupActor;

typedef struct SetupCallbacks {
    void (*first)(SetupSource *, SetupActor *);
} SetupCallbacks;

void memset(void *dst, s32 value, s32 size); /* memset */
void AI_InitHealth(SetupActor *ai, SetupDesc *desc, SetupVec pos, SetupSource *source);
f32 CGameRegion__GetHeight(void *region, f32 x, f32 z);  /* floor height at */
s32 CGameObjectInstance__DrawModel();
void CGameObjectInstance__CalculateOrientationMatrix();

/* Builds an object's AI: clears it (keeping the field at 0x68), runs the
 * shared initialiser, installs the two callbacks, seeds the position and
 * scale copies, the sentinel indices and the default factors, runs the
 * optional first callback, and flags it when the object asks (bit 27). */
void CAI__Setup(SetupSource *source, SetupActor *actor, SetupCallbacks *callbacks) {
    s32 saved;
    f32 default_;
    void *region;
    /* The shared -1 is pinned to $v1: GCC's local allocator always gives
     * it $v0 (it outranks the flags temporary), which stops sched2 from
     * hoisting the load of actor->flags above these stores. */
    register s16 none asm("$3");

    saved = actor->field68;
    memset(actor, 0, 0x13C);
    actor->flags = 0x40;
    actor->field68 = saved;
    AI_InitHealth(actor, source->desc, source->pos, source);
    actor->callback104 = CGameObjectInstance__CalculateOrientationMatrix;
    actor->callbackF4 = CGameObjectInstance__DrawModel;
    actor->field34 = -1;
    actor->field35 = -1;
    actor->field30 = 0;
    actor->pos48 = source->pos;
    actor->pos54 = source->pos;
    actor->words6C[0] = 0;
    actor->words6C[1] = 0;
    actor->words6C[2] = 0;
    actor->field60 = source->scale;
    actor->field64 = 0;
    actor->words78[0] = 0;
    actor->words78[1] = 0;
    actor->words78[2] = 0;
    actor->words78[3] = 0;
    actor->words78[4] = 0;
    actor->field8C = source->scale;
    actor->field90 = source->pos.y;
    actor->field98 = source->scale;
    actor->pos9C = source->pos;
    actor->regionA8 = source->region;
    actor->posAC = source->pos;
    region = source->region;
    actor->regionB8 = region;
    if ((source->desc->flags & 4) && region != NULL) {
        actor->posAC.y = CGameRegion__GetHeight(region, actor->posAC.x, actor->posAC.z);
    }
    default_ = 1.0f;
    actor->fieldC6 = -1;
    actor->field13 = 0x14;
    actor->field95 = 0xFF;
    none = -1;
    actor->fieldC4 = none;
    actor->field35 = none;
    actor->sentinel14[0] = none;
    actor->sentinel14[1] = none;
    actor->sentinel14[2] = none;
    actor->sentinel14[3] = none;
    actor->field96 = none;
    actor->fieldC7 = 0;
    actor->fieldCC = 0;
    actor->fieldD0 = 0;
    actor->field44 = 0;
    actor->field12 = 0;
    actor->words1C[0] = 0;
    actor->words1C[1] = 0;
    actor->words1C[2] = 0;
    actor->words1C[3] = 0;
    actor->field94 = 0;
    actor->fieldEC = 0;
    actor->fieldBC = default_;
    actor->fieldC0 = default_;
    actor->flags &= ~1;
    actor->field18 = default_;
    if (callbacks != NULL && callbacks->first != NULL) {
        callbacks->first(source, actor);
    }
    if (source->flags & 0x8000000) {
        actor->fieldC7 = 1;
    }
}
