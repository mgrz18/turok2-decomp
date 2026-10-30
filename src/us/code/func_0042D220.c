/* WRONG CUT: REAL func_0042D220 size 0x1E8 (splat cut it into func_0042D220 + func_0042D330). */
#include "common.h"

typedef struct IntrusiveList {
    /* 0x00 */ u8 pad0[0x14];
} IntrusiveList;

typedef struct Particle {
    /* 0x000 */ u8 kind;
    /* 0x001 */ u8 pad1[0x13];
    /* 0x014 */ void *def;
    /* 0x018 */ u8 pad18[0x28];
    /* 0x040 */ s32 unk40;
    /* 0x044 */ u8 pad44[0x174];
    /* 0x1B8 */ s32 unk1B8;
    /* 0x1BC */ u8 pad1BC[4];
    /* 0x1C0 */ s32 unk1C0;
    /* 0x1C4 */ u8 pad1C4[0x14];
} Particle; /* 0x1D8 */

typedef struct ParticleSystem {
    /* 0x0000 */ Particle particles[128];
    /* 0xEC00 */ IntrusiveList free;
    /* 0xEC14 */ IntrusiveList active;
    /* 0xEC28 */ IntrusiveList lists[3];
    /* 0xEC64 */ s32 unkEC64;
} ParticleSystem;

/* One of the four default light directions. */
typedef struct DirLight {
    /* 0x00 */ s16 x;
    /* 0x02 */ s16 y;
    /* 0x04 */ s16 z;
    /* 0x06 */ s16 w;
    /* 0x08 */ s16 angle0;
    /* 0x0A */ s16 angle1;
    /* 0x0C */ u8 color[4];
} DirLight;

extern u8 D_800F1850[];
extern u8 D_800F18F0[];
extern DirLight D_800F1930[4];

extern void CList__Construct(IntrusiveList *list, s32 prevOffset, s32 nextOffset);
extern s32 CList__AddTail(IntrusiveList *list, void *node);
extern void func_0027BD10(void *def);
extern void func_00210518(void *, f32);

void func_0042D220(ParticleSystem *sys, s32 arg1) {
    s32 i;
    s32 j;
    DirLight *light;

    for (i = 0; i < 128; i++) {
        sys->particles[i].def = D_800F1850;
        sys->particles[i].kind = 2;
        sys->particles[i].unk40 = 0;
        sys->particles[i].unk1B8 = 0;
        sys->particles[i].unk1C0 = 0;
    }
    CList__Construct(&sys->free, 0x1C4, 0x1C8);
    for (i = 0; i < 128; i++) {
        CList__AddTail(&sys->free, &sys->particles[i]);
    }
    CList__Construct(&sys->active, 0x1CC, 0x1D0);
    for (i = 0; i < 3; i++) {
        CList__Construct(&sys->lists[i], 0x1C4, 0x1C8);
    }
    func_0027BD10(D_800F1850);
    for (j = 0; j < 4; j++) {
        light = &D_800F1930[j];
        switch (j) {
            case 0:
                light->x = -1;
                light->y = 1;
                light->angle0 = -0x8000;
                light->angle1 = -0x8000;
                break;
            case 1:
                light->x = 1;
                light->y = 1;
                light->angle0 = 0;
                light->angle1 = -0x8000;
                break;
            case 2:
                light->x = 1;
                light->y = -1;
                light->angle0 = 0;
                light->angle1 = 0;
                break;
            case 3:
                light->x = -1;
                light->y = -1;
                light->angle0 = -0x8000;
                light->angle1 = 0;
                break;
        }
        light->z = 0;
        light->w = 0;
        light->color[0] = 0xFF;
        light->color[1] = 0xFF;
        light->color[2] = 0xFF;
        light->color[3] = 0xFF;
    }
    func_00210518(D_800F18F0, 1.5707964f);
    sys->unkEC64 = arg1;
}
