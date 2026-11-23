#ifndef STRUCTS_TEXTURE_H
#define STRUCTS_TEXTURE_H

#include "common.h"

/* A texture reference: the two words Texture_Request / Texture_RequestSync hand
 * to CCache__Request (its second and third arguments); func_00207908 sets them. */
typedef struct TextureRef {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 unk4;
} TextureRef;

#endif
