/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_3/80268160.CPP, func_80268150_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
struct Triple;
struct Triple;
struct Triple {
    s32 x;
    s32 y;
    s32 z;
};
struct Triple;
typedef struct Triple Triple;
#ifdef TARGET_PORT
/* ABI (docs/port/ABI.md, abicheck struct_class): the position is a float struct on the host.
 * MIPS passes the Rage Wars Triple (three s32) and a CVector3 in the same registers; x86-64
 * and arm64 do not (integer vs SSE/FP registers), so Triple means three floats here. */
typedef struct { float x, y, z; } T2HostTriple;
#define Triple T2HostTriple
#endif

struct IntPair;
struct IntPair {
    int field_0;
    int field_4;
};
struct IntPair;
struct IntPair;
struct IntPair;
struct Triple;
struct Triple;
struct Triple;
struct IntPair;
struct IntPair;
struct Triple;
struct Triple;
struct Triple;
extern void ImpactEvent_ApplyBlast(void *, s32, s32, Triple, struct IntPair, s32, s32, s32);
void ImpactEvent_AreaDamagePush800000(s32 arg0, s32 arg1, s32 arg2, Triple arg3, struct IntPair arg6) {
    ImpactEvent_ApplyBlast((void *)arg0, arg1, arg2, arg3, arg6, 1, 1, 0x800000);
}
