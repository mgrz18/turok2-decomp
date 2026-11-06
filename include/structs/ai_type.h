#ifndef STRUCTS_AI_TYPE_H
#define STRUCTS_AI_TYPE_H

/* An intelligence type's block (gIntelligenceTypeInfo[m_Type], or
 * gWeaponActorTypeInfo for a player's second object): the setup the
 * construction calls, then the type's handlers (event tables follow). */

#include "common.h"

typedef struct CIntelligenceTypeInfo {
    /* 0x00 */ void (*setup)(void *obj, void *ai);      /* EnemyAI_Init, WeaponActor_InitAI... */
    /* 0x04 */ void (*init)(void *obj, void *ai);       /* EnemyAI_InitDispatch... (func_0021E5D4) */
} CIntelligenceTypeInfo;

#endif
