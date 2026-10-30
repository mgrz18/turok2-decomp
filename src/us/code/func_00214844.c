/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WORLD_2/8024E914_14.CPP, func_802504B0_de (with func_8027892C_de of src/GAMECORE/CODE_80278900.CPP inlined), with Turok 2's layouts. */

#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0xE];
    /* 0x0E */ s8 argument;
    /* 0x0F */ u8 padF[3];
    /* 0x12 */ s8 lod;
} PropDescriptor;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ PropDescriptor *descriptor;
    /* 0x18 */ u8 pad18[0xC];
    /* 0x24 */ s32 cache;
    /* 0x28 */ f32 matrix[16];
    /* 0x68 */ u8 pad68[0x18];
    /* 0x80 */ s32 colorKey;
    /* 0x84 */ s32 value;
    /* 0x88 */ u16 flags;
    /* 0x8A */ u8 colorFrame;
} PlacedProp;

/* A vertex (0x10 bytes). */
typedef struct {
    /* 0x0 */ s16 pos[3];
    /* 0x6 */ u16 flag;
    /* 0x8 */ s16 tc[2];
    /* 0xC */ u8 r, g, b, a;
} ColVtx;

typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 count;
    /* 0x8 */ ColVtx v[1];
} VtxChunk;

/* What this reads around D_8011ACB0. */
typedef struct {
    /* 0x00 */ u8 pad0[0x34];
    /* 0x34 */ s32 split;
} GameState;

extern u8 D_8011AAF1;           /* multiplayer */
extern GameState D_8011ACB0;
extern s32 D_800ACAD8;
extern char D_800A57F8[];
extern s8 D_800ACAF4[];         /* the colour pulse, 64 steps */
extern s32 D_800B6D18;          /* frame count */
extern u8 D_800B6D1B;           /* its low byte */

s32 func_00245724(void);
void **func_00214DBC(PlacedProp *prop);
void func_00214F14(void);
s32 **func_00203CA0(s32, s32, void **, s32, s32, void *, void *, void *, s32);
void func_0020565C(s32, s32 **);
VtxChunk *func_002017D4(void *file, s32 index);
void func_0020B6A0(void **resource, void *matrix, s32 segment, void *lights, void *textures, s32 lod, s32 pass);
void func_002051F4(s32, void *);

/* The colour pulse (Rage Wars' func_8027892C_de): every vertex flagged 0x70
 * gets its base colour from colors plus a step of the pulse table. */
static __inline__ void pulseColors(s32 *object, u8 *colors) {
    s32 state;
    s32 i;
    s32 j;
    s32 count;
    VtxChunk *chunk;
    ColVtx *record;
    ColVtx *end;

    state = D_800B6D18;
    i = 0;
    if (!(state & 1)) {
        count = *object;
        for (j = 0; j < count; j++) {
            chunk = func_002017D4(object, j);
            record = chunk->v;
            end = record + chunk->count;
            if (record != end) {
                do {
                    u16 flags = record->flag;
                    s32 amount;
                    s32 value;
                    u8 *color;

                    if (flags & 0x70) {
                        amount = D_800ACAF4[(state + ((flags & 0x70) >> 2)) & 0x3F];
                        amount >>= (flags & 0xC) >> 2;
                        color = colors + i * 4;

                        value = amount + color[0];
                        if (value >= 0x100) value = 0xFF;
                        if (value < 0) value = 0;
                        record->r = value;

                        value = amount + color[1];
                        if (value >= 0x100) value = 0xFF;
                        if (value < 0) value = 0;
                        record->g = value;

                        value = amount + color[2];
                        if (value >= 0x100) value = 0xFF;
                        if (value < 0) value = 0;
                        record->b = value;
                    }
                    record++;
                    i++;
                } while (record != end);
            }
        }
    }
}

/* Queues a placed prop's model: the level of detail from its descriptor (in
 * multiplayer, from the game setup for some modes), and its vertex colours
 * pulsed once a frame. */
void func_00214844(PlacedProp *prop) {
    s32 argument;
    s32 mode;
    void **instance;
    s32 lod;
    s32 **resource;
    s32 *header;
    s32 *object;
    u8 *colors;
    void *data;

    do { /* a block of its own, as an early-out macro would make; it decides the register allocation */
        if (prop->flags & 0x40) {
            return;
        }
    } while (0);
    argument = prop->descriptor->argument;
    if (D_8011AAF1) {
        mode = func_00245724();
        switch (mode) {
            case 1:
            case 4:
            case 5:
            case 6:
            case 7:
            case 8:
            case 9:
                lod = prop->descriptor->lod;
                break;
            default: {
                GameState *game = &D_8011ACB0;
                if (game->split) {
                    lod = ((s8 *) game)[-0x1B0];
                } else {
                    lod = ((s8 *) game)[-0x1B5];
                }
                break;
            }
        }
    } else {
        lod = prop->descriptor->lod;
    }
    instance = func_00214DBC(prop);
    if (instance == NULL) {
        return;
    }
    resource = func_00203CA0(0, prop->colorKey | D_800ACAD8, instance, prop->cache, 8, prop, func_00214F14, D_800A57F8, 0);
    if (resource != NULL) {
        func_0020565C(0, resource);
        header = *resource;
        if (header[0] == 1) {
            data = (u8 *) header + header[1];
            object = (s32 *) data;
            colors = (u8 *) header + header[2];
            if (prop->colorFrame != D_800B6D1B) {
                pulseColors(object, colors);
            }
            func_0020B6A0(instance, prop->matrix, 0, 0, data, argument, lod);
        } else {
            func_0020B6A0(instance, prop->matrix, 0, 0, header + 2, argument, lod);
        }
        func_002051F4(0, resource);
    }
    func_002051F4(0, instance);
    prop->colorFrame = D_800B6D1B;
}
