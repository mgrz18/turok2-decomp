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
extern void func_0042FF10(char *);
void func_00468E0C(Triple *group) {
    int i;
    for (i = 0; i < group->z; i++) {
        func_0042FF10((char *)(i * 0xEC0 + group->y + 0x558));
    }
    func_0042FF10((char *)group + 0x598);
}
