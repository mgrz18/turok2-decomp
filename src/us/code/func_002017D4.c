#include "common.h"

/* A resource block starts with a table of word offsets (entry 0 skipped): returns the address of section arg1. */
s32 CISet__GetBlock(s32 arg0, s32 arg1) {
    return arg0 + ((s32 *) (arg0 + (arg1 << 2)))[1];
}
