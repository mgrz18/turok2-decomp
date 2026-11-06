#ifndef STRUCTS_AI_BEHAVIOR_H
#define STRUCTS_AI_BEHAVIOR_H

#include "common.h"

struct CAIModeInfo;

/* An enemy behaviour (ROM debug type s_CEnemyBehaviorInfo): gEnemyBehaviorInfo
 * holds one per behaviour index. EnemyAI_Update calls m_pfSetup/m_pfUpdate and
 * installs one of the three mode tables by where the AI is (ground, air,
 * underwater). */
typedef struct CEnemyBehaviorInfo {
    /* 0x00 */ void (*m_pfSetup)();
    /* 0x04 */ void (*m_pfUpdate)();
    /* 0x08 */ struct CAIModeInfo *m_pGroundModeTable;
    /* 0x0C */ struct CAIModeInfo *m_pAirModeTable;
    /* 0x10 */ struct CAIModeInfo *m_pUnderwaterModeTable;
} CEnemyBehaviorInfo; /* size 0x14 */

/* A point on a model joint: joint index and an offset in its space. */
typedef struct JointPoint {
    /* 0x0 */ s32 joint;
    /* 0x4 */ f32 offset[3];
} JointPoint; /* size 0x10 */

#endif
