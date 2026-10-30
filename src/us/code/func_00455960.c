/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80443868_2.CPP, func_80443844_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;

/* Turok 2 layouts (Rage Wars offsets in comments). */
typedef struct PlayerOptions {
    u8 pad0[0x3E];
    u8 team;                     /* 0x3E (0x8F) */
} PlayerOptions;

typedef struct PlayerActor {
    u8 pad0[0x510];
    s32 waiting;                 /* 0x510 (0x5D0) */
    u8 pad514[4];
    PlayerOptions *options;      /* 0x518 (0x5D8) */
} PlayerActor;

typedef struct PlayerRecAt1C {
    u8 pad0[0x1C];
    PlayerActor *actor;          /* 0x1C */
} PlayerRecAt1C;

typedef struct RenderCounters {
    u8 pad0[0x34];
    s32 option34;                /* 0x34 (0x54) */
    u8 pad38[8];
    s32 unk40;                   /* 0x40 (0x74) */
} RenderCounters;

typedef struct Shared_Game {
    u8 pad0[0x5D8];
    RenderCounters counters;     /* 0x5D8 */
} Shared_Game;

typedef struct GameSession GameSession;

extern Shared_Game D_8011A6D8;
extern int D_801198A0;
extern GameSession D_800F6CB0;
extern void func_00430250(GameSession *);

int func_00455960(int unused, PlayerRecAt1C *player) {
    RenderCounters *sync = &D_8011A6D8.counters;
    int team;
    if (sync->option34 != 0) {
        team = player->actor->options->team;
        if (team == 1) {
            sync->unk40 = team;
        }
    }
    if (D_801198A0 == 1) {
        func_00430250(&D_800F6CB0);
    } else {
        player->actor->waiting = 0;
    }
    return 1;
}
