#include "common.h"
#include "structs/rumble.h"

/* Host body: the curve is a pointer (RumbleCurve.curve), pointer-wide on the host. */
void RumbleCurve_Init(RumbleCurve *arg0, void *arg1) {
    arg0->curve = arg1;
    arg0->unk4 = 0;
    arg0->unk8 = 0;
}

extern f32 Random_Blend(f32 arg0, f32 arg1);
struct SampledCurve;
struct SampledCurve {
    int type;
    unsigned char samples[0x10];
    float length;
};
typedef struct RandomCurve {
    s32 type;
    f32 period;
    u8 minimum;
    u8 maximum;
} RandomCurve;
struct PlayerRecAt8;
struct PlayerRecAt8 {
    struct SampledCurve *curve;
    float time;
    float value;
};
typedef struct PlayerRecAt8 PlayerRecAt8;
s32 RumbleCurve_Advance(void *arg0);
typedef struct SampledCurve SampledCurve;
extern f32 gCurrentFB[2];   /* local type: [1] is gRefreshRate (globals/framebuffer.h); this file has its own typedefs instead of common.h */

s32 RumbleCurve_Advance(void *arg0) {
    PlayerRecAt8 *player = arg0;
    f32 temp_f1;
    f32 temp_f1_2;
    s32 temp_v1;
    SampledCurve *temp_a0;
    temp_a0 = player->curve;
    temp_v1 = temp_a0->type;
    switch (temp_v1) {
    case 0: {
        f32 divisor = gCurrentFB[1];
        temp_f1 = player->time + (1.0f / divisor);
        player->time = temp_f1;
        if (temp_a0->length <= temp_f1) {
            return 1;
        }
        goto block_7;
    }
    case 1: {
        RandomCurve *random = (void *)temp_a0;
        f32 divisor = gCurrentFB[1];
        temp_f1_2 = player->time - (1.0f / divisor);
        player->time = temp_f1_2;
        if (temp_f1_2 <= 0.0f) {
            player->time =
                temp_f1_2 + random->period;
            player->value = Random_Blend(
                (f32)random->minimum * 0.003921569f,
                (f32)random->maximum * 0.003921569f);
        }
        goto block_7;
    }
    default:
block_7:
        return 0;
    }
}

typedef struct {
    /* 0x00 */ s32 type;
    /* 0x04 */ u8 samples[0x10];
    /* 0x14 */ f32 length;
} Curve;
typedef struct {
    /* 0x00 */ Curve *curve;
    /* 0x04 */ f32 time;
    /* 0x08 */ f32 value;
} CurveState;

f32 RumbleCurve_GetValue(CurveState *arg0) {
    s32 temp_a1;
    Curve *temp_v1;

    temp_v1 = arg0->curve;
    temp_a1 = temp_v1->type;
    switch (temp_a1) {                              /* irregular */
    case 0:
        return (f32) temp_v1->samples[(s32) ((arg0->time * 16.0f) / temp_v1->length)] * 0.003921569f;
    case 1:
        return arg0->value;
    default:
        return 0.0f;
    }
}
