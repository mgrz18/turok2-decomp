/* CPlayer code: CPlayerPool__NewGame, 0x0042E5AC-0x0042E834 (splat had it inside the data D_0042D50C). */
#include "common.h"
#include "structs/player.h"
#include "structs/game.h"
#include "structs/controller.h"
#include "globals/game.h"
#include "protos/list.h"


typedef struct PlayerList {
    /* 0x00 */ CPlayer **handle;
    /* 0x04 */ CPlayer *players;
    /* 0x08 */ s32 count;
    /* 0x0C */ u8 list0[0x14];
    /* 0x20 */ u8 list1[0x14];
} PlayerList;

extern CPlayer **CCache__Allocate(s32 heap, s32 size, s32 tag, char *name);
extern void CCache__Deallocate(s32 heap, CPlayer **handle);
extern void CCache__Flush(s32);
extern void Controller_SetRumbleOptionAll(s32);
extern void memset(void *p, s32 c, sptr n); /* memset */
extern void CPlayerContState__Init(void *, ControllerProfile *);
extern void CPlayer__Reset(CPlayer *p);
extern void CPlayer__NewGame(CPlayer *p, s32 character, s32 arg2);

/* CPlayer__Construct, inlined. */
static __inline__ void initPlayer(CPlayer *p, s32 index, CPlayerOptions *options) {
    p->m_bActive = 0;
    CPlayerContState__Init(&p->m_ContState, &gControllers[index]);
    p->m_nController = index;
    p->m_pOptions = options;
    p->m_pLoopingSound_WeaponBackground = 0;
    p->m_pLoopingSound_WeaponFiring = 0;
    p->m_Data.m_CheckPointWarpID = 0;
    p->m_Data.m_SavePointWarpID = 0;
    p->m_bLevelNotCompleteExit = 0;
    CPlayer__Reset(p);
}

void CPlayerPool__NewGame(PlayerList *list, s32 count, s32 arg2) {
    GameSettings *flags;
    CPlayerOptions *options;
    CPlayer *p;
    s32 i;

    CList__Construct(list->list0, 0x131C, 0x1320);
    CList__Construct(list->list1, 0x131C, 0x1320);
    Controller_SetRumbleOptionAll(0);
    CCache__Flush(0);
    flags = &gEngineApp_m_Settings;
    if (list->handle != NULL) {
        CCache__Deallocate(0, list->handle);
        list->handle = NULL;
        list->players = NULL;
        list->count = 0;
    }
    if (count == 0) {
        return;
    }
    list->count = count;
    list->handle = CCache__Allocate(0, count * sizeof(CPlayer), 0x23, "Players");
    list->players = *list->handle;
    memset(list->players, 0, count * sizeof(CPlayer));
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
                CPlayer__NewGame(p, options->m_nCharacter, 1);
                CList__Remove(list->list0, p);
                CList__AddHead(list->list1, p);
            }
        }
    } else {
        p = list->players;
        initPlayer(p, options->m_nController, options);
        CPlayer__NewGame(p, options->m_nCharacter, arg2);
        CList__Remove(list->list0, p);
        CList__AddHead(list->list1, p);
    }
    gEngineApp_m_Settings.match.tagPhase = 1;
}
