/* WRONG CUT: REAL CScene__InitPersistentData size 0x5C (splat cut its last word into func_0042C414). */
#include "common.h"
#include "globals/game.h"


extern void CScene__LoadBitSections(void *menu);
extern void CScene__LoadBitSections2(void *menu);

#ifdef TARGET_PORT /* the scene's preview fields by name: at the N64 offset 0x18FE8 the cheat flags
                    * missed CScene.m_PreviewOptions (0 at the first warp: "NEW CHEAT ENABLED" at the
                    * start of every new game) and landed elsewhere in gScene */
#include "structs/world.h"
typedef CScene MenuState;
#define unk18FE0 m_PreviewState
#define unk18FE4 m_PreviewTimer
#define unk18FE8 m_PreviewOptions
#else
typedef struct MenuState {
    /* 0x00000 */ u8 pad0[0x18FE0];
    /* 0x18FE0 */ s32 unk18FE0;
    /* 0x18FE4 */ s32 unk18FE4;
    /* 0x18FE8 */ s32 unk18FE8;
} MenuState;
#endif

void CScene__InitPersistentData(MenuState *menu) {
    CScene__LoadBitSections(menu);
    CScene__LoadBitSections2(menu);
    menu->unk18FE0 = 0;
    menu->unk18FE4 = 34;
    menu->unk18FE8 = gEngineApp_m_Settings.unlockedCheats;
}
