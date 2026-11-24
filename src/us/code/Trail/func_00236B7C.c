#include "common.h"

typedef struct Segment {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ struct Segment *next;
    /* 0x08 */ u8 pad8[0xA8 - 0x8];
    /* 0xA8 */ f32 start;
    /* 0xAC */ f32 length;
} Segment;

typedef struct {
    /* 0x00 */ u8 pad0[0x40];
    /* 0x40 */ Segment *segments;
    /* 0x44 */ u8 pad44[0x4C - 0x44];
    /* 0x4C */ f32 totalLength;
} Path;

/* Stores where each segment starts, as a fraction of the whole path. */
void Path_StoreSegmentFractions(Path *arg0) {
    f32 temp_f0;
    f32 var_f1;
    Segment *var_v0;

    if (arg0->totalLength > 0.0f) {
        var_f1 = 0.0f;
        var_v0 = arg0->segments;
        if (var_v0 != NULL) {
            do {
                var_v0->start = (f32) (var_f1 / arg0->totalLength);
                temp_f0 = var_v0->length;
                var_v0 = var_v0->next;
                var_f1 += temp_f0;
            } while (var_v0 != NULL);
        }
    }
}
