#ifndef GLOBALS_ENEMY_H
#define GLOBALS_ENEMY_H

/* The enemy animation pick tables, by AI mode (the EnemyAnim_<Kind>_<addr> callbacks
 * and EnemyAnim_InteractiveIdle pass them to AI_PickAnimFromTable). */

#include "common.h"
#include "structs/enemy_anim.h"

extern CEnemyAnimPick sEnemyAnimIdle_AF6F0[6];
extern CEnemyAnimPick sEnemyAnimIdle_AF720[6];
extern CEnemyAnimPick sEnemyAnimIdle_AF750[6];
extern CEnemyAnimPick sEnemyAnimIdle_AF780[6];
extern CEnemyAnimPick sEnemyAnimIdle_AF7B0[6];
extern CEnemyAnimPick sEnemyAnimIdle_AF7E0[6];
extern CEnemyAnimPick sEnemyAnimAlert_AF810[6];
extern CEnemyAnimPick sEnemyAnimAlert_AF840[6];
extern CEnemyAnimPick sEnemyAnimAlert_AF870[6];
extern CEnemyAnimPick sEnemyAnimTakeCover_AF8A0[6];
extern CEnemyAnimPick sEnemyAnimTakeCover_AF8D0[6];
extern CEnemyAnimPick sEnemyAnimTakeCover_AF900[6];
extern CEnemyAnimPick sEnemyAnimLeftArmWound_AF930[6];
extern CEnemyAnimPick sEnemyAnimLeftArmWound_AF960[6];
extern CEnemyAnimPick sEnemyAnimLeftArmWound_AF990[6];
extern CEnemyAnimPick sEnemyAnimRightArmWound_AF9C0[6];
extern CEnemyAnimPick sEnemyAnimRightArmWound_AF9F0[6];
extern CEnemyAnimPick sEnemyAnimRightArmWound_AFA20[6];
extern CEnemyAnimPick sEnemyAnimPatrol_AFA50[6];
extern CEnemyAnimPick sEnemyAnimPatrol_AFA80[6];
extern CEnemyAnimPick sEnemyAnimPatrol_AFAB0[6];
extern CEnemyAnimPick sEnemyAnimMove_AFAE0[17];
extern CEnemyAnimPick sEnemyAnimMove_AFB68[17];
extern CEnemyAnimPick sEnemyAnimMove_AFBF0[17];
extern CEnemyAnimPick sEnemyAnimMove_AFC78[6];
extern CEnemyAnimPick sEnemyAnimMove_AFCA8[6];
extern CEnemyAnimPick sEnemyAnimMove_AFCD8[6];
extern CEnemyAnimPick sEnemyAnimFlee_AFD08[6];
extern CEnemyAnimPick sEnemyAnimFlee_AFD38[6];
extern CEnemyAnimPick sEnemyAnimFlee_AFD68[6];
extern CEnemyAnimPick sEnemyAnimComboEnd_AFD98[6];
extern CEnemyAnimPick sEnemyAnimComboEnd_AFDC8[6];
extern CEnemyAnimPick sEnemyAnimComboEnd_AFDF8[6];
extern CEnemyAnimPick sEnemyAnimCombat_AFE28[6];
extern CEnemyAnimPick sEnemyAnimCombat_AFE58[6];
extern CEnemyAnimPick sEnemyAnimCombat_AFE88[6];
extern CEnemyAnimPick sEnemyAnimLeap_AFEB8[6];
extern CEnemyAnimPick sEnemyAnimLeap_AFEE8[6];
extern CEnemyAnimPick sEnemyAnimLeap_AFF18[6];
extern CEnemyAnimPick sEnemyAnimDart_AFF48[6];
extern CEnemyAnimPick sEnemyAnimDart_AFF78[12];
extern CEnemyAnimPick sEnemyAnimProjectile_AFFD8[6];
extern CEnemyAnimPick sEnemyAnimProjectile_B0008[12];
extern CEnemyAnimPick sEnemyAnimWeapon_B0068[11];

#endif
