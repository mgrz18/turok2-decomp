#ifndef STRUCTS_ENEMY_ANIM_H
#define STRUCTS_ENEMY_ANIM_H

#include "common.h"

/* A row of an enemy animation pick table (sEnemyAnim<Kind>_<addr>): AI_PickAnimFromTable
 * adds m_Anim when the intelligence's anim flags have a bit of m_Flags; 0
 * stops once something was found, -1 ends the table; the first row's anim is
 * the fallback. */
typedef struct CEnemyAnimPick {
    /* 0x0 */ s32 m_Flags;
    /* 0x4 */ s32 m_Anim;
} CEnemyAnimPick; /* size 0x8 */

#endif
