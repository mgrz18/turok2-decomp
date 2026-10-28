/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/AUDIO_2/8025CCB0_2.CPP, func_8025CED0_de, with Turok 2's layouts. */
/* Start a random song of a group on a music player and return the request id (-1 if the group has none). */

#include "common.h"

typedef struct MusicBank {
    /* 0x0000 */ u8 pad0[0x2B60];
    /* 0x2B60 */ s32 table;
    /* 0x2B64 */ s32 count;
} MusicBank;

typedef struct MusicPlayer {
    /* 0x00 */ MusicBank *bank;
    /* 0x04 */ s32 request;
    /* 0x08 */ s32 state;
    /* 0x0C */ s32 group;
    /* 0x10 */ s32 song;
    /* 0x14 */ s32 speed;
    /* 0x18 */ s32 volume;
    /* 0x1C */ u8 pad1C[2];
    /* 0x1E */ s16 id;
} MusicPlayer;

extern s32 func_00266D8C(s32 table, s32 count, s32 group, s32 *first, s32 *last);
extern s32 func_002113A4(void);                     /* random */
extern void *func_002718F4(MusicBank *bank);        /* the bank's sound player */
extern void func_002714B0(MusicBank *bank);
extern void func_00271514(MusicBank *bank);
extern void func_00293E60(void *player, s32 id);    /* alSndpSetSound */
extern void func_00293EC0(void *player);            /* alSndpStop */

/* One song of the group, each with the same weight. */
static __inline__ s32 pick(MusicBank *bank, s32 group) {
    s32 first;
    s32 last;
    s32 i;
    s32 total;
    s32 roll;

    total = 0;
    if (func_00266D8C(bank->table, bank->count, group, &first, &last) == 0) {
        return -1;
    }
    if (first != last) {
        for (i = first; i <= last; i++) {
            total += 100;
        }
        roll = func_002113A4() % total;
        total = 0;
        for (i = first; i < last; i++) {
            total += 100;
            if (roll <= total) {
                return i;
            }
        }
        return i;
    }
    return first;
}

static __inline__ void restart(MusicPlayer *player) {
    void *channel;

    if (player->state != 2 && player->state != 0) {
        player->state = 2;
        channel = func_002718F4(player->bank);
        func_00293E60(channel, player->id);
        func_00293EC0(channel);
    }
}

s32 func_00276B10(MusicPlayer *player, s32 group, f32 scale) {
    s32 song;
    s32 request;

    song = pick(player->bank, (s16) group);
    if (song == -1) {
        return -1;
    }
    func_002718F4(player->bank);
    func_002714B0(player->bank);
    player->group = group;
    player->song = song;
    request = ++player->request;
    pick(player->bank, (s16) player->group);
    player->speed = scale * 32767.0f;
    player->volume = 0x40;
    if (player->state == 3 || player->state == 1) {
        restart(player);
    } else {
        player->state = 1;
    }
    func_00271514(player->bank);
    return request;
}
