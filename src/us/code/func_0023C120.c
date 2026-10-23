#include "common.h"

typedef struct Vec3 {
    f32 x, y, z;
} Vec3;

typedef struct Src {
    u8 pad0[4];
    Vec3 pos;       /* 0x4 */
    s32 unk10;      /* 0x10 */
} Src;

typedef struct Outputs {
    u8 pad0[0x2C];
    Vec3 *pos;      /* 0x2C */
    s32 *unk30;     /* 0x30 */
    s32 *unk34;     /* 0x34 */
    s32 unk38;      /* 0x38 */
    s32 *unk3C;     /* 0x3C */
    s32 unk40;      /* 0x40 */
} Outputs;

void func_0023C120(Outputs *out, Src *src) {
    if (out->pos != NULL) {
        *out->pos = src->pos;
    }
    if (out->unk30 != NULL) {
        *out->unk30 = src->unk10;
    }
    if (out->unk34 != NULL) {
        *out->unk34 = out->unk38;
    }
    if (out->unk3C != NULL) {
        *out->unk3C = out->unk40;
    }
}
