#include "common.h"
#include "m2c_macros.h"

extern s32 sOpaqueSortRoot;
extern s32 sOpaqueSortFirst;
extern s32 sDrawItemCount;
extern s32 sBatchResource;
extern s32 sSortNodeCount;
extern s32 sTranslucentSortRoot;
extern s32 sBatchInstanceCount;
extern s32 sTranslucentSortFirst;
extern s32 sBatchResourceCount;

void Render_ResetBatches(void) {
    sBatchResource = 0;
    sBatchInstanceCount = 0;
    sSortNodeCount = 0;
    sDrawItemCount = 0;
    sBatchResourceCount = 0;
    sOpaqueSortFirst = 0;
    sOpaqueSortRoot = 0;
    sTranslucentSortFirst = 0;
    sTranslucentSortRoot = 0;
}
