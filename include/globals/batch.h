#ifndef GLOBALS_BATCH_H
#define GLOBALS_BATCH_H

/* The render batcher's state (structs/batch.h). */

#include "structs/batch.h"

extern void **sBatchResource;           /* the batched model */
extern s32 sBatchLod;                   /* its level of detail */
extern s32 sBatchPass;                  /* its pass */
extern s32 sBatchInstanceCount;         /* queued instances (up to 0x20) */
extern s32 sBatchResourceCount;         /* registered models */
extern void **sBatchResources[0x100];   /* the registered models, unlocked by Render_FlushBatches */
extern DrawItem sDrawItems[500];
extern s32 sDrawItemCount;              /* draw items used */
extern SortNode sSortNodes[100];
extern s32 sSortNodeCount;              /* sort nodes used */
extern SortNode *sOpaqueSortRoot;       /* the opaque tree's root */
extern SortNode *sOpaqueSortFirst;      /* its first node in key order */
extern SortNode *sTranslucentSortRoot;  /* the translucent tree's root */
extern SortNode *sTranslucentSortFirst; /* its first node in key order */

#endif
