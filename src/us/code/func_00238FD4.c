/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/RENDER_3/80268160.CPP, func_8026835C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
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

struct ObjRecAt1;
struct ObjRecAt1 {
    char pad0[0x1];
    u8 unk1;
};
struct ObjRecAt1;
typedef struct ObjRecAt1 ObjRecAt1;
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
struct Filter;
struct Filter {
    u8 type;
    u8 pad[0x14B];
    s32 mask;
};
struct Filter;
typedef struct Filter Filter;
extern void Actor_SpawnParticle(void *, s32, Triple, s32);
struct Triple;
struct Triple;
void func_00238FD4(void *arg0, Filter *arg1, s32 arg2, Triple arg3, struct IntPair arg6) {
    ObjRecAt1 *object = arg0;
    switch (arg1->type) {
    case 0:
        if (object->unk1 != arg6.field_4) {
            return;
        }
        break;
    case 1:
        if (!(arg1->mask & (arg1->type << arg6.field_4))) {
            return;
        }
        break;
    }
    Actor_SpawnParticle(arg0, arg6.field_0, arg3, 0);
}
