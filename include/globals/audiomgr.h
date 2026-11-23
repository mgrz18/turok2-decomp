#ifndef GLOBALS_AUDIOMGR_H
#define GLOBALS_AUDIOMGR_H

/* The SDK audio manager's state (structs/audiomgr.h; versions/names/data_g5.us.txt).
 * Separate from globals/audio.h: files that include that one keep views of some of these. */

#include "structs/audiomgr.h"

extern AMDMABuffer gAudioDmaBuffers[0x3C];      /* SDK dmaBuffs */
extern AMDMAState gAudioDmaState;               /* SDK dmaState */
extern AMDMABuffer *gAudioDmaState_firstUsed;   /* .firstUsed (__amDMA bases its loads here) */
extern AMDMABuffer *gAudioDmaState_firstFree;   /* .firstFree */
extern AMCacheEntry *gAudioDmaCache;            /* Turok's DMA cache list */
extern Acmd *gAudioCmdLists[2];                 /* SDK __am.ACMDList */
extern AMAudioInfo *gAudioInfo[3];              /* SDK __am.audioInfo */

#endif
