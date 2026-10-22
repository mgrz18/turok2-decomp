/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/particle/8028308C_4.CPP, func_8028466C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

struct EntryRecAt4V1;
struct EntryRecAt4V1 {
    int unk0;
    void *handle;
    char pad8[0xC];
};
struct EntryRecAt4V1;
typedef struct EntryRecAt4V1 EntryRecAt4V1;
extern void func_0022F1C8(char *obj, void *handle);
void func_00232C7C(char *obj, unsigned char count) {
    int i;
    for (i = 0; i <= count; i++) {
        EntryRecAt4V1 *e = (EntryRecAt4V1 *)(obj + i * sizeof(EntryRecAt4V1) + 0xEC28);
        if (e->handle != 0) {
            func_0022F1C8(obj, e->handle);
            return;
        }
    }
}
