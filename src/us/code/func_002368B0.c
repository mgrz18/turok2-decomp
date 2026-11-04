#include "common.h"

/* The object a swoosh is attached to (a game object: 0x10F is m_ActiveSwooshes). */
typedef struct {
    /* 0x000 */ u8 pad0[0x10F];
    /* 0x10F */ u8 activeSwooshes;
    /* 0x110 */ u8 pad110[0x1B5 - 0x110];
    /* 0x1B5 */ u8 activeSwooshes2;
} SwooshOwner;

typedef struct {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ f32 y;
} SwooshPoint;

/* A swoosh (weapon trail): linked at 4, its point at 8, its owner at 0x1C, a height at 0x24,
 * flags at 0x3C (1 / 2: counted in the owner at 0x10F / 0x1B5). */
typedef struct Swoosh {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ struct Swoosh *next;
    /* 0x08 */ SwooshPoint *point;
    /* 0x0C */ u8 padC[0x1C - 0xC];
    /* 0x1C */ SwooshOwner *owner;
    /* 0x20 */ u8 pad20[4];
    /* 0x24 */ f32 top;
    /* 0x28 */ u8 pad28[0x3C - 0x28];
    /* 0x3C */ s32 flags;
} Swoosh;

void func_002368B0(Swoosh *arg0) {
    f32 temp_f1;
    u8 temp_v0;
    u8 temp_v0_2;
    SwooshOwner *temp_v1;
    SwooshOwner *temp_v1_2;

    temp_v1 = arg0->owner;
    if (temp_v1 != NULL) {
        if (arg0->flags & 1) {
            temp_v0 = temp_v1->activeSwooshes;
            if (temp_v0 != 0) {
                temp_v1->activeSwooshes = temp_v0 - 1;
            }
        }
        if (arg0->flags & 2) {
            temp_v1_2 = arg0->owner;
            temp_v0_2 = temp_v1_2->activeSwooshes2;
            if (temp_v0_2 != 0) {
                temp_v1_2->activeSwooshes2 = temp_v0_2 - 1;
            }
        }
    }
    arg0->owner = NULL;
    temp_f1 = arg0->point->y;
    if (arg0->top < temp_f1) {
        arg0->top = temp_f1;
    }
}
