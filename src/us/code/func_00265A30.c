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


#ifdef TARGET_PORT
typedef struct ActionState {
/* the view at the host offsets of CAI (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_ActionState(m) __builtin_offsetof(CAI, m)
    union {
        struct { u8 _hp_table[T2_HOFF_ActionState(m_pModeTable)]; void *table; }; /* N64 0x2C */
        struct { u8 _hp_anim[T2_HOFF_ActionState(m_AnimIndex)]; u8 anim; }; /* N64 0xC6 */
        struct { u8 _hp_active[T2_HOFF_ActionState(m_AnimRepeatedCount)]; s8 active; }; /* N64 0xC7 */
        struct { u8 _hp_update[T2_HOFF_ActionState(m_pfStep)]; void (*update)(CGameObjectInstance *, void *); }; /* N64 0xF0 */
        struct { u8 _hp_onEvent[T2_HOFF_ActionState(m_pfHurt)]; void (*onEvent)(CGameObjectInstance *, void *); }; /* N64 0xF8 */
    };
} ActionState;
#else
typedef struct {
    /* 0x00 */ u8 pad0[0x2C];
    /* 0x2C */ void *table;
    /* 0x30 */ u8 pad30[0x96];
    /* 0xC6 */ u8 anim;
    /* 0xC7 */ s8 active;
    /* 0xC8 */ u8 padC8[0x28];
    /* 0xF0 */ void (*update)(CGameObjectInstance *, void *);
    /* 0xF4 */ u8 padF4[4];
    /* 0xF8 */ void (*onEvent)(CGameObjectInstance *, void *);
} ActionState;
#endif

extern void Destructible_Step(CGameObjectInstance *actor, void *state);
extern void Destructible_Hurt(CGameObjectInstance *actor, void *state);
#ifdef TARGET_PORT /* unprototyped: the host passes the pass-through arguments */
extern void TotemDefense_Init();
#else
extern void TotemDefense_Init(void);
#endif

#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck missing_args): on MIPS TotemDefense_Init receives Destructible_Setup's own argument
 * registers (pass-through); the host passes them. */
void Destructible_Setup(CGameObjectInstance *actor, ActionState *state) {
    state->table = gDestructibleModeTable;
    state->update = Destructible_Step;
    state->onEvent = Destructible_Hurt;
    if (actor->m_nTypeFlag == 0x64D) {
        TotemDefense_Init(actor, state);
    }
}
#else
void Destructible_Setup(CGameObjectInstance *actor, ActionState *state) {
    state->table = gDestructibleModeTable;
    state->update = Destructible_Step;
    state->onEvent = Destructible_Hurt;
    if (actor->m_nTypeFlag == 0x64D) {
        TotemDefense_Init();
    }
}
#endif
