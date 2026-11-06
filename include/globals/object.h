#ifndef GLOBALS_OBJECT_H
#define GLOBALS_OBJECT_H

/* Game object globals: the intelligence types' blocks and the animation statics. */

#include "common.h"
#include "structs/ai_type.h"

extern CIntelligenceTypeInfo *gIntelligenceTypeInfo[13];   /* by CIntelligence m_Type */
extern s32 gAnimEventCount;         /* events fired by CGameObjectInstance__FireAnimEvents (debug count) */
extern f32 gSwingAmplitudes[25];    /* swing amplitude per remaining step */
extern struct SineTable sWobbleSineTable;   /* the time-based wobble (rw/abu_types.h SineTable) */

#endif
