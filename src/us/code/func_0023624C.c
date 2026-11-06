#include "common.h"

/* The object a swoosh is attached to: two counts of attached swooshes
 * (0x10F is CGameObjectInstance.m_ActiveSwooshes). */
typedef struct {
    /* 0x000 */ u8 pad0[0x10F];
    /* 0x10F */ u8 activeSwooshes;
    /* 0x110 */ u8 pad110[0x1B5 - 0x110];
    /* 0x1B5 */ u8 activeSwooshes2;
} SwooshOwner;

typedef struct {
    /* 0x0 */ u8 pad0[4];
    /* 0x4 */ f32 fadeTime;
} SwooshDef;

typedef struct Swoosh {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ struct Swoosh *next;
    /* 0x08 */ SwooshDef *def;
    /* 0x0C */ u8 padC[0x1C - 0xC];
    /* 0x1C */ SwooshOwner *owner;
    /* 0x20 */ u8 pad20[4];
    /* 0x24 */ f32 time;
    /* 0x28 */ u8 pad28[0x3C - 0x28];
    /* 0x3C */ s32 flags;           /* 1, 2: counted in the owner's first, second count */
} Swoosh;

typedef struct {
    /* 0x0000 */ u8 pad0[0x7520];
    /* 0x7520 */ Swoosh *swooshes;
} SwooshSystem;

/* Detaches every swoosh of owner arg1, letting each fade out. */
void Swoosh_DetachOwner(SwooshSystem *arg0, SwooshOwner *arg1) {
    f32 temp_f1;
    u8 temp_v0;
    u8 temp_v0_2;
    SwooshOwner *temp_a0;
    SwooshOwner *temp_a0_2;
    Swoosh *var_v1;

    var_v1 = arg0->swooshes;
    if (var_v1 != NULL) {
        do {
            temp_a0 = var_v1->owner;
            if (temp_a0 == arg1) {
                if (temp_a0 != NULL) {
                    if (var_v1->flags & 1) {
                        temp_v0 = temp_a0->activeSwooshes;
                        if (temp_v0 != 0) {
                            temp_a0->activeSwooshes = (u8) (temp_v0 - 1);
                        }
                    }
                    if (var_v1->flags & 2) {
                        temp_a0_2 = var_v1->owner;
                        temp_v0_2 = temp_a0_2->activeSwooshes2;
                        if (temp_v0_2 != 0) {
                            temp_a0_2->activeSwooshes2 = (u8) (temp_v0_2 - 1);
                        }
                    }
                }
                var_v1->owner = NULL;
                temp_f1 = var_v1->def->fadeTime;
                if (var_v1->time < temp_f1) {
                    var_v1->time = temp_f1;
                }
            }
            var_v1 = var_v1->next;
        } while (var_v1 != NULL);
    }
}
