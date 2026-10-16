/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802BA210_de_29.c, func_802BA700_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef struct {
    unsigned int w0;
    unsigned int w1;
} Awords;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
extern void func_0029AAD0(s32 arg0);
struct State_func_802BA700_de;
typedef struct State_func_802BA700_de State_func_802BA700_de;
struct State_func_802BA700_de;
struct State_func_802BA700_de {
    u16 status;
    u16 pad2;
    u32 word4;
    Awords *inner;
    u32 flags;
};
extern State_func_802BA700_de *D_800B7AC4;
extern u32 func_0029DFF0(void);
extern void func_0029E010(u32 arg0);
void func_0029AAD0(s32 arg0) {
    u32 token;
    token = func_0029DFF0();
    if (arg0 & 1) {
        D_800B7AC4->flags |= 8;
    }
    if (arg0 & 2) {
        D_800B7AC4->flags &= ~8;
    }
    if (arg0 & 4) {
        D_800B7AC4->flags |= 4;
    }
    if (arg0 & 8) {
        D_800B7AC4->flags &= ~4;
    }
    if (arg0 & 0x10) {
        D_800B7AC4->flags |= 0x10;
    }
    if (arg0 & 0x20) {
        D_800B7AC4->flags &= ~0x10;
    }
    if (arg0 & 0x40) {
        D_800B7AC4->flags = (D_800B7AC4->flags | 0x10000) & ~0x300;
    }
    if (arg0 & 0x80) {
        D_800B7AC4->flags &= ~0x10000;
        D_800B7AC4->flags |= D_800B7AC4->inner->w1 & 0x300;
    }
    D_800B7AC4->status |= 8;
    func_0029E010(token);
}
