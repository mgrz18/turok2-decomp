#include "common.h"
#include "m2c_macros.h"
#include "globals/batch.h"

void Render_ClearSortTrees(void) {
    sOpaqueSortFirst = NULL;
    sOpaqueSortRoot = NULL;
    sTranslucentSortFirst = NULL;
    sTranslucentSortRoot = NULL;
}
