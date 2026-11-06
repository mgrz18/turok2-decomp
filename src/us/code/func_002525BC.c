#include "common.h"
#include "structs/player.h"
#include "structs/camera.h"
#include "structs/game.h"

/* Tag game state. */

extern u8 D_8011AAEA;
extern s32 D_800C2030;          /* screen width */
extern s32 D_800C2034;          /* screen height */

extern s32 Random_Next(void);
extern void CPlayer__BecomeIt(CPlayer *p);
extern void CPlayerPool__RespawnAll(CPlayerList *list);
extern void CPlayerPool__EndMatch(CPlayerList *list);
extern s32 CPlayerPool__IsScoreLimitReached(CPlayerList *list);
extern CPlayer *CPlayerPool__GetViewPlayer(CPlayerList *list, u32 index);
extern void CPlayerPool__ClearLastTagMode(CPlayerList *list);
extern s32 CPlayerPool__CountPlaying(CPlayerList *list);
extern void Hud_SetupTextureMode(s32 color);
extern void Hud_DrawSprite(s32, s32, s16, s16, f32, f32, s32);

/* Tag: pick who is "it" at the start of a round, flashing over the viewports
 * in turn until the pick lands. */
void CPlayerPool__PickTagIt(CPlayerList *list) {
    s32 count;
    CPlayer *p;
    CCamera *cam;
    f32 w, h, cx, cy, sx, sy;
    GameMatch *s = &D_8011ACB0;

    if (!s->tag) {
        return;
    }
    if (CPlayerPool__IsScoreLimitReached(list) && s->scoreLimit > 0 && !s->over) {
        CPlayerPool__EndMatch(list);
        return;
    }
    count = CPlayerPool__CountPlaying(list);
    s = &D_8011ACB0;
    switch (s->tagPhase) {
        case 0:
        default:
            CPlayerPool__ClearLastTagMode(list);
            CPlayerPool__RespawnAll(list);
            s->tagPhase = 1;
            break;
        case 1:
            CPlayerPool__RespawnAll(list);
            if (count > 0) {
                s32 r = Random_Next() % count;
                s->tagPhase = 2;
                s->tagPickStart = r;
                s->tagPick = r;
            }
            break;
        case 2:
            p = CPlayerPool__GetViewPlayer(list, s->tagPick % count);
            cam = p->m_pCamera;
            if (cam != NULL && cam->m_Menus.count == 0) {
                Hud_SetupTextureMode(D_8011AAEA);
                w = cam->m_Viewport.m_Width;
                h = cam->m_Viewport.m_Height;
                cx = cam->m_Viewport.m_ScreenPosX + w * 0.5f;
                cy = cam->m_Viewport.m_ScreenPosY + h * 0.5f;
                sx = w / D_800C2030;
                sy = h / D_800C2034;
                Hud_DrawSprite(0x1FB, 0, cx - sx * 16.0f, cy - sy * 16.0f, sx, sy, 1);
            }
            s = &D_8011ACB0;
            if (++s->tagPick >= (5 - count) * 20 - s->tagPickStart) {
                CPlayerPool__ClearLastTagMode(list);
                s->tagPhase = 3;
                CPlayer__BecomeIt(p);
            }
            break;
        case 3:
            break;
    }
}
