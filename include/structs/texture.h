#ifndef STRUCTS_TEXTURE_H
#define STRUCTS_TEXTURE_H

#include "common.h"

/* A texture reference: the two words Texture_Request / Texture_RequestSync hand
 * to CCache__Request (its second and third arguments); func_00207908 sets them. */
typedef struct TextureRef {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 unk4;
} TextureRef;

/* One animation frame of a texture resource (block 0 of its CISet): its format (the
 * combiner switch of the trail, particle and emitter draws), its rate in frames, the
 * log2 of its width and height. */
typedef struct TextureFrame {
    /* 0x0 */ u8 format;
    /* 0x1 */ u8 speed;
    /* 0x2 */ u8 shiftS;
    /* 0x3 */ u8 shiftT;
    /* 0x4 */ u8 lod;
    /* 0x5 */ u8 kind;
} TextureFrame;

/* What Texture_GetInfo reads out of a texture resource: its frame and palette counts
 * (blocks 1 and 2) and its frame table (block 0); all zero when it is empty. */
typedef struct TextureInfo {
    /* 0x0 */ s32 count;
    /* 0x4 */ s32 palettes;
    /* 0x8 */ TextureFrame *frame;
} TextureInfo;

#endif
