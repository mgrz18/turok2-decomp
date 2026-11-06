/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/8044ACCC_19.CPP, func_8044A6CC_de.
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
extern void CEmitterPool__Construct_2(char *);
void CCameraPool__ResetEmitterPools_2(Triple *group) {
    int i;
    for (i = 0; i < group->z; i++) {
        CEmitterPool__Construct_2((char *)(i * 0xEC0 + group->y + 0x558));
    }
    CEmitterPool__Construct_2((char *)group + 0x598);
}
