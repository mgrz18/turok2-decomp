/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80256280_de_14.c, func_802571F0_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
extern u32 func_002700D0(s32 offset, s32 length);
struct Window;
typedef struct Window Window;
extern int D_800F2040;
struct Window;
struct Window {
    struct Window *next;
    struct Window *prev;
    u32 base;
    s32 frame;
    s32 buffer;
};
extern Window *D_800F2798;
extern char D_800D1CD8;
extern void func_00201E94(void *, u32, s32, s32, void *, s32, s32);
u32 func_002700D0(s32 offset, s32 length) {
    extern u32 func_0029BF80(s32);
    extern void func_002933E0(Window *, Window *);
    extern void func_002933B0(Window *);
    extern Window *D_800F2794;
    extern char D_800F2778;
    extern s32 D_800B2664;
    extern s32 D_800B2660;
    Window *window;
    Window *after;
    Window **pool;
    u32 address;
    s32 end;
    s32 top;
    s32 odd;
    s32 word;
    after = 0;
    address = offset + D_800F2040;
    end = address + length;
    for (window = D_800F2794; window != 0; window = window->next) {
        top = window->base + 0x300;
        word = window->base;
        if (address < word) {
            break;
        }
        after = window;
        if (top >= end) {
            window->frame = D_800B2660;
            return func_0029BF80(window->buffer + address - window->base);
        }
    }
    pool = &D_800F2798;
    window = *pool;
    *pool = window->next;
    func_002933B0(window);
    if (after != 0) {
        func_002933E0(window, after);
    } else if (pool[-1] != 0) {
        after = pool[-1];
        pool[-1] = window;
        window->next = after;
        window->prev = 0;
        after->prev = window;
    } else {
        pool[-1] = window;
        window->next = 0;
        window->prev = 0;
    }
    odd = address & 1;
    address -= odd;
    window->base = address;
    window->frame = D_800B2660;
    word = window->buffer;
    D_800B2664++;
    func_00201E94(&D_800D1CD8, address, 0x300, word, &D_800F2778, 0, 1);
    return func_0029BF80(word) + odd;
}
