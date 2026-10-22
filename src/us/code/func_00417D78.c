/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/MISC/8043DF84.CPP, func_8043E134_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Shared_PlayerSettingsRecord Shared_PlayerSettingsRecord;
struct Shared_PlayerSettingsRecord {
    char pad0[4];
    s16 score;
    char pad6[0x4C - 6];
    u8 weapons[0x16];
    char pad62[0x78 - 0x62];
    u8 enabled;
    char pad79[6];
    u8 slot;
    s8 character;
    u8 variant;
    char pad82[2];
    u8 name[13];
    u8 locked;
    char pad92[3];
    u8 active;
};
typedef struct RenderCounters {
    f32 ticks, seconds, minutes, hours;
    f32 countdown;
    f32 timeLimit;
    s32 mode;
    s32 elapsed;
    s32 state;
    s32 teams;
    s32 round;
    s32 scores[5];
    s32 counts[5];
    s32 option54;
    s32 option58;
    s32 unk5C;
    s32 unk60;
    s32 setupState;
    s32 restrictionActive;
    s32 mask;
    s32 teamColour;
    s32 unk74;
    s32 option78;
    s32 unk7C;
    s32 counter80;
    s32 unk84;
    s32 first, second;
    s32 kills;
    s32 unk94;
    s32 enabled;
    s32 unk9C;
    s32 timer;
    s32 cooldown;
    s32 humanWon;
    s32 transitionActive;
} RenderCounters;
typedef struct Shared_GameSlot Shared_GameSlot;
struct Shared_GameSlot {
    u8 enabled;
    u8 unknown01[6];
    u8 slot;
    s8 kind;
    u8 unknown09[0x10];
    u8 controller;
    u8 unknown1A[2];
    u8 role;
    u8 escort;
    u8 secondListMode;
    u8 unknown1F[0x77];
};
typedef struct Shared_Game Shared_Game;
struct Shared_Game {
    u32 flags;
    u8 unknown04[4];
    u32 sceneMask;
    u8 unknown0C;
    u8 controllerMode;
    u8 unknown0E[2];
    s32 buttons;
    u16 packedOptions;
    u8 hudFade;
    u8 unknown17;
    u8 volume[3];
    u8 firstListMode;
    u8 unknown1C;
    u8 local;
    u8 mode1E;
    u8 unknown1F;
    union {
        struct {
            f32 speedFactor;
            s8 laps;
            s8 players;
            s8 trialOther;
            s8 trialScore;
            u8 unknown28[0x120];
            Shared_GameSlot slots[8];
            u8 unknown5F8[0x34];
            s32 unk62C;
            u8 unknown630[0x40];
            s32 split;
            u32 unknown674[3];
            s32 humanWon;
            s32 threePlayerLayout;
        };
        struct {
            u8 reserved20[0xB0];
            Shared_PlayerSettingsRecord playerSettings[8];
            u8 reserved580;
            u8 language;
            u8 reserved582[0x32];
            s32 state;
            u8 reserved5B8[0x20];
            RenderCounters counters;
        };
    };
    u8 reserved688[0xCAC - 0x688];
};
extern Shared_Game D_8011A6D8;
extern f32 D_801308FC;
extern s32 func_00417D78(void);
extern f32 D_800B6D28;
void func_002534E0(void *arg0);
s32 func_00417D78(void) {
    f32 time;
    RenderCounters *base;
    time = D_801308FC + D_800B6D28;
    D_801308FC = time;
    if ((120.0f) <= time) {
        base = &D_8011A6D8.counters;
        base->teams = 0;
        func_002534E0((void *) ((char *) base - 0x1440));
        return 1;
    }
    return 0;
}
