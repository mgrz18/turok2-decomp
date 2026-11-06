#include "structs/vector.h"
#include "structs/game.h"

/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028D0E4.c, func_0028D0E4.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "structs/intelligence.h"
#include "structs/world.h"
typedef struct { f32 div; u8 r; u8 g; u8 b; u8 pad7; } Pal;
void CEngineApp__DrawFilledRect(u8 *a, u8 *col, s32 c, s32 d, s32 e, s32 f);
extern Pal gWarpFadeColors[];
extern s32 gScreenWidth;
extern s32 gScreenHeight;
void CScene__DrawWarpFade(CScene *lv) {
    u8 col[4];
    f32 t;
    Pal *p;
    s32 a = lv->m_WarpState;
    if (a != 0) {
      if (a >= 0) {
       if (a < 3) {
        p = &gWarpFadeColors[lv->m_WarpFade];
        if (p->div != 0.0f) {
            col[0] = p->r;
            col[1] = p->g;
            col[2] = p->b;
            t = lv->m_WarpTime * 255.0f / p->div;
            col[3] = (u32)t;
            CEngineApp__DrawFilledRect(&gEngineApp, col, 0, 0, gScreenWidth, gScreenHeight);
        }
       }
      }
    }
}
