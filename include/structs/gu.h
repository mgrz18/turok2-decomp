#ifndef STRUCTS_GU_H
#define STRUCTS_GU_H

/* The integer matrix as libultra's gu functions write it (ultralib's PR/mbi.h Mtx: the 16
 * integer halves, then the 16 fraction halves, as guMtxF2L packs them). The engine's
 * structs/gbi.h keeps Mtx as 0x40 opaque bytes; only the imported gu files (lookat,
 * perspective, the port's ortho) index it, and none of them includes structs/gbi.h. */
typedef struct { long m[4][4]; } Mtx;

#endif
