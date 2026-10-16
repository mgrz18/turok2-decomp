/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802A25D0_de_8.c, func_802A339C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
struct Vec3;
typedef struct Vec3 Vec3;
struct Vec3;
struct Vec3 {
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
struct State802A6DF8;
struct State802A6DF8;
typedef struct State802A6DF8 State802A6DF8;
struct Node438C;
typedef struct Node438C Node438C;
struct Node438C;
struct Node438C {
    u8 pad0[4];
    struct Node438C *next;
    u8 pad8[8];
    Vec3 position;
    f32 a;
    f32 b;
    f32 c;
    f32 matrices[2][16];
};
struct Node438C;
struct Node802A697C;
extern s32 D_800B6D1C;
extern void func_0020FC74(void *, f32, f32, f32);
extern void func_0020F79C(void *);
struct GameplayCamera;
extern void func_00233E18(Node438C *, Node438C *, struct GameplayCamera *, void *);
extern void func_0020D274(void *, s32);
void func_00233FCC(void *arg0, void *arg1) {
    f32 matrix[16];
    Node438C *first;
    Node438C *next;
    State802A6DF8 *state = arg0;
    if (state->vertexCount < 2) {
        return;
    }
    first = state->vertices;
    next = first->next;
    if (first != 0) {
        func_0020FC74(matrix, first->a, first->b, first->c);
    } else {
        func_0020F79C(matrix);
    }
    while (first != 0) {
        Node438C *a;
        Node438C *b;
        if (next == 0) {
            break;
        }
        a = first;
        b = next;
        if (first == 0) {
            goto no_pair;
        }
        if (first->position.x != next->position.x || first->position.y != next->position.y) {
            goto boundary;
        }
z_test:
        if (a->position.z != b->position.z) {
            goto boundary;
        }
        a = b;
        b = b->next;
        if (a == 0) {
            goto no_pair;
        }
        if (b == 0 || a->position.x != b->position.x || a->position.y != b->position.y) {
            goto boundary;
        }
        goto z_test;
boundary:
        if (a != 0 && b != 0) {
            func_00233E18(a, b, arg1, matrix);
            goto apply;
        }
no_pair:
        {
            matrix[12] = first->position.x;
            matrix[13] = first->position.y;
            matrix[14] = first->position.z;
        }
apply:
        func_0020D274(matrix, (s32)first->matrices[D_800B6D1C]);
        first = next;
        next = first->next;
    }
    matrix[12] = first->position.x;
    matrix[13] = first->position.y;
    matrix[14] = first->position.z;
    func_0020D274(matrix, (s32)first->matrices[D_800B6D1C]);
}
