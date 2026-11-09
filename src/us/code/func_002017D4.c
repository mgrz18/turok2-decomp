#include "common.h"
#include "protos/iset.h"

/* A resource block starts with a table of word offsets (entry 0 skipped): returns the address of section arg1. */
void *CISet__GetBlock(void *arg0, s32 arg1) {
    return (u8 *) arg0 + ((s32 *) ((u8 *) arg0 + (arg1 << 2)))[1];
}
