typedef signed int s32;
typedef float f32;

typedef struct {
    /* 0x00 */ f32 value;
    /* 0x04 */ char pad4[0x4C];
    /* 0x50 */ f32 out;
    /* 0x54 */ char pad54[0x8];
    /* 0x5C */ f32 src;
} SubState;

typedef struct {
    /* 0x00 */ s32 mode;
    /* 0x04 */ char pad4[0x8];
    /* 0x0C */ SubState sub;
} State;

extern f32 D_800C0540;
extern State D_8012F9D8;

void func_004407AC(char *actor) {
    f32 v = D_800C0540;
    SubState *sub = &D_8012F9D8.sub;

    *(f32 *) (actor + 0x1FC) = v;
    *(s32 *) (actor + 0xD4) |= 0x2000;
    sub->value = v;
    if (D_8012F9D8.mode == 3) {
        v = sub->src;
        sub->out = v;
    } else {
        sub->out = 0.0f;
    }
}
