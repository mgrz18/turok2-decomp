/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80267198_de.c, func_8026740C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void Actor_SpawnParticle(void *, s32, Triple, s32);
void func_00238134(void *actor,int unused1,int unused2,Triple input,int event) {
 Actor_SpawnParticle(actor,event,input,0);
}
