#ifndef GLOBALS_AI_H
#define GLOBALS_AI_H

#include "common.h"
#include "structs/vector.h"
#include "structs/pick.h"
#include "structs/ai_behavior.h"

struct CAIModeInfo;

/* The point an alerted enemy turns to, and the last noise heard */
extern CVector3 gEnemyAlertPoint;
extern f32 gEnemyAlertPoint_y;   /* gEnemyAlertPoint.y */
extern f32 gEnemyAlertPoint_z;   /* gEnemyAlertPoint.z */
extern CVector3 gEnemyLastNoisePos;

/* AI mode tables: a 0x20-byte header row, then one CAIModeInfo per mode */
extern struct CAIModeInfo gEnemyGroundModeTable[33];
/* the shared mode table every AI table above chains to through its header row (m_Mode): the
 * common modes (26 rows after the header); its address is only stored in those header words */
extern struct CAIModeInfo D_800B0700[27];
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
