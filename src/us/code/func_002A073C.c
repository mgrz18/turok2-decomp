/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BE0D0_de_13.c, func_802BF67C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef float f32;
extern float func_002974C0(float x);
extern float func_002982F0(float x);
struct FftTables;
typedef struct FftTables FftTables;
struct FftTables;
struct FftTables {
    f32 *sines;
    f32 *cosines;
    s32 *order;
    s32 size;
};
extern FftTables *func_002A073C(s32 n);
extern float D_800AAC30;
extern FftTables D_800B8A30;
FftTables *func_002A073C(s32 n) {
    FftTables *tables;
    s32 half;
    s32 i;
    s32 k;
    s32 j;
    s32 m;
    s32 swap;
    s32 limit;
    s32 *order;
    f32 angle;
    f32 scale;
    f32 count;
    half = n >> 1;
    tables = &D_800B8A30;
    i = 0;
    if (i < half) {
        scale = D_800AAC30;
        count = n;
        do {
            angle = i * scale / count;
            tables->sines[i] = func_002974C0(angle);
            tables->cosines[i] = -func_002982F0(angle);
            i++;
        } while (i < half);
    }
    order = tables->order;
    limit = n * 2;
    for (k = 0; k < n; k++) {
        order[k] = k;
    }
    j = 1;
    for (k = 1; k <= limit; k += 2) {
        if (k < j) {
            swap = order[(j - 1) / 2];
            order[(j - 1) / 2] = order[(k - 1) / 2];
            order[(k - 1) / 2] = swap;
        }
        m = n;
        while (m >= 2 && j > m) {
            j -= m;
            m >>= 1;
        }
        j += m;
    }
    tables->size = n;
    return tables;
}
