/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_3/802A35C0_6.CPP, func_802A56C4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
struct Vtx;
typedef struct Vtx Vtx;
struct Vtx;
struct Vtx {
    s16 x;
    s16 y;
    s16 z;
    s16 flag;
    s16 s;
    s16 t;
    u8 r;
    u8 g;
    u8 b;
    u8 a;
};
struct RibbonVertexPool {
    Vtx vertices[2][300]; /* 0x0000 (Rage Wars has two extra words before this) */
    s32 remaining;
    Vtx *next;
};
s32 func_00235F8C(void *arg0, s32 arg1)
{
    struct RibbonVertexPool *pool = arg0;
    s32 avail;
    s32 head;
    avail = pool->remaining;
    if (avail < arg1) {
        return 0;
    }
    head = (s32)pool->next;
    pool->remaining = avail - arg1;
    pool->next += arg1;
    return head;
}
