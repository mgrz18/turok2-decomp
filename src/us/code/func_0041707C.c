/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80442FB0_de.c, func_80442FB0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

struct Binding;
typedef struct Binding Binding;
struct ItemRecAt14;
typedef struct ItemRecAt14 ItemRecAt14;
struct Binding;
struct Binding {
    int pad0;
    int type;
    char pad8[16];
    void *(*storage)(void);
};
struct Binding;
struct ItemRecAt14;
struct ItemRecAt14 {
    char pad[20];
    struct Binding *binding;
};
void func_0041707C(ItemRecAt14 *item,float value) {
 float *storage;
 struct Binding *binding;
 unsigned int *word;
 binding=item->binding;
 storage=binding->storage();
 switch(binding->type) {
 case 0:*(float *)storage=value;return;
 case 3:*(signed char *)storage=(int)value;return;
 case 4:*(short *)storage=(int)value;return;
 case 1:case 2:case 5:*(int *)storage=(int)value;return;
 case 6:*(unsigned char *)storage=(unsigned int)value;return;
 case 7:*(unsigned short *)storage=(unsigned int)value;return;
 case 8:word=(unsigned int *)storage;*word=(unsigned int)value;return;
 }
}
