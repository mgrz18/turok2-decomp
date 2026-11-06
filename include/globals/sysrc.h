#ifndef GLOBALS_SYSRC_H
#define GLOBALS_SYSRC_H

/* The system resources InitializeSystemResources builds at boot (Turok 3's
 * sysrc.h: "sysrc_heap", "sysrc_loader", "sysrc_decompressor"),
 * 0x800D1CC0-0x800D81B7. */

#include "common.h"
#include "structs/heap.h"
#include "structs/loader.h"
#include "structs/decompressor.h"

extern CHeap gHeap;
extern CLoader gLoader;
extern CDecompressor gDecompressor;

#endif
