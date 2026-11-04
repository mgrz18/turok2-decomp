#include "common.h"
#include "structs/game.h"
#include "structs/game_object.h"
#include "structs/player.h"


extern CGameObjectInstance *D_800F1CE0;

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
        if (kind == 1 && D_80119870.m_Settings.multiplayer && D_800F1CE0->m_nTypeFlag == 0x64E
            && D_800F1CE0->m_AI.m_ModeBefore != D_800F1CE0->m_AI.m_Mode) {
            opts = p->m_pOptions;
            mode = opts->m_TagMode;
            if (mode == kind) {
                opts->m_TagMode = 0;
                p->m_pOptions->m_LastTagMode = mode;
                func_002540E4(p);
                D_80119870.m_Settings.match.tagPhase = mode;
            }
        }
    }
    return 0;
}
