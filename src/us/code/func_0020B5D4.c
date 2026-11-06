#include "common.h"
#include "m2c_macros.h"
#include "globals/batch.h"


void Render_ResetBatches(void) {
    sBatchResource = NULL;
    sBatchInstanceCount = 0;
    sSortNodeCount = 0;
    sDrawItemCount = 0;
    sBatchResourceCount = 0;
    sOpaqueSortFirst = NULL;
    sOpaqueSortRoot = NULL;
    sTranslucentSortFirst = NULL;
    sTranslucentSortRoot = NULL;
}
