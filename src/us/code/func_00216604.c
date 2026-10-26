/* After thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars), src/game/func_0025F0F4.c, func_0025F0F4, with Turok 2's layouts. */
#include "common.h"

/* Decodes a packed animation: lays out one track per bone in the table (each
 * slot's offset found from the 0xDEADBEEF end marker), decodes every channel
 * with the encoding's decoder, box-blurs rotation keys over `radius` frames
 * and renormalizes them. */

typedef struct {
    /* 0x0 */ s32 count;
    /* 0x4 */ s32 offsets[1];
} TrackTable;

typedef struct {
    f32 x, y, z, w;
} Quaternion;

typedef void (*TrackDecoder)(u32 *, f32 *, s32);

void func_00218620(u32 *, f32 *, s32);
void func_00216464(u32 *, f32 *, s32);
void func_002186D8(u32 *, f32 *, s32);
u32 func_00217B04(s32);
u32 func_00217C58(u32 *, u32);
f32 *func_002017D4(TrackTable *, s32);
void func_00211074(Quaternion *);

extern s32 D_800E67A4;
extern s32 D_800E67A8;

s32 func_00216604(s32 source, TrackTable *table, s32 tracks, s32 channels, s32 radius, s32 encoding) {
    TrackDecoder decode;
    u32 stream;
    s32 index;
    s32 i;
    s32 j;
    s32 offset;
    f32 total;
    f32 weight;

    decode = NULL;
    switch (encoding) {
    case 0:
        decode = func_00218620;
        break;
    case 1:
        decode = func_00216464;
        break;
    case 2:
        decode = func_002186D8;
        break;
    }
    index = 1;
    table->count = tracks;
    table->offsets[0] = (tracks * 4 + 15) & ~7;
    if (tracks > 0) {
        do {
            table->offsets[index] = 0xDEADBEEF;
            index++;
        } while (tracks >= index);
    }
    i = 0;
    if (tracks > 0) {
        s32 size = D_800E67A8 << 4;

        do {
            for (index = 0; index <= table->count; index++) {
                if (table->offsets[index] == 0xDEADBEEF) {
                    break;
                }
            }
            index--;
            i++;
            table->offsets[index + 1] = table->offsets[index] + size;
        } while (i < tracks);
    }
    i = 0;
    stream = func_00217B04(source);
    if (tracks > 0) {
        do {
            f32 *track = func_002017D4(table, i);

            j = 0;
            if (channels > 0) {
                f32 *out = track;

                do {
                    func_00217C58(&stream, 31);
                    decode(&stream, out, D_800E67A4);
                    out++;
                    j++;
                } while (j < channels);
            }
            i++;
        } while (i < tracks);
    }
    i = 0;
    if (tracks > 0) {
        do {
            f32 *keys = func_002017D4(table, i);

            if (channels == 4 && radius != 0) {
                s32 k;
                s32 frames;

                k = 0;
                frames = D_800E67A8;
                do {
                    /* box blur of channel k over [-radius, radius] frames, wrapping */
                    for (j = 0; j < frames; j++) {
                        total = 0.0f;
                        weight = 0.0f;
                        for (offset = -radius; offset <= radius; offset++) {
                            weight += 1.0f;
                            total += keys[((j + offset + frames) % frames) * 4 + k];
                        }
                        keys[j * 4 + k] = total / weight;
                    }
                    k++;
                } while (k < channels);
            }
            i++;
        } while (i < tracks);
    }
    if (channels == 4) {
        if (tracks > 0) {
            j = 0;
            do {
                Quaternion *rotations = (Quaternion *) func_002017D4(table, j);

                for (i = 0; i < D_800E67A8; i++) {
                    func_00211074(&rotations[i]);
                }
                j++;
            } while (j < tracks);
        }
    }
    return 1;
}
