/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025C524_de_13.c, func_8025D844_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
struct AudioHeapD844 {
    u8 *start;
    u8 *cursor;
    u32 size;
};
struct AudioRootD844 {
    char pad0[0x1DA8];
    struct AudioHeapD844 heap;
};
struct AudioParamsD844 {
    s32 count38, count1C;
    u8 kind;
    u8 pad9[3];
    struct AudioHeapD844 *context;
    s32 value10, value14, value18;
};
struct AudioStateD844 {
    struct AudioRootD844 *root;
    char pad4[0xC];
    void *allocation;
    void *player;
    s32 mode, flags, unused20, timer, index, unused2C;
    f32 first, second;
    s32 counter;
    f32 third, fourth;
};
struct InitParams;
struct InitParams {
    s32 count38;
    s32 count1C;
    u8 kind;
    u8 pad09[3];
    s32 context;
    s32 value10;
    s32 value14;
    s32 value18;
};
struct InitParams;
typedef struct InitParams InitParams;
extern float D_800A81C8;
extern float D_800A81CC;
s32 func_002912A0(s32 a, s32 b, void *c, s32 d, s32 e);
void func_0028FA10(void *arg0, InitParams *arg1);
void func_00274F34(struct AudioStateD844 *state, struct AudioRootD844 *root) {
    struct AudioParamsD844 params;
    void *player;
    state->root = root;
    state->mode = 16;
    state->index = -1;
    state->flags = 0;
    params.count38 = 20;
    params.count1C = 192;
    params.kind = 20;
    params.context = &state->root->heap;
    params.value10 = 0;
    params.value14 = 0;
    params.value18 = 0;
    player = func_002912A0(0, 0, &state->root->heap, 1, 0x7C);
    state->player = player;
    func_0028FA10(player, &params);
    state->allocation = func_002912A0(0, 0, &state->root->heap, 1, 0xF8);
    state->timer = 50;
    state->counter = 0;
    state->first = D_800A81C8;
    state->second = D_800A81CC;
    state->third = D_800A81C8;
    state->fourth = D_800A81C8;
}
