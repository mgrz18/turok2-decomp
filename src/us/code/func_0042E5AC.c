/* CPlayer code: func_0042E5AC, 0x0042E5AC-0x0042E834 (splat had it inside the data D_0042D50C). */
#include "common.h"
#include "structs/player.h"
#include "structs/game.h"

typedef struct Controller {
    u8 pad[0x224];
} Controller;

typedef struct PlayerList {
    /* 0x00 */ CPlayer **handle;
    /* 0x04 */ CPlayer *players;
    /* 0x08 */ s32 count;
    /* 0x0C */ u8 list0[0x14];
    /* 0x20 */ u8 list1[0x14];
} PlayerList;

extern s32 D_8011ACF0;
extern Controller D_800F5EC0[];
extern char D_800A7080[]; /* in the engine's .rodata */

extern void CList__Construct(void *list, s32 prevOffset, s32 nextOffset);
extern void CList__AddHead(void *list, CPlayer *p);
extern void CList__AddTail(void *list, CPlayer *p);
extern void CList__Remove(void *list, CPlayer *p);
extern CPlayer **func_00204EDC(s32 heap, s32 size, s32 tag, char *name);
extern void func_002052D8(s32 heap, CPlayer **handle);
extern void func_002053A8(s32);
extern void func_00285410(s32);
extern void func_00266C5C(void *p, s32 c, s32 n); /* memset */
extern void func_002470E8(void *, Controller *);
extern void func_00248340(CPlayer *p);
extern void func_0042DC18(CPlayer *p, s32 character, s32 arg2);

/* func_0042E91C, inlined. */
static __inline__ void initPlayer(CPlayer *p, s32 index, CPlayerOptions *options) {
    p->m_bActive = 0;
    func_002470E8(&p->m_ContState, &D_800F5EC0[index]);
    p->m_nController = index;
    p->m_pOptions = options;
    p->m_pLoopingSound_WeaponBackground = 0;
    p->m_pLoopingSound_WeaponFiring = 0;
    p->m_Data.m_CheckPointWarpID = 0;
    p->m_Data.m_SavePointWarpID = 0;
    p->m_bLevelNotCompleteExit = 0;
    func_00248340(p);
}

void func_0042E5AC(PlayerList *list, s32 count, s32 arg2) {
    GameSettings *flags;
    CPlayerOptions *options;
    CPlayer *p;
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
    list->handle = func_00204EDC(0, count * sizeof(CPlayer), 0x23, D_800A7080);
    list->players = *list->handle;
    func_00266C5C(list->players, 0, count * sizeof(CPlayer));
    for (i = 0; i < list->count; i++) {
        CList__AddTail(list->list0, &list->players[i]);
    }
    options = &flags->defaultSlot;
    if (flags->multiplayer) {
        for (i = 0; i < list->count; i++) {
            p = &list->players[i];
            options = &flags->slots[i];
            initPlayer(p, i, options);
            if (options->m_bActive) {
                func_0042DC18(p, options->m_nCharacter, 1);
                CList__Remove(list->list0, p);
                CList__AddHead(list->list1, p);
            }
        }
    } else {
        p = list->players;
        initPlayer(p, options->m_nController, options);
        func_0042DC18(p, options->m_nCharacter, arg2);
        CList__Remove(list->list0, p);
        CList__AddHead(list->list1, p);
    }
    D_8011ACF0 = 1;
}
