#ifndef GLOBALS_AI_H
#define GLOBALS_AI_H

#include "common.h"
#include "structs/vector.h"
#include "structs/pick.h"
#include "structs/anim_pick.h"
#include "structs/ai_behavior.h"

struct CAIModeInfo;

/* Enemy animation pick tables (AI_PickAnimFromTable), flags -1 terminated */
extern AIAnimPick sEnemyAnimWeapon_B00C0[6];
extern AIAnimPick sEnemyAnimWeapon_B00F0[6];
extern AIAnimPick sEnemyAnimNormalDeath_B0120[6];
extern AIAnimPick sEnemyAnimNormalDeath_B0150[6];
extern AIAnimPick sEnemyAnimNormalDeath_B0180[6];
extern AIAnimPick sEnemyAnimMovingDeath_B01B0[6];
extern AIAnimPick sEnemyAnimMovingDeath_B01E0[6];
extern AIAnimPick sEnemyAnimMovingDeath_B0210[6];
extern AIAnimPick sEnemyAnimViolentDeath_B0240[6];
extern AIAnimPick sEnemyAnimViolentDeath_B0270[6];
extern AIAnimPick sEnemyAnimViolentDeath_B02A0[6];
extern AIAnimPick sEnemyAnimExplosiveDeath_B02D0[6];
extern AIAnimPick sEnemyAnimExplosiveDeath_B0300[6];
extern AIAnimPick sEnemyAnimExplosiveDeath_B0330[6];
extern AIAnimPick sEnemyAnimNormalDeath_B0360[6];
extern AIAnimPick sEnemyAnimNormalDeath_B0390[6];
extern AIAnimPick sEnemyAnimNormalDeath_B03C0[6];
extern AIAnimPick sEnemyAnimEvade_B03F0[6];
extern AIAnimPick sEnemyAnimEvade_B0420[6];
extern AIAnimPick sEnemyAnimEvade_B0450[6];
extern AIAnimPick sEnemyAnimEvade_B0480[6];
extern AIAnimPick sEnemyAnimEvade_B04B0[6];
extern AIAnimPick sEnemyAnimEvade_B04E0[6];
extern AIAnimPick sEnemyAnimMove_B0510[6];
extern AIAnimPick sEnemyAnimMove_B0540[6];
extern AIAnimPick sEnemyAnimMove_B0570[6];
extern AIAnimPick sEnemyAnimMove_B05A0[6];
extern AIAnimPick sEnemyAnimMove_B05D0[6];
extern AIAnimPick sEnemyAnimMove_B0600[6];
extern AIAnimPick sJoypadTestAnimTable[14];   /* {button mask, anim} */

/* The point an alerted enemy turns to, and the last noise heard */
extern CVector3 gEnemyAlertPoint;
extern f32 gEnemyAlertPoint_y;   /* gEnemyAlertPoint.y */
extern f32 gEnemyAlertPoint_z;   /* gEnemyAlertPoint.z */
extern CVector3 gEnemyLastNoisePos;

/* AI mode tables: a 0x20-byte header row, then one CAIModeInfo per mode */
extern struct CAIModeInfo gEnemyGroundModeTable[33];
extern CEnemyBehaviorInfo *gEnemyBehaviorInfo[8];
extern CVector3 sProjectileMuzzleOffset;
extern struct CAIModeInfo gEnemyStationaryGroundModeTable[4];
extern struct CAIModeInfo gEnemyStationaryAirModeTable[4];
extern struct CAIModeInfo gEnemyStationaryUnderwaterModeTable[4];
extern struct CAIModeInfo gTurretModeTable[7];
extern struct CAIModeInfo gGeneratorModeTable[4];
extern struct CAIModeInfo gPlatformModeTable[6];
extern struct CAIModeInfo gThrowableModeTable[5];
extern struct CAIModeInfo gDoorModeTable[4];
extern struct CAIModeInfo gDestructibleModeTable[5];
extern struct CAIModeInfo gActionModeTable[5];
extern struct CAIModeInfo gInteractiveAnimModeTable[3];

/* Generator loot pick tables (Generator_SpawnEnter), id -1 terminated */
extern Pick sGeneratorAmmoPicks[22];
extern Pick sGeneratorMultiAmmoPicks[9];
extern Pick sGeneratorHealthPicks[4];
extern Pick sGeneratorHealthPicksReduced[3];
extern Pick sGeneratorMultiHealthPicks[4];
extern Pick sGeneratorItemPicks[23];
extern Pick sGeneratorMultiItemPicks[9];

/* Throwable_GetCarryPoint: the two owner joints whose midpoint is the carry point */
extern JointPoint sThrowableCarryJointA;
extern JointPoint sThrowableCarryJointB;

#endif
