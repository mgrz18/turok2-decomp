/* From thejunkman9000/Turok-Rage-Wars-Decomp (Turok: Rage Wars): src/ovl_a/func_0044E2B8.c, func_0044E2B8.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "structs/vector.h"
#include "structs/world.h"
/* protos/scene.h's prototypes of what this file calls (the module defines them all; the
 * merged file declares them from the definitions). */
struct CCamera;
struct CGameObjectInstance;
struct CScene;
struct CSceneRecord;
struct LevelEvent;
void CScene__RestoreFiredEvent(void *, void *);
struct Item_func_8044D668_de;
typedef struct Item_func_8044D668_de Item_func_8044D668_de;
struct Item_func_8044D668_de;
struct Item_func_8044D668_de {
    char pad0[0x11];
    u8 kind;
    char pad12[2];
};
struct Level;
struct Level {
    char pad0[0x1168];  /* record tables 0x58 earlier than in Rage Wars */
    s32 counts[2];
    char pad1170[8];
    Item_func_8044D668_de *items[2];
};
struct Level;
typedef struct Level Level;
extern void Cinema_Disable(void);
extern void Cinema_Enable(void);
#ifdef TARGET_PORT
/* The port (round 13 b2): the record tables by member of CScene (the N64 view reads them at
 * 0x1168-0x117C, other members of the host CScene, and keeps the table pointers in s32). */
void CScene__ReapplyFiredEvents_2(CScene *w) {
    s32 i, k, n;
    Item_func_8044D668_de *item;
    Cinema_Disable();
    for (k = 0; k < 2; k++) {
        n = k == 0 ? w->m_nRecords : w->m_nAltRecords;
        for (i = 0; i < n; i++) {
            item = (Item_func_8044D668_de *) &(k == 0 ? w->m_pRecords : w->m_pAltRecords)[i];
            if (item->kind == 1 || item->kind == 4 || item->kind == 2 || item->kind == 10) {
                CScene__RestoreFiredEvent(w, item);
            }
        }
    }
    Cinema_Enable();
}
#else
#define level ((Level *) scene)
/* the definition takes the callers' type (one prototype for the merged object); the Rage Wars view reads it */
void CScene__ReapplyFiredEvents_2(CScene *scene) {
    s32 i;
    s32 n;
    Item_func_8044D668_de *item;
    Cinema_Disable();
    n = level->counts[0];
    for (i = 0; i < n; i++) {
        item = &level->items[0][i];
        if (item->kind == 1 || item->kind == 4 || item->kind == 2 || item->kind == 10) {
            CScene__RestoreFiredEvent(level, item);
        }
    }
    n = level->counts[1];
    for (i = 0; i < n; i++) {
        item = &level->items[1][i];
        if (item->kind == 1 || item->kind == 4 || item->kind == 2 || item->kind == 10) {
            CScene__RestoreFiredEvent(level, item);
        }
    }
    Cinema_Enable();
}
#undef level
#endif
