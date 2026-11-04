#include "common.h"
#include "structs/game_object.h"
#include "structs/player.h"


/* Game state from the player list on; the multiplayer flag is at +0x1261. */
typedef struct {
    /* 0x0000 */ void *players;
    /* 0x0004 */ u8 pad4[0x1261 - 4];
    /* 0x1261 */ u8 multiplayer;
    /* 0x1262 */ u8 pad1262[0x1460 - 0x1262];
    /* 0x1460 */ s32 unk1460;
} GameState;

extern CGameObjectInstance *D_800F1CE0;
extern GameState D_80119890;

void func_002540E4(CPlayer *p);

static __inline__ u8 ActorKind(CGameObjectInstance *actor) {
    return actor->header.m_Type;
}

/* In multiplayer, when actor 0x64E is a kind-1 actor whose 0x174/0x176 bytes
 * differ, ends the player's tag mode 1 and records it. */
s32 func_002554D0(CPlayer *p) {
    CGameObjectInstance **slot;
    CGameObjectInstance *a;
    CPlayerOptions *opts;
    s32 kind;
    s32 mode;

    if (D_800F1CE0 != NULL) {
        slot = &D_800F1CE0; /* read through the slot: the ROM copies the pointer */
        a = *slot;
        kind = a->header.m_Type;
        if (kind == 1 && D_80119890.multiplayer && D_800F1CE0->m_nTypeFlag == 0x64E
            && D_800F1CE0->m_AI.m_ModeBefore != D_800F1CE0->m_AI.m_Mode) {
            opts = p->m_pOptions;
            mode = opts->m_TagMode;
            if (mode == kind) {
                opts->m_TagMode = 0;
                p->m_pOptions->m_LastTagMode = mode;
                func_002540E4(p);
                D_80119890.unk1460 = mode;
            }
        }
    }
    return 0;
}
