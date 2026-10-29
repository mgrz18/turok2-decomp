#include "common.h"

typedef struct {
    f32 x, y, z;
} Vec3f;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ void *script;
} Cinema;

typedef struct {
    /* 0x000 */ u8 pad0[0x53C];
    /* 0x53C */ Cinema *cinema;
    /* 0x540 */ u8 pad540[0x54C - 0x540];
    /* 0x54C */ s32 busy;
} Camera;

typedef struct {
    /* 0x00 */ u8 pad0[0xB4];
    /* 0xB4 */ u32 buttons;
} Controller;

typedef struct {
    /* 0x00 */ u8 pad0[0x28];
    /* 0x28 */ s8 joined;
} Options;

typedef struct {
    /* 0x000 */ s32 count;
    /* 0x004 */ f32 a;
    /* 0x008 */ f32 b;
    /* 0x00C */ u8 padC[0x37C - 0xC];
    /* 0x37C */ s32 last;
} Strokes;

typedef struct Player {
    /* 0x0000 */ u8 pad0[4];
    /* 0x0004 */ Vec3f pos;
    /* 0x0010 */ s32 region;
    /* 0x0014 */ u8 pad14[0x50 - 0x14];
    /* 0x0050 */ f32 yaw;
    /* 0x0054 */ u8 pad54[0x288 - 0x54];
    /* 0x0288 */ u8 actor[0x3C8 - 0x288];
    /* 0x03C8 */ u8 ai[0x510 - 0x3C8];
    /* 0x0510 */ s32 joined;
    /* 0x0514 */ s32 port;
    /* 0x0518 */ Options *options;
    /* 0x051C */ Camera *camera;
    /* 0x0520 */ u8 pad520[4];
    /* 0x0524 */ s32 health;
    /* 0x0528 */ u8 pad528[0x9F8 - 0x528];
    /* 0x09F8 */ u32 hubKeys;
    /* 0x09FC */ u8 pad9FC[0xA40 - 0x9FC];
    /* 0x0A40 */ u8 input[0x10];
    /* 0x0A50 */ Controller *controller;
    /* 0x0A54 */ u8 padA54[0xC00 - 0xA54];
    /* 0x0C00 */ s32 gotKey;
    /* 0x0C04 */ u8 padC04[0xCD0 - 0xC04];
    /* 0x0CD0 */ Strokes strokes;
    /* 0x1050 */ u8 pad1050[0x1114 - 0x1050];
    /* 0x1114 */ s32 unk1114;
    /* 0x1118 */ u8 pad1118[0x1150 - 0x1118];
    /* 0x1150 */ s32 unk1150;
    /* 0x1154 */ u8 pad1154[0x1320 - 0x1154];
    /* 0x1320 */ struct Player *next;
} Player;

typedef struct {
    /* 0x00 */ Player *head;
    /* 0x04 */ u8 pad4[0x10 - 4];
    /* 0x10 */ u32 count;
} PlayerCList;

typedef struct {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ PlayerCList idle;        /* players not in the game */
    /* 0x20 */ PlayerCList active;      /* players in the game */
    /* 0x34 */ s32 cinema;
    /* 0x38 */ s32 cinemaStarted;
} PlayerList;

typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ f32 timeLeft;
    /* 0x18 */ s32 fragLimit;
    /* 0x1C */ s32 over;
} MatchState;

/* The globals from D_80119E2C: a menu pool first. */
typedef struct {
    /* 0x000 */ void *menus;            /* CMenuPool */
    /* 0x004 */ u8 pad4[0xCC5 - 4];
    /* 0xCC5 */ u8 timed;               /* D_8011AAF1 */
    /* 0xCC6 */ u8 padCC6[0xE78 - 0xCC6];
    /* 0xE78 */ s32 paused;             /* D_8011ACA4 */
    /* 0xE7C */ u8 padE7C[0xE84 - 0xE7C];
    /* 0xE84 */ MatchState match;       /* D_8011ACB0 */
} Globals;

typedef struct {
    /* 0x00 */ s8 character;
    /* 0x01 */ u8 pad1[0x40 - 1];
} PortSetup;

extern Globals D_80119E2C;
extern s32 D_8011ACA4;
extern s32 D_8011AC8C;          /* game state */
extern s32 D_8011ACA8;
extern s32 D_80110048;          /* level */
extern f32 D_800B6D28;          /* frame time */
extern PortSetup D_8011AB88[];
extern char D_8011AAA0;
extern char D_801198B0;
extern char D_00439640;         /* death screen */
extern char D_0043BED0;
extern char D_0043CAB4;         /* key screen */

void func_0024D344(Player *p);
void func_0024CF58(Player *p, Player *other);
void func_00401B30(void *, s32 region, f32 yaw, Vec3f pos);
void func_00246BC0(void *input, Player *p);
s32 func_00285558(Controller *c);
s32 func_00284174(void);
void func_002840EC(void);
s32 func_00416AF8(Globals *g);
void func_00275A50(void);
void func_00275904(s32);
void func_00275A98(void);
void func_00416644(void *pool, void *screen, Player *p, Controller *c, s32);
s32 func_0026EFB8(s32 level);
s32 Cinema_PathPlaying(void);
s32 func_00284408(void);
s32 func_00284240(void);
void func_0042DC18(Player *p, s32 character, s32);
void func_00243414(void *actor, void *ai, s32 mode);
void func_00248BA8(Player *p, s32, s32, s32);
void CList__Remove(void *list, void *node);
void CList__AddHead(void *list, void *node);
void func_0042FFB0(void *pool, Camera *cam);
void func_0025492C(PlayerList *list);
void func_002525BC(PlayerList *list);

static __inline__ void reset_strokes(Strokes *s) {
    s->count = 0;
    s->a = 0.0f;
    s->last = -1;
    s->b = 0.0f;
}

static __inline__ s32 in_cinema(Player *p) {
    s32 result;

    if (D_8011ACA4) {
        return 1;
    }
    result = 0;
    if (p->camera != NULL && p->camera->cinema != NULL) {
        result = p->camera->cinema->script != &D_00439640;
    }
    return result;
}

/* Update the players: move each one in the game (unless a cinema holds the
 * view), handle pause (the death screen), the hub key screen and the level-end
 * cinema; let idle players join while the match allows it and send players
 * who left back to the idle list; then run the match clock and tag. */
void func_00251FC4(PlayerList *list) {
    Player *p;
    Player *next;
    f32 frameTime;
    u32 hub;
    s32 character;

    if (D_8011AC8C == 0xB || D_8011AC8C == 8) {
        return;
    }
    frameTime = D_800B6D28;
    for (p = list->active.head; p != NULL; p = p->next) {
        if (!in_cinema(p)) {
            func_0024D344(p);
        }
        func_0024CF58(p, p);
        func_00401B30(&D_8011AAA0, p->region, p->yaw, p->pos);
    }
    for (p = list->active.head; p != NULL; p = p->next) {
        func_00246BC0(p->input, p);
        if (func_00285558(p->controller)) {
            if (func_00284174()) {
                func_002840EC();
            } else if (!func_00416AF8(&D_80119E2C) && p->health != 0 && p->camera != NULL
                       && p->camera->busy == 0 && (D_80119E2C.match.over == 0 || D_80119E2C.timed == 0)) {
                D_80119E2C.paused = 1;
                func_00275A50();
                func_00275904(300);
                func_00275A98();
                reset_strokes(&p->strokes);
                p->unk1114 = -1;
                p->unk1150 = -1;
                func_00416644(&D_80119E2C, &D_00439640, p, p->controller, 0);
            }
        }
        if (p->gotKey) {
            hub = func_0026EFB8(D_80110048) - 1;
            if (hub < 6 && (p->hubKeys & (1 << hub))) {
                func_00416644(&D_80119E2C, &D_0043CAB4, p, p->controller, 0);
            }
            p->gotKey = 0;
        }
        if (!func_00416AF8(&D_80119E2C) && Cinema_PathPlaying() && func_00284408() == 400 && !list->cinema) {
            if (p->camera != NULL && p->camera->busy == 0) {
                list->cinema = 1;
                list->cinemaStarted = 0;
            }
        }
        if (list->cinema) {
            if (func_00284240() && !list->cinemaStarted) {
                list->cinemaStarted = 1;
                func_00416644(&p->camera->cinema, &D_0043BED0, p, p->controller, 0);
            }
        }
    }
    for (p = list->idle.head; p != NULL; p = next) {
        MatchState *match;

        func_00246BC0(p->input, p);
        next = p->next;
        match = &D_80119E2C.match;
        if (p != NULL && !match->over && (match->timeLeft >= 900.0f || match->timeLeft < 0.0f || !D_80119E2C.timed)
            && ((p->controller->buttons & 0x1000) || p->joined)) {
            p->joined = 1;
            character = D_8011AB88[p->port].character;
            p->options->joined = 1;
            func_0042DC18(p, character, 1);
            func_00243414(p->actor, p->ai, 2);
            func_00248BA8(p, D_8011ACA8, 0, 1);
            CList__Remove(&list->idle, p);
            CList__AddHead(&list->active, p);
        }
    }
    for (p = list->active.head; p != NULL; p = next) {
        next = p->next;
        if (p != NULL && !p->joined && list->active.count >= 2) {
            if (p->camera != NULL) {
                func_0042FFB0(&D_801198B0, p->camera);
                p->camera = NULL;
            }
            CList__Remove(&list->active, p);
            CList__AddHead(&list->idle, p);
        }
    }
    func_0025492C(list);
    func_002525BC(list);
    D_800B6D28 = frameTime;
}
