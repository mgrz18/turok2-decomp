/* One source file from CBarrel__OpenWheel to CPlayer__Update (at least: the file
 * starts earlier, on a 16-byte boundary in .text, but this is the shortest
 * run whose .rodata starts 8-aligned). CPlayer__Update's pool
 * starts at 0x800A6CFC, 4 past an 8-byte boundary, and its 2^32 double is
 * 8-aligned only counted from 0x800A6C70, where CBarrel__OpenWheel's pool opens;
 * CPlayer__DrawHud's (0x800A6C74) and CPlayer__UpdateOrbitCamera's (0x800A6CDC) are 4 past a
 * boundary too, so neither can open the file.
 *
 * The parts were written apart; the first three use structs/player.h's
 * CPlayer, CPlayer__Update keeps its own Player view (see there), and they
 * share one WeaponInfo. CPlayer__Update is after Rage Wars (func_80220ED4_de). */
#include "common.h"
#include "structs/intelligence.h"
#include "structs/player.h"
#include "structs/camera.h"
#include "structs/game.h"
#include "structs/weapon.h"
#include "globals/scene.h"
#include "globals/render.h"

extern s32 gScene_m_PreviewState;
extern s32 gScene_m_PreviewTimer;

void CBarrel__Fill(CBarrel *menu, s32 rows, s32 *owned, s32 perRow, CPlayer *player);

/* Opens the weapon wheel: picks the weapons to show (owned, allowed here,
 * not superseded by an owned upgrade) and how many times round to repeat
 * them so that the slots are at most 20 degrees apart. */
void CBarrel__OpenWheel(CBarrel *menu, CPlayer *player) {
    s32 shown[0x23];
    s32 count = 0;
    s32 range = gScene_m_PreviewTimer - gScene_m_PreviewState + 1;
    s32 rows;
    s32 slots;
    s32 i;

    for (i = 0; i < 0x23; i++) {
        WeaponInfo *info = gWeaponInfoTable[i];
        s32 show;

        if (!player->m_Data.m_WeaponList[i]) {
            show = 0;
        } else if (!((player->m_WeaponActor.m_AI.m_dwFlags & 0x80) ? (info->flags & 2) : (info->flags & 1))) {
            show = 0;
        } else {
            s16 upgrade = info->alternate;

            show = 1;
            if (player->m_Data.m_WeaponList[upgrade]) {
                show = !(i < upgrade);
            }
        }
        shown[i] = show;
        if (show) {
            count++;
        }
    }
    rows = range / count;
    if (rows < 2) {
        rows = 1;
        if (count < range * 0.75f) {
            rows = 2;
        }
    }
    slots = rows * count;
    while (360 / slots > 20) {
        slots += count;
        rows++;
    }
    CBarrel__Fill(menu, rows, shown, count, player);
}

#include "gfx.h"
#include "structs/vector.h"
#include "structs/collision.h"
#include "structs/camera.h"

/* An animated HUD item (COverlay__Update ticks it, COverlay__StartScript restarts it). */
extern s32 gEngineApp_m_Settings_nextMode;          /* game state */
extern u8 gEngineApp_m_Settings_hudAlpha;           /* HUD alpha */
extern s32 gEngineApp_m_Settings_match_tag;
extern u8 gAmmoOverlayChangeScript[];
extern u8 gLivesOverlayChangeScript[];
extern u8 gTokensOverlayChangeScript[];
extern u8 gTotalKillsOverlayChangeScript[];
extern u8 gTotalTagsOverlayChangeScript[];

void COverlay__Update(COverlay *item);
void COverlay__StartScript(COverlay *item, u8 *anim);
void CPlayer__DrawTargetMarkers(CPlayer *player, CCamera *view);
void CPlayer__DrawScope(void *crosshair, CCamera *view);
void Hud_SetupTextureMode(s32 alpha);
void CBarrel__DrawEight(CBarrel *strokes, CCamera *view, CPlayer *player);
void CBarrel__DrawRing(CBarrel *strokes, CCamera *view, CPlayer *player);
s32 CPlayer__GetWeaponAmmo(CPlayer *player, s32 weapon);
s32 CPlayer__GetCurrentAmmoInfo(CPlayer *player);
void Hud_DrawSprite(s32 sprite, s32 frame, s16 x, s16 y, f32 sx, f32 sy, s32);
void Hud_DrawNumber(s32 value, f32 x, f32 y, f32 sx, f32 sy, s32, s32 rightAlign, s32 minDigits);
void func_00254768(CPlayer *player, CCamera *view);

/* The next display list command. Through this helper, as in the ROM, the
 * head is loaded and stored by symbol; a post-increment written in place
 * keeps the address of the head in a register instead. */
static __inline__ Gfx *dl_next(void) {
    return gDList++;
}

/* Draw a player's HUD: tick the item animations, the crosshair and strokes,
 * then the ammo (with the charge gauge of weapon 0xDD), the health, the
 * lives, the keys in single player, and the score and frags in multiplayer. */
void CPlayer__DrawHud(CPlayer *player, CCamera *view) {
    f32 sx;
    f32 sy;
    f32 shift;
    f32 x;
    f32 y;
    s32 ammo;
    s32 icon;
    s32 frame;
    s32 lives;
    s32 showAmmo;
    s32 value;
    CBarrel *strokes;

    showAmmo = 1;
    if (gEngineApp_m_Settings_nextMode == 0xB || gEngineApp_m_Settings_nextMode == 8) {
        return;
    }
    COverlay__Update(&player->m_AmmoOverlay);
    COverlay__Update(&player->m_HealthOverlay);
    COverlay__Update(&player->m_LivesOverlay);
    COverlay__Update(&player->m_TokensOverlay);
    COverlay__Update(&player->m_TotalKillsOverlay);
    COverlay__Update(&player->m_TotalTagsOverlay);
    CPlayer__DrawTargetMarkers(player, view);
    if (view->m_ViewParams.m_nType == 0) {
        CPlayer__DrawScope(&player->m_Overlay, view);
    }
    strokes = &player->m_Barrel;
    Hud_SetupTextureMode(0xFF);
    {   /* gDPSetTextureFilter(G_TF_BILERP) */
        Gfx *g = dl_next();

        g->w0 = 0xE3001201;
        g->w1 = 0x2000;
    }
    if (strokes->m_bStickWheel) {
        CBarrel__DrawEight(strokes, view, player);
    } else {
        CBarrel__DrawRing(strokes, view, player);
    }
    Hud_SetupTextureMode(gEngineApp_m_Settings_hudAlpha);
    sx = view->m_Viewport.m_Width / (f32)gScreenWidth;
    shift = sx * 32.0f;
    sy = view->m_Viewport.m_Height / (f32)gScreenHeight;

    ammo = CPlayer__GetWeaponAmmo(player, player->m_Data.m_CurrentWeapon);
    if (player->m_AmmoOverlay.m_Value != ammo) {
        COverlay__StartScript(&player->m_AmmoOverlay, gAmmoOverlayChangeScript);
        player->m_AmmoOverlay.m_Value = ammo;
    }
    icon = CPlayer__GetCurrentAmmoInfo(player);
    if (player->m_AmmoOverlay.m_bOnScreen && player->m_AmmoOverlay.m_Mode && icon != -1) {
        x = player->m_AmmoOverlay.m_PositionX * sx + view->m_Viewport.m_ScreenPosX;
        y = player->m_AmmoOverlay.m_PositionY * sy + view->m_Viewport.m_ScreenPosY + view->m_Viewport.m_Height;
        Hud_DrawSprite(0x12C, 0, x - shift + sx * 18.0f, y + sy, sx, sy, 1);
        frame = 0;
        if (icon == 0xDD) {
            frame = 8;
            if (!(player->m_WeaponActor.m_AI.m_Weapon.m_RefireDelay <= 0.0f)) {
                frame = (90.0f - player->m_WeaponActor.m_AI.m_Weapon.m_RefireDelay) * 0.08888889f;
            }
            showAmmo = 0;
        } else if (player->m_Data.m_CurrentWeapon == 0x15) {
            showAmmo = 0;
        }
        Hud_DrawSprite(icon, frame, x - shift, y - sy * 8.0f, sx, sy, 1);
        if (showAmmo) {
            Hud_DrawNumber(player->m_AmmoOverlay.m_Value, x - sx * 6.0f, y - sy * 4.0f, sx, sy, 1, 0, 0);
        }
    }

    x = player->m_HealthOverlay.m_PositionX * sx + view->m_Viewport.m_ScreenPosX;
    y = player->m_HealthOverlay.m_PositionY * sy + view->m_Viewport.m_ScreenPosY + view->m_Viewport.m_Height;
    Hud_DrawSprite(0x12C, 0, x - shift + sx * 21.0f, y + sy, sx, sy, 1);
    Hud_DrawSprite(0x1F7, 0, x - shift + sx, y - sy * 9.0f, sx, sy, 1);
    Hud_DrawNumber((player->m_Data.m_Health + 0xFF) >> 8, x - sx * 3.0f, y - sy * 4.0f, sx, sy, 1, 0, 0);

    lives = player->m_Data.m_LifeForce;
    if (player->m_TokensOverlay.m_Value != lives) {
        COverlay__StartScript(&player->m_TokensOverlay, gTokensOverlayChangeScript);
        player->m_TokensOverlay.m_Value = lives;
    }
    if (player->m_TokensOverlay.m_bOnScreen && player->m_TokensOverlay.m_Mode) {
        x = player->m_TokensOverlay.m_PositionX * sx + view->m_Viewport.m_ScreenPosX + view->m_Viewport.m_Width;
        y = player->m_TokensOverlay.m_PositionY * sy + view->m_Viewport.m_ScreenPosY;
        Hud_DrawSprite(0x12C, 1, x + sx * -42.0f, y + sy * 24.0f, sx, sy, 1);
        Hud_DrawSprite(0x1F5, 0, x, y + sy * 8.0f, sx, sy, 1);
        Hud_DrawNumber(player->m_Data.m_LifeForce, x + sx * -7.0f, y + sy * 20.0f, sx, sy, 1, 1, 0);
    }

    if (!gEngineApp_m_Settings.multiplayer) {
        value = player->m_Data.m_Lives;
        if (player->m_LivesOverlay.m_Value != value) {
            COverlay__StartScript(&player->m_LivesOverlay, gLivesOverlayChangeScript);
            player->m_LivesOverlay.m_Value = value;
        }
        if (player->m_LivesOverlay.m_bOnScreen && player->m_LivesOverlay.m_Mode) {
            x = player->m_LivesOverlay.m_PositionX * sx + view->m_Viewport.m_ScreenPosX;
            y = player->m_LivesOverlay.m_PositionY * sy + view->m_Viewport.m_ScreenPosY;
            Hud_DrawSprite(0x12C, 0, x + sx * 18.0f, y + sy * 28.0f, sx, sy, 1);
            Hud_DrawSprite(0x1F6, 0, x, y, sx, sy, 1);
            Hud_DrawNumber(player->m_Data.m_Lives, x + sx * 30.0f, y + sy * 24.0f, sx, sy, 1, 0, 0);
        }
    }
    if (gEngineApp_m_Settings.multiplayer) {
        value = player->m_pOptions->m_Kills;
        if (player->m_TotalKillsOverlay.m_Value != value) {
            COverlay__StartScript(&player->m_TotalKillsOverlay, gTotalKillsOverlayChangeScript);
            player->m_TotalKillsOverlay.m_Value = value;
        }
        if (player->m_TotalKillsOverlay.m_bOnScreen && player->m_TotalKillsOverlay.m_Mode) {
            x = player->m_TotalKillsOverlay.m_PositionX * sx + view->m_Viewport.m_ScreenPosX + view->m_Viewport.m_Width;
            y = player->m_TotalKillsOverlay.m_PositionY * sy + view->m_Viewport.m_ScreenPosY;
            Hud_DrawSprite(0x1FA, 0, x, y + sy * 8.0f, sx, sy, 1);
            Hud_DrawNumber(player->m_pOptions->m_Kills, x + sx * -7.0f, y + sy * 20.0f, sx, sy, 1, 1, 0);
        }
        if (gEngineApp_m_Settings_match_tag) {
            value = player->m_pOptions->m_Tags;
            if (player->m_TotalTagsOverlay.m_Value != value) {
                COverlay__StartScript(&player->m_TotalTagsOverlay, gTotalTagsOverlayChangeScript);
                player->m_TotalTagsOverlay.m_Value = value;
            }
            if (player->m_TotalTagsOverlay.m_bOnScreen && player->m_TotalTagsOverlay.m_Mode) {
                x = player->m_TotalTagsOverlay.m_PositionX * sx + view->m_Viewport.m_ScreenPosX + view->m_Viewport.m_Width;
                y = player->m_TotalTagsOverlay.m_PositionY * sy + view->m_Viewport.m_ScreenPosY;
                Hud_DrawSprite(0x1FD, 0, x, y + sy * 8.0f, sx, sy, 1);
                Hud_DrawNumber(player->m_pOptions->m_Tags, x + sx * -7.0f, y + sy * 20.0f, sx, sy, 1, 1, 0);
            }
        }
    }
    if (player->m_pOptions->m_TagMode == 1) {
        func_00254768(player, view);
    }
}

typedef struct Quad {
    s32 w[4];
} Quad;

typedef struct Orbit {
    /* 0x0 */ f32 angle;
    /* 0x4 */ f32 distance;
} Orbit;

#define MAX(a, b) ((a) < (b) ? (b) : (a))

extern f32 sOrbitCameraAngle;
extern f32 sOrbitCameraDistance;

f32 func_002982F0(f32);             /* sin */
f32 func_002974C0(f32);             /* cos */
void CCamera__SetViewParams(void *camera, s32 type, f32 a, f32 b, f32 x0, f32 y0, f32 x1, f32 y1, f32 z1,
                   f32 h, CQuaternion quad, struct CGameRegion *region, f32 c, f32 d);

/* Points the camera at : in mode 1 from the view's offsets, in
 * mode 2 orbiting it, the buttons turning the orbit and moving it in or out. */
void CPlayer__UpdateOrbitCamera(CPlayer *player, CGameObjectInstance *target) {
    void *camera;
    f32 angle;
    f32 x;

    camera = player->m_pCamera;
    if (camera == NULL) {
        return;
    }
    if (player->m_bActive == 1) {
        CCamera__SetViewParams(camera, 0, target->m_RotY, player->m_vHeadRotOffset.x + player->m_vHeadWobbleRotOffset.x - player->m_JumpAngle,
                      player->m_vHeadRotOffset.y + player->m_vHeadWobbleRotOffset.y, player->m_vHeadRotOffset.z + player->m_vHeadWobbleRotOffset.z,
                      target->header.m_vPos.x + player->m_vEyeOffset.x, target->header.m_vPos.y, target->header.m_vPos.z + player->m_vEyeOffset.z,
                      MAX(player->m_vEyeOffset.y + player->m_ActualCameraVertEyeOffset - player->m_DuckOffset, 40.96f), target->m_qGround, target->header.m_pCurrentRegion, player->m_ZoomBlend, 3.0f);
    } else if (player->m_bActive == 2) {
        if (player->m_ContState.m_dwButtons & 0x200) {
            sOrbitCameraAngle += 0.069813177f;
        } else if (player->m_ContState.m_dwButtons & 0x100) {
            sOrbitCameraAngle -= 0.069813177f;
        } else if (player->m_ContState.m_dwButtons & 0x800) {
            sOrbitCameraDistance -= 4.096f;
        } else if (player->m_ContState.m_dwButtons & 0x400) {
            sOrbitCameraDistance += 4.096f;
        }
        angle = target->m_RotY + sOrbitCameraAngle;
        x = target->header.m_vPos.x + sOrbitCameraDistance * func_002982F0(angle);
        CCamera__SetViewParams(camera, 1, angle, 0.0f, 0.0f, 0.0f, x, target->header.m_vPos.y + 81.92f,
                      target->header.m_vPos.z + sOrbitCameraDistance * func_002974C0(angle), 0.0f, target->m_qGround, target->header.m_pCurrentRegion, 0.0f, 3.0f);
    }
}


void WeaponActor_SwitchToRequested(void *a, void *b);

/* Picks the player's weapon set (0 on foot, 1 in states 10-11, 2 with
 * flag 0x8000); when it changes, remembers the weapon held in the old set
 * if it belongs there and queues the new set's weapon. */
void CPlayer__UpdateWeaponSet(CPlayer *player) {
    s32 keep;
    s16 weapon;

    player->m_LastFrameAllowedWeaponType = player->m_CurrentAllowedWeaponType;
    if ((u32) ((u16)player->m_Mode - 10) < 2) {
        player->m_CurrentAllowedWeaponType = 1;
    } else if (player->m_ModeFlags & 0x8000) {
        player->m_CurrentAllowedWeaponType = 2;
    } else {
        player->m_CurrentAllowedWeaponType = 0;
    }
    if (player->m_LastFrameAllowedWeaponType == player->m_CurrentAllowedWeaponType) {
        return;
    }
    weapon = player->m_Data.m_CurrentWeapon;
    switch (player->m_LastFrameAllowedWeaponType) {
    case 0:
        if (!(gWeaponInfoTable[weapon]->flags & 1)) {
            goto no;
        }
        keep = 1;
        break;
    case 1:
        if (!(gWeaponInfoTable[weapon]->flags & 2)) {
            goto no;
        }
        keep = 1;
        break;
    case 2:
        if (weapon != 19) {
            goto no;
        }
        keep = 1;
        break;
    default:
    no:
        keep = 0;
        break;
    }
    if (keep) {
        player->m_Data.m_LastWeapon[player->m_LastFrameAllowedWeaponType] = player->m_Data.m_CurrentWeapon;
    }
    player->m_RequestedWeapon = player->m_Data.m_LastWeapon[player->m_CurrentAllowedWeaponType];
    if (player->m_RequestedWeapon == 19) {
        WeaponActor_SwitchToRequested(&player->m_WeaponActor, &player->m_WeaponActor.m_AI);
    }
}

/* After AbuN64/ragewars-decomp (Turok: Rage Wars), src/WEAPONS/8021CD70_8.CPP, func_80220ED4_de, with Turok 2's layouts. */

typedef struct Model {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 flags;
} Model;

/* What the actor stands in (GameObject_GetRegionInfo). */
typedef struct Surface {
    /* 0x00 */ u8 pad0[0x44];
    /* 0x44 */ u32 flags;
    /* 0x48 */ u16 unk48;
    /* 0x4A */ u8 pad4A[2];
    /* 0x4C */ u16 item;
    /* 0x4E */ u8 pad4E[0x54 - 0x4E];
    /* 0x54 */ u16 unk54;
} Surface;

typedef struct Controller {
    /* 0x00 */ u8 pad0[8];
    /* 0x08 */ CVector3 pos;
} Controller;

typedef struct Input {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ Controller *controller;
    /* 0x14 */ u8 pad14[0x24 - 0x14];
    /* 0x24 */ u32 held;
    /* 0x28 */ u32 pressed;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
} Input;

typedef struct Voice {
    /* 0x00 */ u8 pad0[0x10];
    /* 0x10 */ s32 bank;
} Voice;

/* CPlayer (structs/player.h) as CPlayer__Update reads it, kept local: it uses
 * the player both as itself and as its m_CharacterActor (actor = p), and
 * several words are typed against the header (0x10 the region as a Model,
 * the controller's 0x08 as a position, the weapon actor's 0x3F4 as a voice,
 * u32 flags), which would take a cast at most uses. */
typedef struct Player {
    /* 0x0000 */ u8 pad0[4];
    /* 0x0004 */ CVector3 pos;
    /* 0x0010 */ Model *model;
    /* 0x0014 */ CIntelligence *def;
    /* 0x0018 */ u8 pad18[4];
    /* 0x001C */ f32 velY;
    /* 0x0020 */ u8 pad20[0x40 - 0x20];
    /* 0x0040 */ Quad unk40;
    /* 0x0050 */ f32 yaw;
    /* 0x0054 */ u8 pad54[0xD4 - 0x54];
    /* 0x00D4 */ u32 flags;
    /* 0x00D8 */ u8 padD8[0xDC - 0xD8];
    /* 0x00DC */ s16 anim;
    /* 0x00DE */ u8 padDE[0xE2 - 0xDE];
    /* 0x00E2 */ s8 animPending;
    /* 0x00E3 */ u8 padE3[0x140 - 0xE3];
    /* 0x0140 */ u32 aiFlags;          /* CAI */
    /* 0x0144 */ u8 pad144[0x1A8 - 0x144];
    /* 0x01A8 */ struct Player *owner;
    /* 0x01AC */ u8 pad1AC[0x288 - 0x1AC];
    /* 0x0288 */ u8 emitter[4];
    /* 0x028C */ CVector3 emitterPos;
    /* 0x0298 */ Model *emitterModel;
    /* 0x029C */ u8 pad29C[0x2C8 - 0x29C];
    /* 0x02C8 */ Quad emitter40;
    /* 0x02D8 */ f32 emitterYaw;
    /* 0x02DC */ u8 pad2DC[0x3C8 - 0x2DC];
    /* 0x03C8 */ u8 unk3C8[0x3F4 - 0x3C8];
    /* 0x03F4 */ Voice *voice;
    /* 0x03F8 */ u8 pad3F8[0x430 - 0x3F8];
    /* 0x0430 */ struct Player *unk430;
    /* 0x0434 */ u8 pad434[0x510 - 0x434];
    /* 0x0510 */ s32 control;
    /* 0x0514 */ u8 pad514[4];
    /* 0x0518 */ CPlayerOptions *options;
    /* 0x051C */ u8 pad51C[4];
    /* 0x0520 */ s32 character;
    /* 0x0524 */ s32 health;               /* 8.8 */
    /* 0x0528 */ u8 pad528[2];
    /* 0x052A */ s16 collide;
    /* 0x052C */ u8 pad52C[0x6A9 - 0x52C];
    /* 0x06A9 */ u8 unk6A9;
    /* 0x06AA */ u8 pad6AA;
    /* 0x06AB */ u8 unk6AB;
    /* 0x06AC */ u8 pad6AC[0x92C - 0x6AC];
    /* 0x092C */ s32 item;
    /* 0x0930 */ s32 lastItem;
    /* 0x0934 */ s16 ammo[31];
    /* 0x0972 */ s8 owned[36];
    /* 0x0996 */ s16 weapon;
    /* 0x0998 */ u8 pad998[0xA08 - 0x998];
    /* 0x0A08 */ s16 state;
    /* 0x0A0A */ u8 padA0A[2];
    /* 0x0A0C */ s16 prevState;
    /* 0x0A0E */ u8 padA0E[2];
    /* 0x0A10 */ f32 stateTime;
    /* 0x0A14 */ u32 prevStateFlags;
    /* 0x0A18 */ u8 padA18[4];
    /* 0x0A1C */ u32 stateFlags;
    /* 0x0A20 */ u8 padA20[4];
    /* 0x0A24 */ f32 step;
    /* 0x0A28 */ f32 unkA28;
    /* 0x0A2C */ u8 padA2C[4];
    /* 0x0A30 */ f32 unkA30;
    /* 0x0A34 */ u8 padA34[0xA40 - 0xA34];
    /* 0x0A40 */ Input input;
    /* 0x0A78 */ f32 velX;
    /* 0x0A7C */ f32 velZ;
    /* 0x0A80 */ f32 speed;
    /* 0x0A84 */ f32 lastVelY;
    /* 0x0A88 */ s32 onGround;
    /* 0x0A8C */ u8 padA8C[0xA9C - 0xA8C];
    /* 0x0A9C */ f32 airTime;
    /* 0x0AA0 */ CVector3 goal;
    /* 0x0AAC */ f32 unkAAC;
    /* 0x0AB0 */ u8 padAB0[0xAC0 - 0xAB0];
    /* 0x0AC0 */ f32 crouch;
    /* 0x0AC4 */ s32 crouchToggle;
    /* 0x0AC8 */ f32 unkAC8;
    /* 0x0ACC */ u8 padACC[0xAFC - 0xACC];
    /* 0x0AFC */ f32 unkAFC;
    /* 0x0B00 */ f32 unkB00;
    /* 0x0B04 */ u8 padB04[0xB14 - 0xB04];
    /* 0x0B14 */ s16 nextWeapon;
    /* 0x0B16 */ u8 padB16[2];
    /* 0x0B18 */ CVector3 lastPos;
    /* 0x0B24 */ u8 padB24[4];
    /* 0x0B28 */ f32 unkB28;
    /* 0x0B2C */ u8 padB2C[0xC04 - 0xB2C];
    /* 0x0C04 */ s32 action;
    /* 0x0C08 */ s32 lastAction;
    /* 0x0C0C */ u8 padC0C[0xCD0 - 0xC0C];
    /* 0x0CD0 */ s32 unkCD0;
    /* 0x0CD4 */ u8 padCD4[0x1050 - 0xCD4];
    /* 0x1050 */ s32 unk1050;
    /* 0x1054 */ u8 pad1054[0x1298 - 0x1054];
    /* 0x1298 */ f32 shake;
    /* 0x129C */ u8 pad129C[0x12A4 - 0x129C];
    /* 0x12A4 */ s32 unk12A4;
    /* 0x12A8 */ u8 pad12A8[4];
    /* 0x12AC */ s32 unk12AC;
} Player;

typedef struct PlayerStateInfo {
    /* 0x00 */ void (*enter)(Player *, void *);
    /* 0x04 */ void (*update)(Player *, Player *);
    /* 0x08 */ u32 *flags;
    /* 0x0C */ u8 padC[0x18 - 0xC];
} PlayerStateInfo;


extern f32 gFrameIncrement;              /* frame time */
extern s32 gEngineApp_m_Game_m_DebugLines;
extern s32 gEngineApp_m_Settings_match_tag;
extern s32 gEvenOdd;
extern f32 gCharacterHealthRegen[];           /* health regeneration per character */
extern PlayerStateInfo gPlayerStateTable[];
extern u32 D_800F1E30;
extern u32 D_800F1F10;
extern CCollisionResults gCollisionResults;  /* the last collision test (m_pInstance: what it hit) */
extern s32 gCollisionResults_m_nFloorKind;
extern s32 gScene_m_PendingWarpID;
extern s32 gCharacterModels[];
extern s32 gTagModeModel;

s32 Cinema_PathPlaying(void);
void CCache__SetFrameDelay(s32);
void Sound__SetMuted(s32 enable);
void AI_UpdateRegionFlags(Player *, u32 *ai);
void CGameObjectInstance__UpdateAnimation(Player *);
void AI_RideMovingPlatform(Player *, u32 *ai);
Surface *GameObject_GetRegionInfo(Player *);
void CPlayer__UpdateSpeedScale(Player *, Player *, Surface *);
f32 CGameRegion__GetCeilingHeight(Model *, f32 x, f32 z);
f32 CGameRegion__GetHeight(Model *, f32 x, f32 z);
void Sound__SetReverb(f32);
void CPlayer__EaseEyeOffset(Player *, Player *);
f32 func_00298470(f32);
s32 CPlayer__IsInMatchEndState(Player *);
f32 Random_Float(f32);
void DamageEvent_Set(DamageEvent *, s32, s32, f32, s32, s32);
void Player_OnMultiplayerKill(Player *, u32 *ai, DamageEvent *);
void Sound__Play(s32 sound, CVector3 pos, s32, s32);
s32 GameObject_IsOnGround(Player *);
void GameObject_PlayLandSound(Player *);
void CPlayer__CheckJumpPad(Player *, Player *);
void CPlayer__CheckWater(Player *, Player *);
void CPlayer__CheckState12Hit(Player *, Player *);
void CPlayer__CheckClimb(Player *, Player *);
void CPlayer__UpdateZoom(Player *);
void GameObject_ApplyCurrent(Player *, Surface *, CVector3 *, s32);
void CPlayer__Hurt(Player *, Player *, s32, s32, s32);
void CPlayer__UpdateWaterRegion(Player *, Player *, Surface *);
void CPlayer__UpdateFlashlight(Player *, Player *);
s32 Collision_MoveInstance(Player *, CVector3, u32 *);
void CGameRegion__FireTriggers(Model *, s32, Player *);
void CScene__CheckAutoActivation(void *, Player *, CVector3 *, CVector3 *);
void GameObject_FireTriggers(u8 *, s32, Player *);
void CPlayer__RamWithMount(Player *, Player *, u32 *);
s32 CPlayer__TryMount(Player *, Player *, u32 *);
void CPlayer__TouchPickup(Player *, u8 *);
void CPlayer__Dismount(Player *, Player *);
void CPlayer__Warp(Player *, s32, s32, s32);
f32 GameObject_GetRegionFloorHeight(Player *, s32);
void CScene__SetInstanceObjectType(void *, void *, s32, s32);
s32 CGameObjectInstance__SetAnimByAction(Player *, s32, s32);
s32 CGameObjectInstance__PickRandomAnim(Player *, s32, s32);
void CGameObjectInstance__SetAnim(Player *, s32, s32);
void func_0021A500(Player *);
s16 *CPlayer__GetFiringAmmo(Player *, s32);
void CBarrel__Update(s32 *, Player *, u8 *);
void Player_UpdateAim(Player *);

static __inline__ s32 link_enabled(u8 *bits) {
    if (gEngineApp_m_Settings.cheatFlags & 0x10) {
        return 1;
    }
    return *bits & 0xF;
}

static __inline__ s32 not_flagged(u32 flags, u32 bit) {
    return (flags & bit) == 0;
}

/* Entering a surface that changes the player's state (flags 0x40000): through
 * a 0x200000 surface only from one without the flag, otherwise only from
 * above the floor. */
static __inline__ void enter_surface(Player *p, Player *actor, Surface *before, Surface *after, Model *model) {
    s32 ok = 0;     /* dead store, but it keeps the 2.5f of the shake out of the loop's hoisted invariants */

    if (p->stateFlags & 0x8000) {
        return;
    }
    if (not_flagged(after->flags, 0x40000)) {
        return;
    }
    if (after->flags & 0x200000) {
        ok = not_flagged(before->flags, 0x40000);
    } else if (CGameRegion__GetHeight(model, actor->pos.x, actor->pos.z) < actor->pos.y) {
        ok = 0;
    } else {
        ok = 1;
    }
    if (ok) {
        CPlayer__Warp(p, after->unk48, (after->unk54 & 1) ? 1 : 2, 0);
    }
}

/* Moves the player one frame: input toggles, health regeneration in
 * multiplayer, then the state machine and collision in a loop over the frame
 * time, the landscape ceiling, the animation for the current action, the
 * cheats (all weapons / ammo) and the weapon sounds. */
void CPlayer__Update(Player *p) {
    CVector3 prev;
    Input saved;
    DamageEvent damage;
    s32 soundFlags;
    s32 human;
    s32 linkFlags;
    f32 scale;
    f32 remaining;
    f32 rate;
    f32 regen;
    Player *actor;
    GameSettings *game;
    s32 enable;
    Model *oldModel;
    Model *model;
    Surface *material;
    Surface *before;
    Surface *under;
    u32 *stateFlags;
    s32 hit;
    s32 landed;
    s32 flag;
    s32 first;
    s32 second;
    s32 low;
    s32 high;
    s32 modelId;
    s32 i;
    s32 j;

    linkFlags = 0;
    scale = gFrameIncrement;
    actor = p;
    soundFlags = 0;
    human = p->control == 2;
    if (Cinema_PathPlaying()) {
        human = 1;
    }
    game = &gEngineApp_m_Settings;
    if (game->multiplayer) {
        human = 1;
        CCache__SetFrameDelay(8);
    }
    enable = 0;
    p->owner = p;
    p->unk430 = p;
    if ((p->aiFlags & 0x80) && game->multiplayer == 0) {
        enable = *(u32 *) ((u8 *) game - 0xCAC) == 0; /* gEngineApp_m_Game_m_MenuPool, the block before */
    }
    Sound__SetMuted(enable);
    if (gEngineApp_m_Settings.multiplayer == 0) {
        if (!(gEngineApp_m_Game_m_DebugLines != 0 && (p->input.held & 0x20)) && (p->input.pressed & 0x800)) {
            if (p->unk12A4 != 0) {
                p->unk12A4 = 0;
            } else {
                p->unk12A4 = 1;
            }
        }
        if (gEngineApp_m_Settings.multiplayer == 0 && !(p->unk1050 != 0 && p->unkCD0 != 0)
            && !(gEngineApp_m_Game_m_DebugLines != 0 && (p->input.held & 0x20)) && (p->input.pressed & 0x200)) {
            WeaponInfo *info = gWeaponInfoTable[p->nextWeapon];
            s16 next = info->alternate;

            if (next != -1 && p->owned[next] != 0) {
                p->nextWeapon = info->alternate;
            }
        }
    }
    regen = 0.0f;   /* also serves the comparisons with 0 (f2 in the ROM) */
    if (gEngineApp_m_Settings.multiplayer != 0 && (rate = gCharacterHealthRegen[p->character]) != 0.0f) {
        f32 health = p->health;

        if (health != 0.0f && p->options->m_TagMode == 0) {
            f32 max;

            regen = health + (f32) (s32) (rate * 256.0f) * (gFrameIncrement * 0.06666667f);
            max = (u32) (p->def->m_Player.m_StartHealth << 8);
            if (!(max <= regen)) {
                max = regen;
            }
            p->health = max;
        }
    }
    actor->flags &= ~0x20000;
    AI_UpdateRegionFlags(actor, &actor->aiFlags);
    if (human) {
        CGameObjectInstance__UpdateAnimation(actor);
    }
    gFrameIncrement = scale;
    AI_RideMovingPlatform(actor, &actor->aiFlags);
    if (link_enabled(&p->unk6A9)) {
        linkFlags |= 0x800;
    }
    if (link_enabled(&p->unk6AB)) {
        linkFlags |= 0x1000;
    }
    if (p->options->m_TagMode == 1) {
        linkFlags |= 0x2000;
        soundFlags |= 0x400000;
    }
    Cinema_PathPlaying();
    if (p->unkAAC > 0.0f) {
        f32 left = p->unkAAC - gFrameIncrement;

        if (left < 0.0f) {
            left = 0.0f;
        }
        p->unkAAC = left;
    }
    if (p->unkA28 > 0.0f) {
        f32 left = p->unkA28 - gFrameIncrement;

        if (left < 0.0f) {
            left = 0.0f;
        }
        p->unkA28 = left;
    }
    /* Run the frame in steps (one, here): the input's edges are seen once. */
    saved = p->input;
    for (remaining = gFrameIncrement; remaining > 0.0f;) {
        remaining = 0.0f;
        AI_UpdateRegionFlags(actor, &actor->aiFlags);
        p->prevState = (u16) p->state;
        p->prevStateFlags = p->stateFlags;
        model = actor->model;
        material = GameObject_GetRegionInfo(actor);
        CPlayer__UpdateSpeedScale(p, actor, material);
        if (gEngineApp_m_Settings.multiplayer == 0) {
            CVector3 *pos = &p->pos;

            if (model != NULL && pos != NULL && (model->flags & 0x40)) {
                f32 amount = (s32) (CGameRegion__GetCeilingHeight(model, p->pos.x, pos->z) - CGameRegion__GetHeight(model, p->pos.x, pos->z));

                if (amount < 1024.0f) {
                    Sound__SetReverb(1.0f - amount * 0.0009765625f);
                } else {
                    Sound__SetReverb(0.0f);
                }
            } else {
                Sound__SetReverb(0.0f);
            }
        }
        if (material != NULL) {
            p->step = gFrameIncrement * p->unkB28;
        } else {
            p->step = gFrameIncrement;
        }
        CPlayer__EaseEyeOffset(p, actor);
        p->speed = func_00298470(p->velX * p->velX + p->velZ * p->velZ);
        p->unkB00 += p->speed * 0.021000002f;
        p->unkAFC += p->speed * 1.00000005e-4f;
        p->goal = actor->pos;
        if (p->shake > 0.0f && p->health != 0 && !CPlayer__IsInMatchEndState(p)) {
            p->goal.x += (Random_Float(5.0f) - 2.5f) * 10.24f;
            p->goal.z += (Random_Float(5.0f) - 2.5f) * 10.24f;
            p->goal.y += Random_Float(1.0f) * 10.24f;
            p->yaw += (Random_Float(20.0f) - 10.0f) * 0.017453294f;
            p->unkAC8 += Random_Float(0.5f) - 0.25f;
            if (gEvenOdd != 0) {
                DamageEvent_Set(&damage, p->unk12AC, 0x19, 25.599998f, 0x100000, 0);
                Player_OnMultiplayerKill(p, &p->aiFlags, &damage);
            }
            Sound__Play(0x227, p->pos, 0, -1);
        }
        p->stateTime += gFrameIncrement;
        {
            s32 ground = GameObject_IsOnGround(actor);

            if (ground != 0 && p->onGround == 0 && p->lastVelY < -51.199997f) {
                GameObject_PlayLandSound(actor);
            }
            p->onGround = ground;
            p->lastVelY = actor->velY;
            if (ground == 0 && p->state != 14 && !(actor->aiFlags & 0x200)) {
                p->airTime += gFrameIncrement;
            } else {
                p->airTime = 0.0f;
            }
        }
        if ((u16) p->state >= 2 && p->health != 0) {
            CPlayer__CheckJumpPad(p, actor);
            CPlayer__CheckWater(p, actor);
            CPlayer__CheckState12Hit(p, actor);
            CPlayer__CheckClimb(p, actor);
            CPlayer__UpdateZoom(p);
        }
        if (!Cinema_PathPlaying() || (u32) ((u16) p->state - 19) < 12) {
            void (*update)(Player *, Player *) = gPlayerStateTable[p->state].update;

            if (update != NULL) {
                update(p, actor);
            }
        }
        material = GameObject_GetRegionInfo(actor);
        if (material != NULL) {
            GameObject_ApplyCurrent(actor, material, &p->goal, 1);
            if ((material->flags & 0x4000) && actor->pos.y <= CGameRegion__GetHeight(actor->model, actor->pos.x, actor->pos.z)) {
                CPlayer__Hurt(p, actor, 0x3E700, 0, 0x1D);
            }
        }
        stateFlags = gPlayerStateTable[p->state].flags;
        if (p->state == 14) {
            stateFlags = &D_800F1E30;
            if (p->velX >= 0.0f) {
                stateFlags = &D_800F1F10;
            }
        }
        flag = 0;
        if (actor->aiFlags & 0x6000) {
            flag = 1;
        } else if ((u32) ((u16) p->state - 2) < 2) {
            if ((p->input.pressed & 0x400) && gEngineApp_m_Settings.multiplayer == 0 && !(gEngineApp_m_Game_m_DebugLines != 0 && (p->input.held & 0x20))) {
                p->crouchToggle ^= 1;
            }
        } else {
            p->crouchToggle = 0;
        }
        if (p->crouchToggle != 0) {
            flag = 1;
        }
        {
            f32 change;

            if (flag) {
                change = (40.96f - p->crouch) * 0.25f;
            } else {
                change = -p->crouch * 0.25f;
            }
            p->crouch += change * gFrameIncrement;
            change = 40.96f - p->crouch;
            if (-10.24f <= change && change <= 10.24f) {
                *stateFlags |= 0x80;
            } else {
                *stateFlags &= ~0x80;
            }
        }
        CPlayer__UpdateWaterRegion(p, actor, material);
        if (p->unkA30 > 0.0f) {
            p->unkA30 -= gFrameIncrement;
            if (p->unkA30 < 0.0f) {
                p->unkA30 = 0.0f;
            }
        }
        CPlayer__UpdateFlashlight(p, actor);
        oldModel = actor->model;
        prev = actor->pos;
        before = GameObject_GetRegionInfo(actor);
        if (p->collide != 0) {
            actor->flags |= 0x10000;
            hit = Collision_MoveInstance(actor, p->goal, stateFlags);
            actor->flags &= ~0x10000;
        } else {
            hit = 0;
        }
        model = actor->model;
        material = GameObject_GetRegionInfo(actor);
        if (before != material) {
            CGameRegion__FireTriggers(oldModel, linkFlags | 2, actor);
            CGameRegion__FireTriggers(model, linkFlags | 1, actor);
        }
        CScene__CheckAutoActivation(&gScene, actor, &prev, &actor->pos);
        landed = 0;
        if (hit) {
            if (gCollisionResults.m_pInstance != NULL && gCollisionResults.m_pInstance->header.m_Type == 1) {
                GameObject_FireTriggers((u8 *)gCollisionResults.m_pInstance, soundFlags | 2, actor);
            }
            if (gCollisionResults.m_pWallRegion != NULL) {
                CGameRegion__FireTriggers((Model *)gCollisionResults.m_pWallRegion, linkFlags | 4, actor);
            }
            CPlayer__RamWithMount(p, actor, stateFlags);
            landed = CPlayer__TryMount(p, actor, stateFlags);
            if (gCollisionResults_m_nFloorKind == 2 && (u32) ((u16) p->state - 4) < 2) {
                if (actor->velY > 0.0f) {
                    actor->velY = 0.0f;
                }
                p->state = 5;
                p->velZ *= 0.1f;
                p->velX *= 0.1f;
            }
        }
        if (gCollisionResults.m_pInstance != NULL && gCollisionResults.m_pInstance->header.m_Type == 3) {
            CPlayer__TouchPickup(p, (u8 *)gCollisionResults.m_pInstance);
        }
        if (before != NULL && material != NULL) {
            if ((p->stateFlags & 0x8000) && !landed && !(before->unk54 & 8) && (material->unk54 & 8)) {
                CPlayer__Dismount(p, actor);
            }
            if (material->flags & 0x4000000) {
                if (material->item != p->item) {
                    p->item = material->item;
                }
            }
            if (material->flags & 0x8000000) {
                p->item = material->item;
                p->lastItem = material->item;
                gScene_m_PendingWarpID = material->item;
            }
            enter_surface(p, actor, before, material, model);
        }
        p->input.pressed = 0;
        p->input.unk2C = 0;
        p->input.unk30 = 0;
        p->input.unk34 = 0;
    }
    p->input = saved;
    gFrameIncrement = scale;
    under = GameObject_GetRegionInfo(actor);
    if (under != NULL && (under->flags & 0x4000)) {
        f32 x2 = actor->pos.x * actor->pos.x;
        f32 z2 = actor->pos.z * actor->pos.z;

        if (x2 + actor->pos.y * actor->pos.y + z2 > 4e8f) {
            f32 y = 4e8f - x2 - z2;

            if (y < 0.0f) {
                y = 0.0f;
            }
            y = func_00298470(y);
            if (actor->pos.y < 0.0f) {
                y = -y;
            }
            actor->pos.y = y;
            if (under->flags & 0x4000) {
                actor->pos.y = GameObject_GetRegionFloorHeight(actor, 0) - 1.0f;
            }
        }
    }
    if (human) {
        modelId = 102;
        if (gEngineApp_m_Settings.multiplayer != 0) {
            if (gEngineApp_m_Settings_match_tag != 0 && p->options->m_TagMode != 0) {
                modelId = gTagModeModel;
            } else {
                modelId = gCharacterModels[p->def->m_Id];
            }
        }
        CScene__SetInstanceObjectType(&gScene, p, modelId, p->action);
        if (p->lastAction != p->action) {
            if (!CGameObjectInstance__SetAnimByAction(actor, p->action, 0) && Cinema_PathPlaying()) {
                CGameObjectInstance__SetAnimByAction(actor, 7000, 0);
            }
        }
        first = CGameObjectInstance__PickRandomAnim(actor, p->action, -1);
        second = CGameObjectInstance__PickRandomAnim(actor, p->action, first);
        low = second;
        if (second > first) {
            low = first;
        }
        high = second;
        if (high < first) {
            high = first;
        }
        if (actor->animPending) {
            if (actor->anim == low) {
                if (Random_Float(100.0f) < 0.4f) {
                    CGameObjectInstance__SetAnim(actor, high, 0);
                    actor->animPending = 0;
                }
            } else {
                CGameObjectInstance__SetAnim(actor, low, 0);
                actor->animPending = 0;
            }
        }
    }
    if (!human) {
        func_0021A500(actor);
    }
    p->emitterPos = p->pos;
    p->emitterYaw = p->yaw;
    p->emitterModel = p->model;
    p->emitter40 = p->unk40;
    p->lastPos = p->pos;
    if (gEngineApp_m_Settings.cheatFlags & 8) {
        for (i = 0; i < 31; i++) {
            if (i != 21) {
                p->ammo[i] = 0;
            }
        }
        for (i = 0; i < 35; i++) {
            WeaponInfo *info;

            if (i == 21) {
                continue;
            }
            info = gWeaponInfoTable[i];
            for (j = 0; j < 3; j++) {
                AmmoInfo *def = info->ammo[j];
                s16 *slot;

                if (def == NULL) {
                    break;
                }
                slot = &p->ammo[def->type];
                *slot = (def->max < *slot) ? *slot : def->max;
            }
        }
    }
    if (gEngineApp_m_Settings.cheatFlags & 4) {
        if (gEngineApp_m_Settings.multiplayer == 0 && p->owned[21] == 0) {
            *(p->ammo + *CPlayer__GetFiringAmmo(p, 21)) = 1;
        }
        for (i = 0; i < 35; i++) {
            if ((i >= 24) ^ !gEngineApp_m_Settings.multiplayer) {
                p->owned[i] = 1;
            }
        }
    }
    CPlayer__UpdateWeaponSet((CPlayer *) p);
    CBarrel__Update(&p->unkCD0, p, p->unk3C8);
    if (p->shake > 0.0f) {
        p->shake -= gFrameIncrement;
        if (p->shake > 0.0f) {
            p->flags |= 0x800000;
        } else {
            p->flags &= ~0x800000;
        }
    }
    Player_UpdateAim(p);
    p->input.controller->pos = p->pos;
    CScene__SetInstanceObjectType(&gScene, p->emitter, gWeaponInfoTable[p->weapon]->model, p->voice->bank);
    p->lastAction = p->action;
}
