#include "common.h"
#include "structs/table.h"
#include "protos/iset.h"

s32 CISet__GetROMAddress(sptr arg0, s32 arg1, s32 arg2) {
    return arg1 + ((OffsetTable *)arg0)->offsets[arg2];
}
