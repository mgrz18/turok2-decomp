/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8043E134_de.c, func_8043DEF8_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef signed int s32;
struct PakMenuSlot;
struct PakMenuSlot {
    char pad0[0x4];
    s8 unk4;
    char pad05[0xB0 - 5];
    s32 unkB0;
};
struct PakMenuSlot;
typedef struct PakMenuSlot PakMenuSlot;
struct OwnerCode8043DF84;
struct OwnerCode8043DF84 {
    char pad0[0x85C];
    s32 unk85C;
};
struct PakMenuSlot;
struct MenuRecAt20V2;
typedef struct MenuRecAt20V2 MenuRecAt20V2;
struct MenuRecAt20V2;
struct OwnerCode8043DF84;
struct MenuRecAt20V2 {
    char pad0[0x1C];
    struct OwnerCode8043DF84 *owner;
    PakMenuSlot *widget;
};
extern void func_00417B74(void *arg0, s32 arg1);
extern void func_00285A68(s32 arg);
extern void func_00285A80(s32 arg);
void func_00417B74(void *arg0, s32 arg1) {
    s32 val;
    MenuRecAt20V2 *menu = arg0;
    val = menu->widget->unk4;
    if (arg1 != 0) {
        func_00285A68(val);
    } else {
        func_00285A80(val);
    }
}
