/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/MENU/ITEMS/8044ACCC_30.CPP, func_8044D5C4_de.
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
struct State_func_8044D5C4_de;
struct State_func_8044D5C4_de {
    char p[0x1168];
    s32 unk1168;
    s32 unk116C;
    char q[8];
    s32 unk1178;
    s32 unk117C;
};
struct State_func_8044D5C4_de;
typedef struct State_func_8044D5C4_de State_func_8044D5C4_de;
void Cinema_Enable();
void Cinema_Disable();
#ifdef TARGET_PORT
/* The port (round 13 b2): the record tables by member of CScene (the N64 view reads them at
 * 0x1168-0x117C, other members of the host CScene, and keeps the table pointers in s32). */
void CScene__RestoreFiredEvents_2(CScene *w) {
    s32 i, n;
    Cinema_Disable();
    n = w->m_nRecords;
    for (i = 0; i < n; i++) CScene__RestoreFiredEvent(w, &w->m_pRecords[i]);
    n = w->m_nAltRecords;
    for (i = 0; i < n; i++) CScene__RestoreFiredEvent(w, &w->m_pAltRecords[i]);
    Cinema_Enable();
}
#else
#define arg0 ((State_func_8044D5C4_de *) scene)
/* the definition takes the callers' type (one prototype for the merged object); the Rage Wars view reads it */
void CScene__RestoreFiredEvents_2(CScene *scene) {
 s32 i,n;
 Cinema_Disable();
 n=arg0->unk1168;
 for(i=0;i<n;i++) CScene__RestoreFiredEvent(arg0,arg0->unk1178+i*20);
 n=arg0->unk116C;
 for(i=0;i<n;i++) CScene__RestoreFiredEvent(arg0,arg0->unk117C+i*20);
 Cinema_Enable();
}
#undef arg0
#endif
