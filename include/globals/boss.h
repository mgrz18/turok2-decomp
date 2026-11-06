#ifndef GLOBALS_BOSS_H
#define GLOBALS_BOSS_H

/* The boss data of the first VM module (scripts, tables and spawn points of
 * the Mantid Queen, the Primagen, the Mother and the Blind One), and the
 * module-2 copies (`_2`: the same bytes, used by the module-2 code).
 * Names and evidence: versions/names/data_g7.us.txt. The mode tables
 * (CAIModeInfo) are in globals/boss_ai.h. */

#include "structs/boss.h"

/* Mantid Queen (WaveBossState gMantidQueen) */
extern WaveBossMinion sMantidQueenMinions[2];   /* ids 0x1D4D, 0x1D4E */
extern WaveBossStep sMantidQueenScript[];
extern f32 sMantidQueenSpotHitScript[];          /* ScriptAnimator scripts */
extern f32 sMantidQueenMode6SpotScript[];
extern f32 sMantidQueenSpotOpenScript[];

/* Primagen (LevelScriptState gLevelScript) */
extern BossAttack sPrimagenAttacks[];
extern BossStep sPrimagenAttackScriptStage1[];   /* script A, kind 1 */
extern BossStep sPrimagenAttackScript2Stage1[];  /* script A, kind 2 */
extern BossStep sPrimagenAttackScriptStage2[];
extern BossStep sPrimagenAttackScript2Stage2[];
extern BossStep sPrimagenAttackScriptStage3[];
extern BossStep sPrimagenAttackScript2Stage3[];
extern BossStep2 sPrimagenSpawnScript1[];        /* script B */
extern BossStep2 sPrimagenSpawnScript2[];
extern BossStep2 sPrimagenSpawnScript3[];
extern BossStep2 sPrimagenSpawnScript4[];
extern BossStep2 sPrimagenSpawnScript5[];
extern BossStep3 sPrimagenSoundScript2[];        /* script C */
extern BossStep3 sPrimagenSoundScript1[];
extern BossStep3 sPrimagenSoundScript3[];
extern BossStep3 sPrimagenSoundScript4[];
extern f32 sPrimagenPartHitScript[];             /* ScriptAnimator scripts */
extern f32 sPrimagenPartDestroyScript[];
extern f32 sPrimagenMode7PartScript[];
extern s16 sPrimagenTossTable[];                 /* {type, weight} pairs, -1 ends (WeightedTable_PickStatic) */
extern s16 sPrimagenTossTableStage3[];
extern s16 sPrimagenTossTable2[];
extern s16 sPrimagenTossTable2Stage3[];

/* Mother (MotherBossState gMotherBoss) */
extern SpawnPoint gMotherSpawnPoints[4];
extern CVector3 sMotherBossPhase1PlayerPos[2];   /* the first is where the player is put */
extern s16 sMotherBossPickupTable[];             /* {type, weight} pairs, -1 ends */
extern s16 sMotherBossDropTablePhase1[];
extern s16 sMotherBossDropTablePhase2[];
extern s16 sMotherBossPhase1SpawnDrops[];
extern s32 sMotherBossPhaseVisBits[3];           /* per phase */
extern s32 sMotherBossPhaseActiveMasks[3];
extern s32 sMotherBossPhaseLiveMasks[3];
extern f32 sMotherBossSpotResetScript[];         /* ScriptAnimator scripts */
extern f32 sMotherBossHurtScript[];
extern f32 sMotherBossSpotFlashScript[];

/* Blind One (BlindOneState gBlindOne) */
extern BlindOneStep sBlindOneScript[];
extern BlindOnePhase sBlindOnePhases[];
extern f32 sBlindOneHitScript[];                 /* ScriptAnimator script */
extern u32 sBlindOneMoverSeq1[];                 /* actor bit masks (BlindOne_UpdateMover) */
extern u32 sBlindOneMoverSeq2[];
extern u32 sBlindOneMoverSeq3[];

/* Module-2 copies. */
extern BossAttack sPrimagenAttacks_2[];
extern BossStep sPrimagenAttackScriptStage1_2[];
extern BossStep sPrimagenAttackScript2Stage1_2[];
extern BossStep sPrimagenAttackScriptStage2_2[];
extern BossStep sPrimagenAttackScript2Stage2_2[];
extern BossStep sPrimagenAttackScriptStage3_2[];
extern BossStep sPrimagenAttackScript2Stage3_2[];
extern BossStep2 sPrimagenSpawnScript1_2[];
extern BossStep2 sPrimagenSpawnScript2_2[];
extern BossStep2 sPrimagenSpawnScript3_2[];
extern BossStep2 sPrimagenSpawnScript4_2[];
extern BossStep2 sPrimagenSpawnScript5_2[];
extern BossStep3 sPrimagenSoundScript2_2[];
extern BossStep3 sPrimagenSoundScript1_2[];
extern BossStep3 sPrimagenSoundScript3_2[];
extern BossStep3 sPrimagenSoundScript4_2[];
extern f32 sPrimagenMode7PartScript_2[];
extern s16 sPrimagenTossTable_2[];
extern s16 sPrimagenTossTableStage3_2[];
extern s16 sPrimagenTossTable2_2[];
extern s16 sPrimagenTossTable2Stage3_2[];
extern SpawnPoint gMotherSpawnPoints_2[4];
extern CVector3 sMotherBossPhase1PlayerPos_2[2];
extern s16 sMotherBossPickupTable_2[];
extern s16 sMotherBossDropTablePhase1_2[];
extern s16 sMotherBossDropTablePhase2_2[];       /* at module 1's sPrimagenTossTableStage3 (link address 0x00431CF0) */
extern s16 sMotherBossPhase1SpawnDrops_2[];
extern s32 sMotherBossPhaseVisBits_2[3];
extern s32 sMotherBossPhaseActiveMasks_2[3];
extern s32 sMotherBossPhaseLiveMasks_2[3];
extern f32 sMotherBossSpotResetScript_2[];
extern f32 sMotherBossHurtScript_2[];
extern f32 sMotherBossSpotFlashScript_2[];
extern BlindOneStep sBlindOneScript_2[];
extern BlindOnePhase sBlindOnePhases_2[];
extern f32 sBlindOneHitScript_2[];
extern u32 sBlindOneMoverSeq1_2[];
extern u32 sBlindOneMoverSeq2_2[];
extern u32 sBlindOneMoverSeq3_2[];

#endif
