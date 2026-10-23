/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/80443868_2.CPP, func_80443844_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;

typedef struct PlayerOptions {
    char pad0[0x3E];
    u8 team;                   /* 0x3E (Rage Wars 0x8F) */
} PlayerOptions;

typedef struct Player {
    char pad0[0x510];
    s32 waiting;               /* 0x510 (Rage Wars 0x5D0) */
    char pad514[0x518 - 0x514];
    PlayerOptions *info;       /* 0x518 (Rage Wars 0x5D8) */
} Player;

typedef struct PlayerRecAt1C {
    char pad0[0x1C];
    Player *actor;             /* 0x1C */
} PlayerRecAt1C;

typedef struct RenderCounters {
    char pad0[0x34];
    s32 option34;              /* 0x34 (Rage Wars 0x54) */
    char pad38[0x40 - 0x38];
    s32 unk40;                 /* 0x40 (Rage Wars 0x74) */
} RenderCounters;

typedef struct Shared_Game {
    char pad0[0x5D8];
    RenderCounters counters;   /* 0x5D8 */
} Shared_Game;

extern Shared_Game D_8011A6D8;
extern int D_801198A0;
extern char D_800F6CB0[];
extern void func_00430330(char *);

int func_0041CA00(int unused, PlayerRecAt1C *player) {
    RenderCounters *sync = &D_8011A6D8.counters;
    int team;
    if (sync->option34 != 0) {
        team = player->actor->info->team;
        if (team == 1) {
            sync->unk40 = team;
        }
    }
    if (D_801198A0 == 1) {
        func_00430330(D_800F6CB0);
    } else {
        player->actor->waiting = 0;
    }
    return 1;
}
