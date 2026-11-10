/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80267198_de.c, func_8026740C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"
#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck struct_class): the position is a float struct on the host.
 * MIPS passes the Rage Wars Triple (three s32) and a CVector3 in the same registers; x86-64
 * and arm64 do not (integer vs SSE/FP registers), so Triple means three floats here. */
typedef struct { float x, y, z; } T2HostTriple;
#define Triple T2HostTriple
#endif


extern void Actor_SpawnParticle(void *, s32, Triple, s32);
void func_00238134(void *actor,int unused1,int unused2,Triple input,int event) {
 Actor_SpawnParticle(actor,event,input,0);
}
