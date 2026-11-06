/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/game/func_0028B004.c, func_0028B004.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "common.h"
#include "gfx.h"
#include "structs/intelligence.h"
#include "structs/world.h"
#include "structs/game_object.h"
typedef struct Ent12 { s32 **tblp; s32 b; s32 c; } Ent12;
typedef struct Obj2 { u8 pad0[8]; void *unk8; f32 unkC; } Obj2;
void Render_ResetBatches();
void Render_FlushBatches(void);
void CGameObjectInstance__Draw(CGameObjectInstance *e, s32 a);
void CScene__DrawRegisteredInstances(CScene *lv, s32 arg) {
    Gfx *g;
    s32 i;
    s32 n;
    s32 c;
    CGameObjectInstance **arr;
    Render_ResetBatches();
    n = lv->m_nRegistered;
    g = gDList++;
    g->w0 = 0xDB040004;
    g->w1 = 5;
    g = gDList++;
    g->w0 = 0xDB04000C;
    g->w1 = 5;
    g = gDList++;
    g->w0 = 0xDB040014;
    g->w1 = 0xFFFB;
    g = gDList++;
    g->w0 = 0xDB04001C;
    g->w1 = 0xFFFB;
    arr = (CGameObjectInstance **)lv->m_pRegistered;
    for (i = 0; i < n; i++) {
        CGameObjectInstance *e = arr[i];
        if (e->m_dwFlags & 4) {
            c = lv->m_nDeferred;
            if (c != 0x40) {
                lv->m_pDeferred[c] = e;
                lv->m_nDeferred = c + 1;
            }
        } else {
            CGameObjectInstance__Draw(e, arg);
        }
    }
    Render_FlushBatches();
}
