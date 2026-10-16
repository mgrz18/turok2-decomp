/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8023F43C_de_7.c, func_802405FC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;
typedef struct RenderSortRecord {
    s32 index;
    f32 depth;
} RenderSortRecord;
extern void func_0023DB5C(RenderSortRecord *left, RenderSortRecord *right);
void func_0023DB5C(RenderSortRecord *left, RenderSortRecord *right) {
    RenderSortRecord temporary = *left;
    *left = *right;
    *right = temporary;
}
