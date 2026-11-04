#include "common.h"
#include "structs/vector.h"
#include "structs/player.h"
#include "structs/controller.h"

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
    /* 0x00 */ CPlayer *head;
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

void func_0024D344(CPlayer *p);
void func_0024CF58(CPlayer *p, CPlayer *other);
void func_00401B30(void *, s32 region, f32 yaw, CVector3 pos);
void func_00246BC0(void *input, CPlayer *p);
s32 func_00285558(ControllerProfile *c);
s32 func_00284174(void);
void func_002840EC(void);
s32 func_00416AF8(Globals *g);
void func_00275A50(void);
void func_00275904(s32);
void func_00275A98(void);
void func_00416644(void *pool, void *screen, CPlayer *p, ControllerProfile *c, s32);
s32 func_0026EFB8(s32 level);
s32 Cinema_PathPlaying(void);
s32 func_00284408(void);
s32 func_00284240(void);
void func_0042DC18(CPlayer *p, s32 character, s32);
void func_00243414(void *actor, void *ai, s32 mode);
void func_00248BA8(CPlayer *p, s32, s32, s32);
void CList__Remove(void *list, void *node);
void CList__AddHead(void *list, void *node);
void func_0042FFB0(void *pool, Camera *cam);
void func_0025492C(PlayerList *list);
void func_002525BC(PlayerList *list);

static __inline__ void reset_strokes(CBarrel *s) {
    s->m_State = 0;
    s->m_Turn = 0.0f;
    s->m_nCursor = -1;
    s->m_Open = 0.0f;
}

static __inline__ s32 in_cinema(CPlayer *p) {
    s32 result;

    if (D_8011ACA4) {
        return 1;
    }
    result = 0;
    if (p->m_pCamera != NULL && ((Camera *)p->m_pCamera)->cinema != NULL) {
        result = ((Camera *)p->m_pCamera)->cinema->script != &D_00439640;
    }
    return result;
}

/* Update the players: move each one in the game (unless a cinema holds the
 * view), handle pause (the death screen), the hub key screen and the level-end
 * cinema; let idle players join while the match allows it and send players
 * who left back to the idle list; then run the match clock and tag. */
void func_00251FC4(PlayerList *list) {
    CPlayer *p;
    CPlayer *next;
    f32 frameTime;
    u32 hub;
    s32 character;

    if (D_8011AC8C == 0xB || D_8011AC8C == 8) {
        return;
    }
    frameTime = D_800B6D28;
    for (p = list->active.head; p != NULL; p = p->m_pNext) {
        if (!in_cinema(p)) {
            func_0024D344(p);
        }
        func_0024CF58(p, p);
        func_00401B30(&D_8011AAA0, p->m_CharacterActor.header.m_pCurrentRegion, p->m_CharacterActor.m_RotY, p->m_CharacterActor.header.m_vPos);
    }
    for (p = list->active.head; p != NULL; p = p->m_pNext) {
        func_00246BC0(&p->m_ContState, p);
        if (func_00285558(p->m_ContState.m_pContState)) {
            if (func_00284174()) {
                func_002840EC();
            } else if (!func_00416AF8(&D_80119E2C) && p->m_Data.m_Health != 0 && p->m_pCamera != NULL
                       && ((Camera *)p->m_pCamera)->busy == 0 && (D_80119E2C.match.over == 0 || D_80119E2C.timed == 0)) {
                D_80119E2C.paused = 1;
                func_00275A50();
                func_00275904(300);
                func_00275A98();
                reset_strokes(&p->m_Barrel);
                p->m_LivesOverlay.m_Value = -1;
                p->m_TokensOverlay.m_Value = -1;
                func_00416644(&D_80119E2C, &D_00439640, p, (p->m_ContState.m_pContState), 0);
            }
        }
        if (p->m_bLevelNotCompleteExit) {
            hub = func_0026EFB8(D_80110048) - 1;
            if (hub < 6 && (p->m_Data.m_dwLevelFirstTimeFlags & (1 << hub))) {
                func_00416644(&D_80119E2C, &D_0043CAB4, p, (p->m_ContState.m_pContState), 0);
            }
            p->m_bLevelNotCompleteExit = 0;
        }
        if (!func_00416AF8(&D_80119E2C) && Cinema_PathPlaying() && func_00284408() == 400 && !list->cinema) {
            if (p->m_pCamera != NULL && ((Camera *)p->m_pCamera)->busy == 0) {
                list->cinema = 1;
                list->cinemaStarted = 0;
            }
        }
        if (list->cinema) {
            if (func_00284240() && !list->cinemaStarted) {
                list->cinemaStarted = 1;
                func_00416644(&((Camera *)p->m_pCamera)->cinema, &D_0043BED0, p, (p->m_ContState.m_pContState), 0);
            }
        }
    }
    for (p = list->idle.head; p != NULL; p = next) {
        MatchState *match;

        func_00246BC0(&p->m_ContState, p);
        next = p->m_pNext;
        match = &D_80119E2C.match;
        if (p != NULL && !match->over && (match->timeLeft >= 900.0f || match->timeLeft < 0.0f || !D_80119E2C.timed)
            && ((p->m_ContState.m_pContState->buttonsSingle & 0x1000) || p->m_bActive)) {
            p->m_bActive = 1;
            character = D_8011AB88[p->m_nController].character;
            p->m_pOptions->m_bActive = 1;
            func_0042DC18(p, character, 1);
            func_00243414(&p->m_WeaponActor, &p->m_WeaponActor.m_AI, 2);
            func_00248BA8(p, D_8011ACA8, 0, 1);
            CList__Remove(&list->idle, p);
            CList__AddHead(&list->active, p);
        }
    }
    for (p = list->active.head; p != NULL; p = next) {
        next = p->m_pNext;
        if (p != NULL && !p->m_bActive && list->active.count >= 2) {
            if (p->m_pCamera != NULL) {
                func_0042FFB0(&D_801198B0, ((Camera *)p->m_pCamera));
                p->m_pCamera = NULL;
            }
            CList__Remove(&list->active, p);
            CList__AddHead(&list->idle, p);
        }
    }
    func_0025492C(list);
    func_002525BC(list);
    D_800B6D28 = frameTime;
}
