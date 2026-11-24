/* The second VM module's copy of src/us/vm1/BlindOne/func_0040EB30.c (byte for byte, at the same link address or 0xE0 below
 * it, docs/VMASM.md), with the second module's names. */
#include "common.h"
#include "structs/game.h"
#include "globals/game.h"
#include "globals/scene.h"
#include "protos/scene.h"

extern void CPlayerPool__SetInvincibility(void *players, f32 value);

void BlindOne_PhaseVictoryInit_2(void) {
    CPlayerPool__SetInvincibility(&gEngineApp_m_Game.m_PlayerList, 900.0f);
    CScene__UnmarkGeneratorsSaved(&gScene);
}
