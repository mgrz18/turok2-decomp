/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/MENU/TEAM/80217388_6.CPP, func_802181FC_de, with Turok 2's layouts. */

#include "common.h"
#include "structs/player.h"

typedef struct WeaponInfo {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s16 alternate;
} WeaponInfo;

typedef struct MenuSlot {
    /* 0x00 */ s32 weapon;
    /* 0x04 */ WeaponInfo *info;
    /* 0x08 */ s32 available;
    /* 0x0C */ f32 angle;
    /* 0x10 */ s32 lift;
    /* 0x14 */ f32 scale;
} MenuSlot;

typedef struct WeaponMenu {
    /* 0x000 */ u8 pad0[0x1C];
    /* 0x01C */ MenuSlot slots[36];
    /* 0x37C */ s32 cursor;
    /* 0x380 */ s32 ready;
    /* 0x384 */ s32 group;
    /* 0x388 */ u8 pad388[4];
    /* 0x38C */ s32 altLayout;
} WeaponMenu;

extern s32 D_800ADCC8[8];   /* layouts: group 1, group 0, flag 0x80; then the same for D_8011AAF1 */
extern s32 D_800ADCE8[8];
extern s32 D_800ADD08[8];
extern s32 D_800ADD28[8];
extern s32 D_800ADD48[8];
extern s32 D_800ADD68[8];
extern f32 D_800AE2D8;              /* slot scale */
extern WeaponInfo *D_800AF618[];
extern u8 D_8011AAF1;

s32 func_0024E700(CPlayer *player, s32 weapon);

/* Lays out the eight-slot weapon wheel from the layout table for :
 * each weapon becomes its alternate when the player owns that (and is not
 * holding it), the held weapon gets the cursor, and the slots are set
 * 45 degrees apart. */
void func_0024C2A0(WeaponMenu *menu, s32 group, CPlayer *player) {
    s32 *layout;
    s32 i;
    s32 w;
    s16 alt;
    s32 ok;

    if (group != 0) {
        layout = D_800ADCC8;
        menu->altLayout = 0;
        if (D_8011AAF1 != 0) {
            layout = D_800ADD28;
        }
    } else {
        layout = D_800ADCE8;
        menu->altLayout = 0;
        if (D_8011AAF1 != 0) {
            layout = D_800ADD48;
        }
    }
    if (player->m_CharacterActor.m_AI.m_dwFlags & 0x80) {
        layout = D_800ADD08;
        if (D_8011AAF1 != 0) {
            layout = D_800ADD68;
        }
        menu->altLayout = 1;
    }
    menu->group = group;
    menu->cursor = -1;
    for (i = 0; i < 8; i++) {
        w = layout[i];
        if (w == -1) {
            menu->slots[i].weapon = w;
            menu->slots[i].available = 0;
        } else {
            if (!(player->m_CharacterActor.m_AI.m_dwFlags & 0x80)) {
                alt = D_800AF618[w]->alternate;
                if (alt != -1 && player->m_Data.m_WeaponList[alt] != 0 && player->m_Data.m_CurrentWeapon != alt) {
                    w = alt;
                }
            }
            menu->slots[i].weapon = w;
            if (player->m_Data.m_CurrentWeapon == w) {
                menu->cursor = i;
            }
            menu->slots[i].info = D_800AF618[w];
            if (func_0024E700(player, w) == 0 || player->m_Data.m_WeaponList[w] == 0) {
                ok = 0;
            } else {
                ok = 1;
            }
            menu->slots[i].available = ok;
            menu->slots[i].lift = 0;
            menu->slots[i].scale = D_800AE2D8;
            menu->slots[i].angle = i * 0.785398245f;
        }
    }
    menu->ready = 1;
}
