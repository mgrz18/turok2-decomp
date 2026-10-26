#include "common.h"

/* Switches an animation player to its queued animation. With `blend` set and a
 * different animation queued, it loads that one and, if `previous` is given,
 * leaves the old state there (frozen on the old animation) to blend from;
 * otherwise, or if the load fails, it reloads the current animation. */

typedef struct {
    /* 0x00 */ s32 time;
    /* 0x04 */ s16 current;
    /* 0x06 */ s16 next;
    /* 0x08 */ u16 frames;
    /* 0x0A */ u8 fieldA;
    /* 0x0B */ u8 dirty;
    /* 0x0C */ s32 keys;
    /* 0x10 */ void *data;
} AnimPlayer;

typedef struct {
    u8 pad[2];
    u16 field2;
    u8 pad4[2];
    u16 frames;
} AnimHeader;

void *func_0020367C(s32, s32, s32, s32, void *, void *, void *, s32);
s32 func_0020185C(s32, s32, s32, s32 *);
s32 func_00219968(AnimPlayer *);
void *func_0021793C(AnimPlayer *);
AnimHeader *func_002017D4(void *, s32);
void func_00216A9C(void);

extern u8 D_800A5A10[];

static __inline__ s32 load_animation(AnimPlayer *player, s32 file, s32 base, s32 index) {
    s32 size;
    void *tree;

    if (index == -1) {
        player->data = NULL;
        return 0;
    }
    player->data = func_0020367C(0, func_0020185C(file, base, index, &size), size, 4, player, func_00216A9C, D_800A5A10, 0);
    if (!func_00219968(player)) {
        return 0;
    }
    tree = func_0021793C(player);
    player->frames = func_002017D4(tree, 0)->frames;
    player->current = index;
    player->keys = func_002017D4(func_002017D4(tree, 3), 0)->field2;
    return 1;
}

void func_00218B10(AnimPlayer *player, s32 file, s32 base, s32 blend, s32 *changed, AnimPlayer *previous) {
    s32 current = player->current;
    s32 next = player->next;
    AnimPlayer saved;

    if (next != current && blend) {
        player->dirty = 1;
        saved = *player;
        if (load_animation(player, file, base, next)) {
            if (previous != NULL) {
                *previous = saved;
                previous->current = current;
                previous->next = current;
                previous->dirty = 1;
            }
            player->current = next;
            player->next = next;
            player->time = 0;
            player->fieldA = 0;
            player->dirty = 1;
            if (changed != NULL) {
                *changed = 1;
            }
        } else {
            if (changed != NULL) {
                *changed = 0;
            }
            load_animation(player, file, base, current);
        }
    } else {
        if (changed != NULL) {
            *changed = 0;
        }
        load_animation(player, file, base, current);
    }
}
