#include "common.h"

typedef struct PlayerOptions {
    /* 0x00 */ u8 pad0[0x3E];
    /* 0x3E */ u8 tagMode;
} PlayerOptions;

typedef struct Player {
    /* 0x000 */ u8 pad0[0xB8];
    /* 0x0B8 */ u16 typeId;
    /* 0x0BA */ u8 padBA[0x518 - 0xBA];
    /* 0x518 */ PlayerOptions *options;
    /* 0x51C */ u8 pad51C[0x520 - 0x51C];
    /* 0x520 */ s32 mode;
    /* 0x524 */ s32 unk524;
    /* 0x528 */ u8 pad528[0x996 - 0x528];
    /* 0x996 */ s16 weapon;
    /* 0x998 */ u8 pad998[0xBF4 - 0x998];
    /* 0xBF4 */ s32 team;
    /* 0xBF8 */ u8 padBF8[0x1298 - 0xBF8];
    /* 0x1298 */ f32 health;
} Player;

typedef struct Actor {
    /* 0x000 */ u8 pad0[0x98];
    /* 0x098 */ s32 unk98;
    /* 0x09C */ u8 pad9C[0x110 - 0x9C];
    /* 0x110 */ u8 slots[1][0x18];     /* indexed by D_800B6D1C */
    /* 0x128 */ u8 pad128[0x151 - 0x128];
    /* 0x151 */ s8 unk151;
    /* 0x152 */ u8 pad152[0x1A8 - 0x152];
    /* 0x1A8 */ Player *owner;
} Actor;

typedef struct Hit {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s32 *unkC;
    /* 0x10 */ u8 pad10[0x18 - 0x10];
    /* 0x18 */ s32 amount;
} Hit;

typedef struct WeaponDef {
    /* 0x00 */ u8 pad0[4];
    /* 0x04 */ u16 id;
} WeaponDef;

typedef struct MatchSettings {
    /* 0x000 */ u8 pad0[0x19];
    /* 0x019 */ u8 messages;
    /* 0x01A */ u8 pad1A[0x1FC - 0x1A];
    /* 0x1FC */ s32 tagFlash;
} MatchSettings;

extern MatchSettings D_8011AAD8;
extern s32 D_800AB8B0;       /* screen flash mode */
extern f32 D_800AB8B4[3];    /* screen flash color */
extern f32 D_800AB8C0[2];    /* screen flash range */
extern s32 D_800B6D1C;
extern s32 D_800ADB5C;
extern WeaponDef *D_800AF618[];

s32 *func_002017D4(s32 arg0, s32 arg1);
s32 func_002548FC(Player *player);
void func_0021D928(Actor *actor, s32 arg1, void *hit);
void func_0021D7BC(Actor *actor, s32 arg1, void *hit);
void func_0020B258(s32 *arg0, s32 arg1, s32 arg2, void *arg3, s32 arg4, s32 arg5, s32 arg6);
void func_0020A33C(s32 *arg0, s32 arg1, s32 arg2, void *arg3, s32 arg4, s32 arg5, s32 arg6);
void func_0020B6A0(s32 *arg0, s32 arg1, s32 arg2, void *arg3, s32 arg4, s32 arg5, s32 arg6);

#define HIT_LIMIT(hit) (*func_002017D4(*(hit)->unkC, 2))
#define WEAPON_INDEX(player) (D_800AF618[(player)->weapon]->id - 0x5E0)

void func_00247FB0(Actor *actor, s32 arg1, Hit *hit) {
    Player *player = actor->owner;
    u8 *flags;
    f32 *color;
    s32 index;

    if (hit->amount >= HIT_LIMIT(hit)) {
        return;
    }
    flags = &D_8011AAD8.messages;
    if (*flags != 0) {
        if (player->options->tagMode != 0 && player->unk524 == 0) {
            return;
        }
        if (*(s32 *) (flags + (0x1FC - 0x19)) != 0) {
            D_800AB8B0 = 3;
            switch (player->team) {
                case 0:
                default:
                    color = D_800AB8B4;
                    color[0] = 0.0f;
                    color[1] = 0.0f;
                    color[2] = 192.0f;
                    break;
                case 1:
                    color = D_800AB8B4;
                    color[1] = 0.0f;
                    color[2] = 0.0f;
                    color[0] = 192.0f;
                    break;
            }
            D_800AB8C0[0] = 70.0f;
            D_800AB8C0[1] = 255.0f;
            func_0021D928(actor, arg1, hit);
            if (player->health > 0.0f && func_002548FC(player) == 0) {
                func_0020B258(hit->unkC, actor->unk98, 1, actor->slots[D_800B6D1C], 0, (s8) hit->amount, -1);
            }
            if (player->mode != 10 && player->unk524 != 0) {
                index = WEAPON_INDEX(player);
                if (index >= 0 && index < HIT_LIMIT(hit)) {
                    func_0020A33C(hit->unkC, actor->unk98, 1, actor->slots[D_800B6D1C], 0, index, actor->unk151);
                }
            }
            return;
        }
    }
    if (player->health > 0.0f && func_002548FC(player) == 0) {
        func_0021D928(actor, arg1, hit);
        func_0020B258(hit->unkC, actor->unk98, 1, actor->slots[D_800B6D1C], 0, (s8) hit->amount, -1);
    } else {
        func_0021D7BC(actor, arg1, hit);
    }
    if (player->mode != 10 && player->options->tagMode == 0 && player->unk524 != 0 && player->typeId != D_800ADB5C) {
        index = WEAPON_INDEX(player);
        if (index >= 0 && index < HIT_LIMIT(hit)) {
            func_0020B6A0(hit->unkC, actor->unk98, 1, actor->slots[D_800B6D1C], 0, WEAPON_INDEX(player), actor->unk151);
        }
    }
}
