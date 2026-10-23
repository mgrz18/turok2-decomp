typedef signed int s32;
typedef float f32;

typedef struct {
    /* 0x00 */ char pad0[0x8];
    /* 0x08 */ f32 length;
} AnimData;

typedef struct {
    /* 0x00 */ char pad0[0x18];
    /* 0x18 */ AnimData *data;
    /* 0x1C */ char pad1C[0x8];
    /* 0x24 */ s32 frames;
} AnimState;

void func_00290D5C(AnimState *s, f32 scale) {
    if (s->data != 0) {
        s->frames = scale * s->data->length;
    } else {
        s->frames = 0x1E8;
    }
}
