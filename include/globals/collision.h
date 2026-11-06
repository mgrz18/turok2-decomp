#ifndef GLOBALS_COLLISION_H
#define GLOBALS_COLLISION_H

/* The engine objects' collision setups (Turok 3's colinfo.c name), at the
 * head of the player code's file in the first VM module, and its module-2
 * copy (written by hand in asm/data_0046642C.s). The last record ends the
 * table (NULL m_pInfo). */

#include "structs/collision.h"

extern CCollisionSetup gCollisionSetupTable[34];
extern CCollisionSetup gCollisionSetupTable_2[34];

#endif
