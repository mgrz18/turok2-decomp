/* NON-MATCHING: 159 of 166 words differ: right structure, but the counter/flag schedule at the top and the struct-copy and loop register choice differ (needs more work). */
/* WRONG CUT: REAL func_0042EF2C size 0x298 (splat cut it into func_0042EF2C + func_0042F0E4). */
#include "common.h"

/* The match configuration handed in by the menu. */
typedef struct MatchConfig {
    /* 0x00 */ u16 mode;
    /* 0x02 */ s16 playerCount;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 teams[4]; /* the team is the low byte */
    /* 0x10 */ s16 a[4];
    /* 0x18 */ s16 b[4];
    /* 0x20 */ s16 c[4];
} MatchConfig;

typedef struct Setting {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ MatchConfig *config;
    /* 0x08 */ s32 value;
    /* 0x0C */ s32 enabled;
    /* 0x10 */ s32 index;
    /* 0x14 */ void (*handler)();
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
} Setting; /* 0x20 */

typedef struct GameSetupPlayer {
    /* 0x00 */ u8 active;
    /* 0x01 */ u8 pad1[7];
    /* 0x08 */ u8 team;
    /* 0x09 */ u8 pad9[0x37];
} GameSetupPlayer; /* 0x40 */

typedef struct GameSetup {
    /* 0x000 */ s32 mode;
    /* 0x004 */ u8 pad4[0x13];
    /* 0x017 */ u8 unk17;
    /* 0x018 */ u8 pad18;
    /* 0x019 */ u8 multiplayer;
    /* 0x01A */ u8 playerCount;
    /* 0x01B */ u8 pad1B[8];
    /* 0x023 */ u8 unk23;
    /* 0x024 */ u8 pad24[4];
    /* 0x028 */ u8 unk28;
    /* 0x029 */ u8 pad29[4];
    /* 0x02D */ u8 unk2D;
    /* 0x02E */ u8 pad2E[0x3A];
    /* 0x068 */ u8 singlePlayer;
    /* 0x069 */ u8 pad69[0x3F];
    /* 0x0A8 */ GameSetupPlayer players[4];
} GameSetup;

typedef struct GameSetupCopy {
    s32 data[0x184 / 4];
} GameSetupCopy;

typedef struct MatchState {
    /* 0x000 */ s32 unk0;
    /* 0x004 */ s32 unk4;
    /* 0x008 */ s32 unk8;
    /* 0x00C */ s32 unkC;
    /* 0x010 */ s32 unk10;
    /* 0x014 */ Setting a[4];
    /* 0x094 */ Setting b[4];
    /* 0x114 */ Setting c[4];
    /* 0x194 */ Setting d;
    /* 0x1B4 */ s32 playerCount;
    /* 0x1B8 */ s32 unk1B8;
    /* 0x1BC */ s32 unk1BC;
    /* 0x1C0 */ s32 unk1C0;
    /* 0x1C4 */ s32 unk1C4;
    /* 0x1C8 */ s32 saved;
    /* 0x1CC */ GameSetupCopy savedSetup;
} MatchState;

extern MatchState D_800F56B0;
extern s32 D_800B6D60;
extern GameSetup D_8011AAD8;
extern u32 D_8011ACA8;

extern void func_0042ECF0();
extern void func_0042F75C();
extern void func_0042F7C8();
extern void func_0041BA50(GameSetup *setup);
extern s32 func_002113A4(void);

/* Starts a match from config: sets up the per-player settings and the game setup. */
void func_0042EF2C(MatchConfig *config) {
    MatchState *s = &D_800F56B0;
    GameSetup *setup;
    s32 i;
    s32 r;

    D_800B6D60++;
    s->unk4 = 0;
    s->unk8 = 1;
    s->unk10 = 0;
    s->unkC = 1;
    if (s->saved == 0) {
        s->saved = 1;
        s->savedSetup = *(GameSetupCopy *)&D_8011AAD8;
    }
    for (i = 0; i < 4; i++) {
        D_8011AAD8.singlePlayer = 0;
        D_8011AAD8.players[i].active = 0;
        s->a[i].config = config;
        s->a[i].index = i;
        s->a[i].enabled = 1;
        s->a[i].handler = func_0042ECF0;
        s->a[i].unk18 = 0;
        s->a[i].unk1C = 0;
        s->a[i].value = config->a[i];
        s->b[i].config = config;
        s->b[i].index = i;
        s->b[i].enabled = 1;
        s->b[i].handler = func_0042F75C;
        s->b[i].unk18 = 0;
        s->b[i].unk1C = 0;
        s->b[i].value = config->b[i];
        s->c[i].index = i;
        s->c[i].config = config;
        s->c[i].enabled = 1;
        s->c[i].handler = func_0042F75C;
        s->c[i].unk18 = 0;
        s->c[i].unk1C = 0;
        s->c[i].value = config->c[i];
    }
    s->d.config = config;
    s->d.index = 0;
    s->d.enabled = 1;
    s->d.handler = func_0042F7C8;
    s->d.unk18 = 0;
    s->d.unk1C = 0;
    s->d.value = config->unk6;
    s->unk1B8 = 0;
    s->playerCount = config->playerCount;
    s->unk1C0 = 0;
    s->unk1C4 = 0;
    s->unk1BC = config->unk4;
    setup = &D_8011AAD8;
    D_8011ACA8 = config->mode;
    func_0041BA50(setup);
    if (s->playerCount < 2) {
        setup->singlePlayer = 1;
        setup->multiplayer = 0;
    } else {
        for (i = 0; i < s->playerCount; i++) {
            setup->players[i].active = 1;
            setup->players[i].team = config->teams[i];
        }
        D_8011AAD8.multiplayer = 1;
    }
    setup = &D_8011AAD8;
    setup->unk17 = 0;
    setup->mode = 12;
    setup->playerCount = s->playerCount;
    r = func_002113A4() % 9;
    setup->unk23 = r;
    setup->unk28 = r;
    setup->unk2D = r;
}
