/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/SOUND/8022C894_2.CPP, func_8022D040_de, with Turok 2's layouts. */
#include "common.h"

typedef struct {
    /* 0x000 */ u8 pad0[0x140];
    /* 0x140 */ s32 motionState;
} Body;

typedef struct {
    /* 0x000 */ u8 pad0[0xE2];
    /* 0x0E2 */ s8 animationDone;
    /* 0x0E3 */ u8 padE3[0xA08 - 0xE3];
    /* 0xA08 */ s16 pose;
    /* 0xA0A */ u8 padA0A[0xA10 - 0xA0A];
    /* 0xA10 */ f32 poseTimer;
    /* 0xA14 */ u8 padA14[0xA78 - 0xA14];
    /* 0xA78 */ f32 stick;
    /* 0xA7C */ u8 padA7C[0xA80 - 0xA7C];
    /* 0xA80 */ f32 landing;
    /* 0xA84 */ u8 padA84[0xC04 - 0xA84];
    /* 0xC04 */ s32 animation;
} Player;

extern char D_800ADF18;
extern char D_800ADF30;
extern void func_0024EF70(void *, void *, void *);
extern void func_0024F658(void *, void *, void *);
extern s32 func_0024E5F0(void *, void *, s32);

/* Landing state: pose 0xC or 0xD, movement tables, animation 0x8A2, 0x8A7
 * or 0x14 by the stick (unless animation 0x1144 is still running), then
 * state 3 once the pose timer passes 2 without body bit 0x800. */
void func_00255B50(Player *player, Body *body) {
    s32 busy;
    f32 x;

    if (player->landing == 0.0f) {
        player->pose = 0xC;
    } else {
        player->pose = 0xD;
    }
    func_0024EF70(player, body, &D_800ADF18);
    func_0024F658(player, body, &D_800ADF30);
    busy = 0;
    if (player->animation == 0x1144) {
        busy = player->animationDone == 0;
    }
    if (!busy) {
        x = player->stick;
        if (1.024f <= x) {
            player->animation = 0x8A2;
        } else if (x <= -1.024f) {
            player->animation = 0x8A7;
        } else {
            player->animation = 0x14;
        }
    }
    if (!(body->motionState & 0x800) && player->poseTimer > 2.0f) {
        func_0024E5F0(player, body, 3);
    }
}
