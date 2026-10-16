/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80267198_de_2.c, func_80267CE4_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
struct Triple;
struct Triple {
    s32 x;
    s32 y;
    s32 z;
};
struct Triple;
typedef struct Triple Triple;
struct Triple;
struct Triple;
struct Triple;
struct SevenBytes;
typedef struct SevenBytes SevenBytes;
struct Node_func_80267CE4_de;
struct Node_func_80267CE4_de {
    s32 unused;
    struct Node_func_80267CE4_de *next;
};
struct SevenBytes;
struct SevenBytes {
    u8 a;
    u8 b;
    u8 c;
    u8 d;
    u8 e;
    u8 f;
    u8 g;
};
extern void func_00238888(s32 arg0, s32 arg1, s32 arg2, Triple arg3, SevenBytes arg6);
struct Node_func_80267CE4_de;
typedef struct Node_func_80267CE4_de Node_func_80267CE4_de;
struct Triple;
extern void func_00281060(void *view, s32 red, s32 green, s32 blue, s32 mode, s32 alpha, s32 duration, s32 delay);
struct Triple;
struct Triple;
struct Triple;
struct Triple;
extern void *D_801198D0;
extern s32 func_0028124C(Node_func_80267CE4_de *arg0, Triple arg1);
void func_00238888(s32 arg0, s32 arg1, s32 arg2, Triple arg3, SevenBytes arg6) {
    Node_func_80267CE4_de *node;
    node = D_801198D0;
    while (node != 0) {
        if (func_0028124C(node, arg3) != 0) {
            func_00281060(node, arg6.a, arg6.b, arg6.c, arg6.d,
                          arg6.e, arg6.f, arg6.g);
        }
        node = node->next;
    }
}
