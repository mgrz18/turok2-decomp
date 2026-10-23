/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_002512C8.c, func_80251328_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
extern s32 func_00203068(s32 unused0, s32 unused1, s32 keep_recent);
struct Node_func_80251328_de;
struct Node_func_80251328_de {
    s32 resource;
    char pad4[8];
    s32 flags;
    s32 lastUsed;
    char pad14[4];
    struct Node_func_80251328_de *next;
};
struct Node_func_80251328_de;
typedef struct Node_func_80251328_de Node_func_80251328_de;
extern Node_func_80251328_de *D_800D81E4;
extern Node_func_80251328_de *D_800D81D8;
extern Node_func_80251328_de **D_800D81D4;
extern s32 D_800D8DAC;
extern s32 D_800D8DF0;
extern char D_800D1CC0;
extern char D_800D81E0;
extern char D_800D81F4;
extern void func_002065F8(void *, Node_func_80251328_de *);
extern s32 func_002026EC(void *, s32);
extern void CList__Remove(void *, Node_func_80251328_de *);
static __inline__ Node_func_80251328_de *find_stale(Node_func_80251328_de **recent) {
    Node_func_80251328_de *node;
    for (node = D_800D81E4; node != 0; node = node->next) {
        if (!(node->flags & 0x702)) {
            if ((u32)(D_800D8DF0 - node->lastUsed) >= 5) {
                return node;
            }
            *recent = node;
            break;
        }
    }
    for (; node != 0; node = node->next) {
        if (!(node->flags & 0x702) && (u32)(D_800D8DF0 - node->lastUsed) >= 5) {
            return node;
        }
    }
    return 0;
}
s32 func_00203068(s32 unused0, s32 unused1, s32 keep_recent) {
    Node_func_80251328_de *recent;
    Node_func_80251328_de *node;
    s32 result;
    recent = 0;
    node = find_stale(&recent);
    if (node == 0) {
        if (keep_recent == 0) {
            node = recent;
        }
    }
    if (node != 0) {
        func_002065F8(0, node);
        result = func_002026EC(&D_800D1CC0, node->resource);
        CList__Remove(&D_800D81E0, node);
        if (node->flags & 0x1000) {
            CList__Remove(&D_800D81F4, node);
        }
        if (D_800D81D8 == node) {
            D_800D81D8 = 0;
            node->flags = 0;
        } else {
            node->flags = 0;
        }
        *(Node_func_80251328_de **)(D_800D8DAC * 4 + (char *)D_800D81D4) = node;
        D_800D8DAC++;
        return result;
    }
    return 0;
}
