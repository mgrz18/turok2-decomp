/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/SYSTEM_3/802A6AC0_2.CPP, func_802A6450_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
struct CVector3_t;
struct CVector3_t;
typedef struct CVector3_t CVector3;
struct CVector3_t;
struct CVector3_t {
    f32 x;
    f32 y;
    f32 z;
};
struct Node802A697C;
struct Node802A697C {
    u32 unk0;
    f32 value;
    s32 childValue;
    s32 matrixIndex;
};
struct Node802A697C;
typedef struct Node802A697C Node802A697C;
struct State802A6DF8;
struct State802A6DF8 {
    struct State802A6DF8 *prev;
    struct State802A6DF8 *next;
    Node802A697C *node;
    u8 padC[0x10];
    void *object;
    s32 contextId;
    f32 value;
    u8 pad28[0xC];
    s32 renderMode;
    u8 pad38[4];
    u32 flags;
    struct Node438C *vertices;
    u8 pad44[4];
    s32 vertexCount;
};
struct List802A6DF8;
struct State802A6DF8;
struct List802A6DF8 {
    struct State802A6DF8 *head;
    struct State802A6DF8 *tail;
    s32 count;
};
struct List802A6DF8;
typedef struct List802A6DF8 List802A6DF8;
struct Lists802A6DF8;
struct Lists802A6DF8 {
    List802A6DF8 active;
    List802A6DF8 inactive;
};
struct Lists802A6DF8;
typedef struct Lists802A6DF8 Lists802A6DF8;
struct Owner802A6DF8;
struct Owner802A6DF8 {
    u8 pad0[0x7514];
    Lists802A6DF8 lists;
};
struct State802A6DF8;
typedef struct State802A6DF8 State802A6DF8;
struct Owner802A6DF8;
typedef struct Owner802A6DF8 Owner802A6DF8;
struct Node438C;
typedef struct Node438C Node438C;
struct Node438C;
struct Node438C {
    u8 pad0[4];
    struct Node438C *next;
    u8 pad8[8];
    CVector3 position;
    f32 a;
    f32 b;
    f32 c;
    f32 matrices[2][16];
};
struct Node438C;
struct Node802A697C;
void *func_00236D10(Owner802A6DF8 *arg0, s32 arg1, s32 arg2) {
    State802A6DF8 *node;
    node = arg0->lists.inactive.head;
    if (node != 0) {
        do {
            if ((s32)node->object == arg1) {
                if (node->contextId == arg2) {
                    return node;
                }
            }
            node = node->next;
        } while (node != 0);
    }
    return 0;
}
