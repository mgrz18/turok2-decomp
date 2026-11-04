/* WRONG CUT: splat named this D_002150B8 (0x2150B8, 500 bytes): its first 8 bytes are junk fill
 * ("ICKUP_SA" text), the real function starts at the 16-aligned 0x2150C0 (ROM symbol func_002150C0),
 * 492 bytes; func_00220D00 calls it for the level file's third table.
 * Sets up a placed level actor from its 0x20-byte record: position, bounds (position +- radius),
 * model, segment, angle (s8 in 1/254 turns), flags (always with bit 1), default 24-byte state. */

#include "common.h"
#include "structs/vector.h"

typedef struct {
    s32 data[6];
} ActorState;

/* A placed actor's record in the level data. */
typedef struct {
    /* 0x00 */ s32 value;
    /* 0x04 */ CVector3 pos;
    /* 0x10 */ f32 radius;
    /* 0x14 */ u16 id;
    /* 0x16 */ u16 segment; /* 0xFFFF = none */
    /* 0x18 */ u16 flags;
    /* 0x1A */ u16 unk1A;
    /* 0x1C */ u16 model;   /* 0xFFFF = none */
    /* 0x1E */ s8 angle;
} PlacedActorRecord;

typedef struct {
    /* 0x000 */ u8 state;
    /* 0x001 */ u8 pad1;
    /* 0x002 */ u16 id;
    /* 0x004 */ CVector3 pos;
    /* 0x010 */ void *segment;
    /* 0x014 */ void *model;
    /* 0x018 */ u8 pad18[0x1C];
    /* 0x034 */ s32 owner;
    /* 0x038 */ u8 pad38[0x10C];
    /* 0x144 */ f32 angle;
    /* 0x148 */ u8 pad148[4];
    /* 0x14C */ CVector3 min;
    /* 0x158 */ CVector3 max;
    /* 0x164 */ f32 unk164;
    /* 0x168 */ s32 unk168;
    /* 0x16C */ u16 flags;
    /* 0x16E */ u16 unk16E;
    /* 0x170 */ s32 unk170;
    /* 0x174 */ s32 unk174;
    /* 0x178 */ ActorState initial;
    /* 0x190 */ s32 value;
    /* 0x194 */ u8 colorFrame;
    /* 0x195 */ u8 colorFrame2;
} PlacedActor;

extern u8 D_800F1360[];         /* the empty model */
extern u8 D_800F7078[];         /* the model cache */
extern u8 D_800B6D1B;
extern ActorState D_800AB0D0;   /* default state */

void *func_00225E88(void *cache, s32 model);
f32 func_002671B4(PlacedActor *actor);

void func_002150C0(PlacedActor *actor, PlacedActorRecord *record, s32 owner, u8 *segments) {
    CVector3 top;

    actor->state = 3;
    actor->id = record->id;
    actor->value = record->value;
    actor->pos = record->pos;
    actor->angle = record->angle * 0.02473695f;
    actor->unk164 = 0.35f;
    actor->unk168 = 0;
    actor->min.x = record->pos.x - record->radius;
    actor->min.y = record->pos.y - record->radius;
    actor->min.z = record->pos.z - record->radius;
    actor->max.x = record->pos.x + record->radius;
    actor->max.y = record->pos.y + record->radius;
    actor->max.z = record->pos.z + record->radius;
    actor->owner = owner;
    if (record->model == 0xFFFF) {
        actor->model = D_800F1360;
    } else {
        actor->model = func_00225E88(D_800F7078, record->model);
    }
    if (record->segment == 0xFFFF) {
        actor->segment = NULL;
    } else {
        actor->segment = segments + record->segment * 32;
    }
    actor->flags = record->flags | 2;
    actor->unk170 = 0;
    actor->unk174 = 0;
    actor->unk16E = record->unk1A;
    top.x = actor->pos.x;
    top.y = actor->pos.y + func_002671B4(actor) * 0.5f;
    top.z = actor->pos.z;
    actor->initial = D_800AB0D0;
    actor->colorFrame = actor->colorFrame2 = D_800B6D1B - 1;
}
