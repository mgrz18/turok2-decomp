#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x3E];
    /* 0x3E */ u8 tagMode;
    /* 0x3F */ u8 tagModeSaved;
} PlayerOptions;

typedef struct {
    /* 0x000 */ u8 pad0[0x518];
    /* 0x518 */ PlayerOptions *options;
} Player;

typedef struct {
    /* 0x0000 */ u8 pad0[0x1480];
    /* 0x1480 */ s32 unk1480;
} Game;

typedef struct {
    /* 0x00 */ u32 flags;
} ActionInfo;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ ActionInfo info;
} ActorDef;

typedef struct {
    /* 0x000 */ u8 pad0[0x14];
    /* 0x014 */ ActorDef *def;
    /* 0x018 */ u8 pad18[0xA0];
    /* 0x0B8 */ u16 typeId;
    /* 0x0BA */ u8 padBA[0x1A];
    /* 0x0D4 */ u32 flags;
} Actor;

typedef struct {
    /* 0x000 */ u8 pad0[0x34];
    /* 0x034 */ s8 unk34;
    /* 0x035 */ u8 pad35;
    /* 0x036 */ s8 unk36;
    /* 0x037 */ u8 pad37[0x90];
    /* 0x0C7 */ s8 active;
    /* 0x0C8 */ u8 padC8[0x48];
    /* 0x110 */ s32 done;
    /* 0x114 */ s32 tagCleared;
} ActionState;

extern Game D_80119870;
extern Player *func_002532B4(Game *game);
extern void func_00243414(Actor *actor, ActionState *state, s32 mode);

void func_00266380(Actor *actor, ActionState *state) {
    ActionInfo *info = &actor->def->info;
    Game *game;
    Player *player;
    s32 mode;

    if (actor->typeId == 0x64E && state->unk36 != state->unk34) {
        game = &D_80119870;
        player = func_002532B4(game);
        if (player != NULL && state->tagCleared == 0) {
            mode = player->options->tagMode;
            if (mode == 1) {
                player->options->tagMode = 0;
                player->options->tagModeSaved = mode;
                game->unk1480 = mode;
                state->tagCleared = mode;
            }
        }
    }
    if (state->active != 0) {
        if (info->flags & 2) {
            actor->flags &= ~0x2000;
            actor->flags &= ~0x100;
        }
        if (state->done != 0) {
            func_00243414(actor, state, 0);
        }
        if (actor->typeId == 0x64C) {
            func_00243414(actor, state, 2);
        }
    }
}
