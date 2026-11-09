/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/GAMECORE/CODE_80278900.CPP, func_8027892C_de, with Turok 2's layouts. */
#include "common.h"
#include "globals/engine.h"
#include "globals/scene.h"
#include "protos/iset.h"

/* Pulses the vertex colours of a model whose vertices carry a pulse phase
 * (flag bits 0x70) and amplitude shift (bits 0xC), from their saved base colours. */

typedef struct {
    /* 0x0 */ s16 ob[3];
    /* 0x6 */ u16 flag;
    /* 0x8 */ s16 tc[2];
    /* 0xC */ u8 cn[4];
} Vertex;

typedef struct {
    /* 0x0 */ s32 field0;
    /* 0x4 */ s32 count;
    /* 0x8 */ Vertex vertices[1];
} VertexGroup;


void Model_PulseVertexColors(s32 *object, u8 *colors) {
    s32 state = gFrameCount;
    s32 i = 0;
    s32 g, count;

    if (!(state & 1)) {
        count = object[0];
        for (g = 0; g < count; g++) {
            VertexGroup *group = CISet__GetBlock(object, g);
            Vertex *v = group->vertices;
            Vertex *end = v + group->count;

            while (v != end) {
                u16 flags = v->flag;
                s32 amount, value;
                u8 *color;

                if (flags & 0x70) {
                    amount = gColorPulseTable[(state + ((flags & 0x70) >> 2)) & 0x3F];
                    amount >>= (flags & 0xC) >> 2;
                    color = colors + i * 4;

                    value = amount + color[0];
                    if (value >= 0x100) value = 0xFF;
                    if (value < 0) value = 0;
                    v->cn[0] = value;

                    value = amount + color[1];
                    if (value >= 0x100) value = 0xFF;
                    if (value < 0) value = 0;
                    v->cn[1] = value;

                    value = amount + color[2];
                    if (value >= 0x100) value = 0xFF;
                    if (value < 0) value = 0;
                    v->cn[2] = value;
                }
                v++;
                i++;
            }
        }
    }
}
