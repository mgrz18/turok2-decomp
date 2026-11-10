/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8023F43C_de_7.c, func_802405FC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

#ifdef TARGET_PORT
/* the records swapped are {pointer, f32} (ModelHit, InstanceHit): 16 bytes on the host */
typedef struct { void *p; f32 key; } T2SortRecord;
#define RenderSortRecord T2SortRecord
#endif
extern void Collision_SwapInstanceHits(RenderSortRecord *left, RenderSortRecord *right);
void Collision_SwapInstanceHits(RenderSortRecord *left, RenderSortRecord *right) {
    RenderSortRecord temporary = *left;
    *left = *right;
    *right = temporary;
}
