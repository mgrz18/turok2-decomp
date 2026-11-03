/* Player code: func_0042E5AC, 0x0042E5AC-0x0042E834 (splat had it inside the data D_0042D50C). */
#include "common.h"

typedef struct Options {
    /* 0x00 */ u8 pad0[0x28];
    /* 0x28 */ u8 active;
    /* 0x29 */ u8 pad29[0x2F - 0x29];
    /* 0x2F */ s8 controller;
    /* 0x30 */ s8 character;
    /* 0x31 */ u8 pad31[0x40 - 0x31];
} Options;

typedef struct GameFlags {
    /* 0x000 */ u8 pad0[0x19];
    /* 0x019 */ u8 multiplayer;     /* D_8011AAF1 */
    /* 0x01A */ u8 pad1A[0x40 - 0x1A];
    /* 0x040 */ Options options[5];
} GameFlags;

typedef struct Controller {
    u8 pad[0x224];
} Controller;

typedef struct Player {
    /* 0x0000 */ u8 pad0[0x510];
    /* 0x0510 */ s32 unk510;
    /* 0x0514 */ s32 index;
    /* 0x0518 */ Options *options;
    /* 0x051C */ u8 pad51C[0x92C - 0x51C];
    /* 0x092C */ s32 unk92C;
    /* 0x0930 */ s32 unk930;
    /* 0x0934 */ u8 pad934[0xA40 - 0x934];
    /* 0x0A40 */ u8 unkA40[0xC00 - 0xA40];
    /* 0x0C00 */ s32 unkC00;
    /* 0x0C04 */ u8 padC04[0x1288 - 0xC04];
    /* 0x1288 */ s32 sound0;
    /* 0x128C */ s32 sound1;
    /* 0x1290 */ u8 pad1290[0x1328 - 0x1290];
} Player;

typedef struct PlayerList {
    /* 0x00 */ Player **handle;
    /* 0x04 */ Player *players;
    /* 0x08 */ s32 count;
    /* 0x0C */ u8 list0[0x14];
    /* 0x20 */ u8 list1[0x14];
} PlayerList;

extern GameFlags D_8011AAD8;
extern s32 D_8011ACF0;
extern Controller D_800F5EC0[];
extern char D_800A7080[]; /* in the engine's .rodata */

extern void CList__Construct(void *list, s32 prevOffset, s32 nextOffset);
extern void CList__AddHead(void *list, Player *p);
extern void CList__AddTail(void *list, Player *p);
extern void CList__Remove(void *list, Player *p);
extern Player **func_00204EDC(s32 heap, s32 size, s32 tag, char *name);
extern void func_002052D8(s32 heap, Player **handle);
extern void func_002053A8(s32);
extern void func_00285410(s32);
extern void func_00266C5C(void *p, s32 c, s32 n); /* memset */
extern void func_002470E8(void *, Controller *);
extern void func_00248340(Player *p);
extern void func_0042DC18(Player *p, s32 character, s32 arg2);

/* func_0042E91C, inlined. */
static __inline__ void initPlayer(Player *p, s32 index, Options *options) {
    p->unk510 = 0;
    func_002470E8(p->unkA40, &D_800F5EC0[index]);
    p->index = index;
    p->options = options;
    p->sound0 = 0;
    p->sound1 = 0;
    p->unk92C = 0;
    p->unk930 = 0;
    p->unkC00 = 0;
    func_00248340(p);
}

void func_0042E5AC(PlayerList *list, s32 count, s32 arg2) {
    GameFlags *flags;
    Options *options;
    Player *p;
    s32 i;

    CList__Construct(list->list0, 0x131C, 0x1320);
    CList__Construct(list->list1, 0x131C, 0x1320);
    func_00285410(0);
    func_002053A8(0);
    flags = &D_8011AAD8;
    if (list->handle != NULL) {
        func_002052D8(0, list->handle);
        list->handle = NULL;
        list->players = NULL;
        list->count = 0;
    }
    if (count == 0) {
        return;
    }
    list->count = count;
    list->handle = func_00204EDC(0, count * sizeof(Player), 0x23, D_800A7080);
    list->players = *list->handle;
    func_00266C5C(list->players, 0, count * sizeof(Player));
    for (i = 0; i < list->count; i++) {
        CList__AddTail(list->list0, &list->players[i]);
    }
    options = flags->options;
    if (flags->multiplayer) {
        for (i = 0; i < list->count; i++) {
            p = &list->players[i];
            options = &flags->options[i + 1];
            initPlayer(p, i, options);
            if (options->active) {
                func_0042DC18(p, options->character, 1);
                CList__Remove(list->list0, p);
                CList__AddHead(list->list1, p);
            }
        }
    } else {
        p = list->players;
        initPlayer(p, options->controller, options);
        func_0042DC18(p, options->character, arg2);
        CList__Remove(list->list0, p);
        CList__AddHead(list->list1, p);
    }
    D_8011ACF0 = 1;
}
