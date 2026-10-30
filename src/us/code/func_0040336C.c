#include "common.h"

typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 flags;
} Sub0040336C;

typedef struct {
    /* 0x0000 */ u8 pad0[0x1268];
    /* 0x1268 */ Sub0040336C sub;
} Game0040336C;

typedef struct {
    /* 0x000 */ u8 pad0[0x9E8];
    /* 0x9E8 */ s32 unk9E8;
} Player0040336C;

typedef struct {
    /* 0x00 */ u8 pad0[0xD4];
    /* 0xD4 */ s32 flags;
} Obj0040336C;

typedef struct {
    /* 0x00 */ u8 pad0[0xF8];
    /* 0xF8 */ s32 unkF8;
} Item0040336C;

extern Game0040336C D_80119870;
void func_002535E4(Game0040336C *game, f32 value);
s32 func_00284204(void);
Player0040336C *func_002532A8(Game0040336C *game);
void func_00248BA8(Player0040336C *player, s32 a1, s32 a2, s32 a3);

void func_0040336C(Obj0040336C *obj, Item0040336C *item) {
    Game0040336C *game = &D_80119870;
    Player0040336C *player;

    func_002535E4(game, 900.0f);
    item->unkF8 = 0;
    obj->flags &= ~0x2000;
    if (func_00284204()) {
        player = func_002532A8(game);
        if (player != NULL) {
            Sub0040336C *sub = &game->sub;

            player->unk9E8 = 1;
            sub->flags |= 0x6000;
            func_00248BA8(player, 0x176F, 2, 0);
        }
    }
}
