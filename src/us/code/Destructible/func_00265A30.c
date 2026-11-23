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

extern void Destructible_Step(CGameObjectInstance *actor, void *state);
extern void Destructible_Hurt(CGameObjectInstance *actor, void *state);
extern void TotemDefense_Init();

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
