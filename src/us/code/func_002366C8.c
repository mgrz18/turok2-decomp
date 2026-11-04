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

/* The owner of the swoosh lists (the world): the list at 0x7514, its head at 0x7520. */
typedef struct {
    /* 0x0000 */ u8 pad0[0x7514];
    /* 0x7514 */ u8 list[0xC];
    /* 0x7520 */ Swoosh *head;
} SwooshList;

s32 func_00233BD4(void *, void *);                  /* extern */
s32 func_0026D474(void *, void *);              /* extern */

void func_002366C8(SwooshList *arg0) {
    f32 temp_f1;
    u8 temp_v0;
    u8 temp_v0_2;
    Swoosh *temp_s1;
    SwooshOwner *temp_v1;
    SwooshOwner *temp_v1_2;
    Swoosh *var_s0;

    var_s0 = arg0->head;
    if (var_s0 != NULL) {
        do {
            temp_s1 = var_s0->next;
            if (func_00233BD4(arg0, var_s0) == 0) {
                temp_v1 = var_s0->owner;
                if (temp_v1 != NULL) {
                    if (var_s0->flags & 1) {
                        temp_v0 = temp_v1->activeSwooshes;
                        if (temp_v0 != 0) {
                            temp_v1->activeSwooshes = temp_v0 - 1;
                        }
                    }
                    if (var_s0->flags & 2) {
                        temp_v1_2 = var_s0->owner;
                        temp_v0_2 = temp_v1_2->activeSwooshes2;
                        if (temp_v0_2 != 0) {
                            temp_v1_2->activeSwooshes2 = temp_v0_2 - 1;
                        }
                    }
                }
                var_s0->owner = NULL;
                temp_f1 = var_s0->point->y;
                if (var_s0->top < temp_f1) {
                    var_s0->top = temp_f1;
                }
                func_0026D474(arg0->list, var_s0);
            }
            var_s0 = temp_s1;
        } while (var_s0 != NULL);
    }
}
