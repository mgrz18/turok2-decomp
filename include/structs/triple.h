#ifndef STRUCTS_TRIPLE_H
#define STRUCTS_TRIPLE_H

/* The Rage Wars `Triple`: a position passed by value (ImpactEvent_*,
 * CInstancePool__Spawn, Generator_SpawnObject, ...). The files imported from
 * Rage Wars declare it as three s32 (`struct Triple { s32 x, y, z; }`, what
 * the N64 code reads); this header holds its host twin only.
 *
 * ABI (docs/port/ABI.md, abicheck struct_class): the position is a float
 * struct on the host. MIPS passes the Rage Wars Triple (three s32) and a
 * CVector3 in the same registers; x86-64 and arm64 do not (integer vs SSE/FP
 * registers), so Triple means three floats here. */
#ifdef TARGET_PORT
typedef struct { float x, y, z; } T2HostTriple;
#define Triple T2HostTriple
#endif

#endif
