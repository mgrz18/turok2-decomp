/* CISet / CUSet, offset-table containers.
 *
 * Canonical prototypes, generated from the definitions by tools/port/genprotos.py
 * (do not edit; re-run it). Types the header cannot name without a struct header
 * are struct tags or void *. */

#ifndef PROTOS_ISET_H
#define PROTOS_ISET_H

#include "common.h"

struct ArrayTable;

void *CISet__GetBlock(void *, s32);
s32 CISet__GetBlockAndSize(s32 *, s32, s32 *);
s32 CISet__GetBlockSize(s32 *, s32);
s32 CISet__GetDataSize(s32 *);
s32 CISet__GetOffsetAndSize(s32 *, s32, s32 *);
s32 CISet__GetROMAddress(s32, s32, s32);
s32 CISet__GetROMAddressAndSize(s32 *, s32, s32, s32 *);
void CISet__GetSetAndSize(s32 *, s32 **, s32 *);
void *CUSet__GetArrayAndSize(struct ArrayTable *, s32 *);
void CUSet__GetSetAndSize(struct ArrayTable *, void **, s32 *);
s32 CUSet__GetTotalSize(void *);
void CUSet__Transpose(void *, void *, s32);

#endif
