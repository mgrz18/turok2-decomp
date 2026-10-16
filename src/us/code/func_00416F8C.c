/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80442EC0_de.c, func_80442EC0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
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
f32 func_00416F8C(ItemRecAt14 *arg0) {
 Binding *v = arg0->binding;
 void *p = v->storage();
 switch(v->type) {
 default: return 0.0f;
 case 0: return *(f32 *)p;
 case 3: return *(s8 *)p;
 case 4: return *(s16 *)p;
 case 1: case 2: case 5: return *(s32 *)p;
 case 6: return *(u8 *)p;
 case 7: return *(u16 *)p;
 case 8: return *(u32 *)p;
 }
}
