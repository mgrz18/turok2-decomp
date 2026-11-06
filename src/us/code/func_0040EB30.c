#include "common.h"
#include "structs/game.h"

extern u8 gScene[];
extern void CPlayerPool__SetInvincibility(void *players, f32 value);
extern void CScene__UnmarkGeneratorsSaved(void *);

void BlindOne_PhaseVictoryInit(void) {
    CPlayerPool__SetInvincibility(&D_80119870.m_PlayerList, 900.0f);
    CScene__UnmarkGeneratorsSaved(gScene);
}
