/* NON-MATCHING: 205 of 210 words differ: the ROM keeps the 1.5f message time in $f20 (two pool loads) so the tag and kill branches are not cross-jumped; here they merge and the code shifts */
#include "common.h"

typedef struct {
    /* 0x00 */ u8 pad0[0x33];
    /* 0x33 */ char name[0xB];
    /* 0x3E */ u8 tagMode;
} PlayerOptions;

typedef struct {
    /* 0x0000 */ u8 pad0[0x518];
    /* 0x0518 */ PlayerOptions *options;
    /* 0x051C */ void *camera;
    /* 0x0520 */ u8 pad520[0x12B4 - 0x520];
    /* 0x12B4 */ s32 messageIndex;
    /* 0x12B8 */ char messages[4][25];
} Player;

typedef struct {
    /* 0x00 */ u8 pad0[0x65];
    /* 0x65 */ u8 part;             /* & 3: 1 for the head */
} HitPart;

typedef struct {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ HitPart *part;
} Hit;

typedef struct {
    /* 0x00 */ u8 pad0[0x19];
    /* 0x19 */ u8 multiplayer;
} Settings;

/* The global at D_801198B0: what this reads of it. */
typedef struct Game {
    /* 0x0000 */ u8 pad0[0x1228];
    /* 0x1228 */ Settings settings;    /* D_8011AAD8 */
} Game;

extern Game D_801198B0;
extern char D_800B53D0[];       /* "head shot!" */
extern char D_800B53FC[];       /* "tagged by " */
extern char D_800B5408[];       /* "you tagged " */
extern char D_800B5414[];       /* "killed by " */
extern char D_800B5420[];       /* "you killed " */
extern char D_800B542C[];       /* "you killed yourself" */

char *func_00288800(char *dst, char *src); /* strcpy */
void func_00281610(Game *game, void *camera, char *text, f32 time);

/* Shows a player's newest message for time, and moves on in its ring of four. */
static __inline__ void postMessage(Game *game, Player *p, f32 time) {
    func_00281610(game, p->camera, p->messages[p->messageIndex], time);
    p->messageIndex++;
    p->messageIndex %= 4;
}

/* The multiplayer kill (or tag) messages for victim and killer. */
void func_002478C8(Player *victim, Player *killer, Hit *hit) {
    Settings *settings = &D_801198B0.settings;

    if (!settings->multiplayer) {
        return;
    }
    if (victim == killer) {
        func_00288800(killer->messages[killer->messageIndex], D_800B542C);
    } else if (victim->options->tagMode) {
        func_00288800(victim->messages[victim->messageIndex], D_800B53FC);
        func_00288800(victim->messages[victim->messageIndex] + 10, killer->options->name);
        postMessage(&D_801198B0, victim, 1.5f);
        func_00288800(killer->messages[killer->messageIndex], D_800B5408);
        func_00288800(killer->messages[killer->messageIndex] + 11, victim->options->name);
    } else {
        func_00288800(victim->messages[victim->messageIndex], D_800B5414);
        func_00288800(victim->messages[victim->messageIndex] + 10, killer->options->name);
        postMessage(&D_801198B0, victim, 1.5f);
        if (hit->part != NULL && (hit->part->part & 3) == 1) {
            func_00288800(killer->messages[killer->messageIndex], D_800B53D0);
        } else {
            func_00288800(killer->messages[killer->messageIndex], D_800B5420);
            func_00288800(killer->messages[killer->messageIndex] + 11, victim->options->name);
        }
    }
    postMessage(&D_801198B0, killer, 1.5f);
}
