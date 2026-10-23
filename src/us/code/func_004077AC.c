typedef int s32;
typedef float f32;

typedef struct {
    char pad0[0xD4];
    s32 flags;                  /* 0xD4 */
    char padD8[0x1FC - 0xD8];
    f32 unk_1FC;                /* 0x1FC */
} Actor;

/* Fields addressed relative to D_8012F9E4; the mode word sits 0xC before it. */
typedef struct {
    f32 unk_0;                  /* 0x00 */
    char pad4[0x50 - 0x4];
    f32 unk_50;                 /* 0x50 */
    char pad54[0x5C - 0x54];
    f32 unk_5C;                 /* 0x5C */
} State;

extern State D_8012F9E4;
extern f32 D_800C0540;          /* 1.0f */

void func_004077AC(Actor *actor) {
    f32 one = D_800C0540;
    State *state;

    actor->unk_1FC = one;
    actor->flags |= 0x2000;
    state = &D_8012F9E4;
    state->unk_0 = one;
    if (((s32 *)state)[-3] == 3) {
        state->unk_50 = state->unk_5C;
    } else {
        state->unk_50 = 0;
    }
}
