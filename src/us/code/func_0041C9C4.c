/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80443868_2.CPP, func_804436F8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
struct Leaf;
struct Leaf {
    char pad[0x3D];
    u8 flag;
};
struct Leaf;
struct Middle;
struct Middle {
    char pad[0x518]; /* player options pointer (0x5D8 in Rage Wars) */
    struct Leaf *leaf;
};
struct Middle;
struct RootRecAt1C;
struct RootRecAt1C {
    char pad[0x1C];
    struct Middle *middle;
};
struct RootRecAt1C;
extern s32 func_0041C9C4(void *unused, struct RootRecAt1C *root);
s32 func_0041C9C4(void *unused, struct RootRecAt1C *root) {
    root->middle->leaf->flag = 1;
    return 1;
}
