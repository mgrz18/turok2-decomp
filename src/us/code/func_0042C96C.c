/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0044E2B8.c, func_0044E3E4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
struct Entry_func_8044D794_de;
typedef struct Entry_func_8044D794_de Entry_func_8044D794_de;
struct Owner_func_8044D794_de;
typedef struct Owner_func_8044D794_de Owner_func_8044D794_de;
struct Entry_func_8044D794_de;
struct Entry_func_8044D794_de {
    char pad0[4];
    struct Entry_func_8044D794_de *next;
    s32 handle;
};
struct Entry_func_8044D794_de;
struct Owner_func_8044D794_de;
struct Owner_func_8044D794_de {
    char pad0[0x6C];
    void *unk6C;
    char pad70[0xC];
    void *unk7C;
    char pad80[0x1100];
    struct Entry_func_8044D794_de *active;
    char pad1184[0x10]; /* active at 0x1180, spare at 0x1194 */
    struct Entry_func_8044D794_de *spare;
};
extern void CList__AddHead(void *, Entry_func_8044D794_de *);
extern void CList__Remove(void *, Entry_func_8044D794_de *);
extern void func_0026DFB0(s32);
void func_0042C96C(Owner_func_8044D794_de *arg0) {
    Entry_func_8044D794_de *entry;
    Entry_func_8044D794_de *next;
    entry = arg0->active;
    while (entry != 0) {
        next = entry->next;
        func_0026DFB0(entry->handle);
        CList__Remove(&arg0->active, entry);
        CList__AddHead(&arg0->spare, entry);
        entry = next;
    }
}
