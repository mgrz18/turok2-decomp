#include "common.h"

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
