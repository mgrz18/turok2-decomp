/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8025C524_de_13.c, func_8025D844_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

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
