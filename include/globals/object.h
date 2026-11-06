#ifndef GLOBALS_OBJECT_H
#define GLOBALS_OBJECT_H

/* Game object globals: the intelligence types' blocks and the animation statics. */

#include "common.h"
#include "structs/ai_type.h"
#include "structs/vector.h"

extern CIntelligenceTypeInfo *gIntelligenceTypeInfo[13];   /* by CIntelligence m_Type */
extern s32 gAnimEventCount;         /* events fired by CGameObjectInstance__FireAnimEvents (debug count) */
extern f32 gSwingAmplitudes[25];    /* swing amplitude per remaining step */
extern struct SineTable sWobbleSineTable;   /* the time-based wobble (rw/abu_types.h SineTable) */

/* The animation decoder's statics: frame and track counts, the remap range */
extern s32 sAnimFrameCount;
extern s32 sAnimTrackCount;
extern f32 sAnimRemapMin;
extern f32 sAnimRemapMax;

/* The skeleton poser's scratch, shared by every level of its recursion (one bone per call) */
extern struct BoneList *sPoseChildren;   /* the bone's children */
extern f32 sPoseMtx[4][4];                /* its local matrix */
extern CVector3 sPosePos;                 /* its translation */
extern CVector3 sPosePosA;                /* ... from channel A */
extern CVector3 sPosePosB;                /* ... from channel B */
extern CQuaternion sPoseRot;              /* its rotation */
extern CQuaternion sPoseRotA;             /* ... from channel A */
extern CQuaternion sPoseRotB;             /* ... from channel B */
extern s32 sPoseBoneType;                /* the object's type */
extern u8 *sPoseBoneData;                /* the bone data */
extern u8 sPoseFlags;                    /* the bone's flags */
extern struct CGameRegion *sPoseRegion;
extern f32 sPoseCeilingHeight;          /* the ground height */
extern f32 sPoseCollisionHeight;        /* the object's height */
extern f32 sPoseCeilingOverlap;         /* the depth below the ground */

#endif
