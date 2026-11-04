/* (&D_80119870.m_Settings)->unlockedCheats instead of D_80119870.m_Settings.unlockedCheats gives the ROM's register choice (found
 * with the permuter). */
#include "common.h"
#include "structs/game_object.h"
#include "structs/game.h"

/* The parts of the player this reads (from func_002532A8). */
typedef struct {
    /* 0x000 */ u8 pad000[0x9D0];
    /* 0x9D0 */ s32 pieces[5];
    /* 0x9E4 */ u8 pad9E4[0xC];
    /* 0x9F0 */ s32 unk9F0;
} PlayerPieces;


/* The object this callback gets (only +0xF8 is used). */
typedef struct {
    /* 0x00 */ u8 pad00[0xF8];
    /* 0xF8 */ void *held;
} Holder;

extern void func_002535E4(CPlayerList *list, f32 arg1);
extern PlayerPieces *func_002532A8(CPlayerList *list);
extern void func_00412438(s32 id);


void func_00407CB0(s32 arg0, Holder *obj) {
    PlayerPieces *player;
    GameSettings *flags;

    obj->held = NULL;
    func_002535E4(&D_80119870.m_PlayerList, 900.0f);
    player = func_002532A8(&D_80119870.m_PlayerList);
    player->unk9F0 = 1;
    if (player->pieces[0] && player->pieces[1] && player->pieces[2] && player->pieces[3] && player->pieces[4]) {
        func_00412438(0x172);
        (&D_80119870.m_Settings)->unlockedCheats |= 0x7FF0019;
    } else {
        func_00412438(0x173);
        flags = &D_8011AAD8;
        flags->unlockedCheats |= 2;
    }
}
