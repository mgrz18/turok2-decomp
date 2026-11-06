#include "common.h"
#include "m2c_macros.h"

extern s32 sOpaqueSortRoot;
extern s32 sOpaqueSortFirst;
extern s32 sTranslucentSortRoot;
extern s32 sTranslucentSortFirst;

void func_0020BFC0(void) {
    sOpaqueSortFirst = 0;
    sOpaqueSortRoot = 0;
    sTranslucentSortFirst = 0;
    sTranslucentSortRoot = 0;
}
