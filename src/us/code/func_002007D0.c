/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80255C4C_de_8.c, func_80255F70_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
struct IntrusiveList;
struct IntrusiveList {
    void *head;
    void *tail;
    s32 prevOffset;
    s32 nextOffset;
    s32 count;
};
typedef struct IntrusiveList IntrusiveList;
extern void func_002007D0(IntrusiveList *arg0);
void func_002007D0(IntrusiveList *arg0) {
    char *cursor = arg0->head;
    int stride = arg0->nextOffset;
    int value = *(int *)(cursor + stride);
    int count = arg0->count - 1;
    arg0->count = count;
    arg0->head = value;
}
