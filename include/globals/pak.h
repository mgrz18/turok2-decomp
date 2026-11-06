#ifndef GLOBALS_PAK_H
#define GLOBALS_PAK_H

/* The controller pak menus, the save buffers and the pak driver's state. */

#include "common.h"
#include "ultra_types.h"
#include "structs/cache.h"

extern CMemEntry *gSaveBufferHandle; /* the CCache handle of gSaveBuffer (structs/save.h) */
extern CMemEntry *gLoadBufferHandle; /* the CCache handle of gLoadBuffer */
extern s32 gDamagedPakContinued;    /* Continue picked on the damaged-pak dialog */
extern s32 gPakMenuMode;            /* which notes the pak menu lists */
extern s32 gPakMenuPort;            /* the controller port the pak menus work on */
extern s32 sPakMenuStartLatched;    /* START seen once */
extern s32 gPakDirectoryAllocated;
extern u8 gPakCharTable[];          /* the controller pak character set */
extern s8 gPakQueueState;           /* the pak rescan request / queue state */
extern OSMesgQueue gPakAccessQueue; /* the pak access lock */
extern OSMesg gPakMesgBuf[1];       /* gPakAccessQueue's buffer */
extern u8 gPakWatched[4];
extern s32 gPakInserted[4];
extern s32 gPakChanged[4];
extern s32 gPakRemoved[4];

#endif
