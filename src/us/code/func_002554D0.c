#include "common.h"

typedef struct {
    /* 0x000 */ u8 kind;
    /* 0x001 */ u8 pad1[0xB7];
    /* 0x0B8 */ u16 type;
    /* 0x0BA */ u8 padBA[0xBA];
    /* 0x174 */ s8 unk174;
    /* 0x175 */ u8 pad175;
    /* 0x176 */ s8 unk176;
} Actor;

typedef struct {
    /* 0x00 */ u8 pad0[0x3E];
    /* 0x3E */ u8 tagMode;
    /* 0x3F */ u8 lastTagMode;
} Options;

typedef struct {
    /* 0x000 */ u8 pad0[0x518];
    /* 0x518 */ Options *options;
} Player;

/* Game state from the player list on; the multiplayer flag is at +0x1261. */
typedef struct {
    /* 0x0000 */ void *players;
    /* 0x0004 */ u8 pad4[0x1261 - 4];
    /* 0x1261 */ u8 multiplayer;
    /* 0x1262 */ u8 pad1262[0x1460 - 0x1262];
    /* 0x1460 */ s32 unk1460;
} GameState;

extern Actor *D_800F1CE0;
extern GameState D_80119890;

void func_002540E4(Player *p);

static __inline__ u8 ActorKind(Actor *actor) {
    return actor->kind;
}

/* In multiplayer, when actor 0x64E is a kind-1 actor whose 0x174/0x176 bytes
 * differ, ends the player's tag mode 1 and records it. */
s32 func_002554D0(Player *p) {
    Actor **slot;
    Actor *a;
    Options *opts;
    s32 kind;
    s32 mode;

    if (D_800F1CE0 != NULL) {
        slot = &D_800F1CE0; /* read through the slot: the ROM copies the pointer */
        a = *slot;
        kind = a->kind;
        if (kind == 1 && D_80119890.multiplayer && D_800F1CE0->type == 0x64E
            && D_800F1CE0->unk176 != D_800F1CE0->unk174) {
            opts = p->options;
            mode = opts->tagMode;
            if (mode == kind) {
                opts->tagMode = 0;
                p->options->lastTagMode = mode;
                func_002540E4(p);
                D_80119890.unk1460 = mode;
            }
        }
    }
    return 0;
}
