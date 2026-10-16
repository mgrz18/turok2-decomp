/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025A844_de_4.c, func_8025BDAC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed short s16;
struct HeaderRecAt102;
typedef struct HeaderRecAt102 HeaderRecAt102;
struct HeaderRecAt102;
struct HeaderRecAt102 {
    char pad[0x102];
    s16 local;
};
struct SlotCode8025A3EC;
struct SlotCode8025A3EC {
    int id;
    char pad4[0x9C];
    int keyA;
    char padA4[8];
    int keyB;
    char padB0[0x1C];
};
struct SlotCode8025A3EC;
typedef struct SlotCode8025A3EC SlotCode8025A3EC;
struct ObjRecAtCV4;
struct ObjRecAtCV4 {
    HeaderRecAt102 *owner;
    char pad4[8];
    SlotCode8025A3EC slots[16];
};
struct HeaderRecAt102;
struct ObjRecAtCV4;
typedef struct ObjRecAtCV4 ObjRecAtCV4;
struct HeaderRecAt102;
int func_002740E8(ObjRecAtCV4 *obj, int keyA, int keyB) {
    int i;
    int *keys = &obj->slots[0].keyB;
    for (i = 0; i < 16; i++) {
        if (keys[i * 0x33 - 43] != -1 && obj->owner->local != i && keys[i * 0x33 - 3] == keyA &&
            keys[i * 0x33] == keyB) {
            return i;
        }
    }
    return -1;
}
