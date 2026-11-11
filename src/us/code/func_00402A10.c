#include "common.h"
#include "structs/boss.h"
#include "globals/boss.h"

typedef struct {
    f32 x, y, z;
} Vec3_00402A10;

typedef struct {
    /* 0x00 */ u8 pad0[0x8];
    /* 0x08 */ void (*onReset)(sptr a0, void *obj);
} Def00402A10;

typedef struct {
    /* 0x00 */ s32 flags;
    /* 0x04 */ u8 pad0[0x30 - 0x4];
    /* 0x30 */ Def00402A10 *def;
    /* 0x34 */ s8 kind;
    /* 0x35 */ u8 pad1[0x88 - 0x35];
    /* 0x88 */ s32 unk88;
    /* 0x8C */ u8 pad2[0x9C - 0x8C];
    /* 0x9C */ Vec3_00402A10 pos;
    /* 0xA8 */ s32 angle;
    /* 0xAC */ Vec3_00402A10 prevPos;
    /* 0xB8 */ s32 prevAngle;
} Obj00402A10;

extern s32 gMantidQueen_waveActive[]; /* local type: the field's real type (or the member path) changes the ROM's code here */
#ifdef TARGET_PORT /* ABI (docs/port/ABI.md, abicheck struct_scalar/float_int) */
#define AI_TurnHeadToTarget AI_TurnHeadToTarget_i
#endif
void AI_TurnHeadToTarget(sptr a0, Obj00402A10 *obj, s32 a2);
void MantidQueen_Update(void *state);

#ifdef TARGET_PORT
#include "structs/game_object.h"
/* Host body (round 9 x2): the CAI by member. The N64 view (Obj00402A10) is the CAI: the mode
 * row m_pModeCurrent at 0x30 (its update m_pfCallback at + 8), the mode at 0x34, m_pPathTarget at
 * 0x88, m_vInitialPos/m_pInitialRegion copied to m_vLeashPos/m_pLeashRegion. On the host it read
 * bytes of the mode table pointers, so the Queen's mode updates never ran: she stayed in mode 1
 * (the intro) for good, no wave was sent and no spot opened. */
void MantidQueen_Step(sptr a0, Obj00402A10 *obj) {
    CAI *ai = (CAI *) obj;

    ai->m_vLeashPos = ai->m_vInitialPos;
    ai->m_pLeashRegion = ai->m_pInitialRegion;
    if (gMantidQueen.waveActive != 0 || ai->m_Mode == 15) {
        ai->m_pPathTarget = NULL;
        ai->m_dwFlags |= 0x20000000;
    } else {
        ai->m_dwFlags &= ~0x20000000;
    }
    AI_TurnHeadToTarget(a0, obj, 0);
    if (ai->m_pModeCurrent != NULL && ai->m_pModeCurrent->m_pfCallback != NULL) {
        ai->m_pModeCurrent->m_pfCallback(a0, ai);
    }
    MantidQueen_Update(&gMantidQueen);
}
#else
void MantidQueen_Step(sptr a0, Obj00402A10 *obj) {
    obj->prevPos = obj->pos;
    obj->prevAngle = obj->angle;
    if (gMantidQueen_waveActive[0] != 0 || obj->kind == 15) {
        obj->unk88 = 0;
        obj->flags |= 0x20000000;
    } else {
        obj->flags &= ~0x20000000;
    }
    AI_TurnHeadToTarget(a0, obj, 0);
    if (obj->def != NULL && obj->def->onReset != NULL) {
        obj->def->onReset(a0, obj);
    }
    MantidQueen_Update(&gMantidQueen);
}
#endif
