/* WRONG CUT: REAL func_0042EBBC size 0x70 (splat's 0x74 includes the padding nop at 0x42EC2C before the 16-aligned Demo_ReadRunLength). */
#include "common.h"

typedef struct Lists {
    /* 0x00 */ u8 list0[0x40];
    /* 0x40 */ s32 unk40;
    /* 0x44 */ u8 pad44[4];
    /* 0x48 */ u8 list1[0x40];
    /* 0x88 */ s32 unk88;
} Lists;

extern u8 D_800B2C30[];
extern u8 D_800B2C40[];

extern void COverlay__Construct(void *, s32, s32);
extern void COverlay__StartScript(void *, void *);

#ifdef TARGET_PORT
#include "structs/player.h"

/* Host body: this is COnScreen's boss-bar reset (m_pBoss, m_BossOverlay, m_pBoss2,
 * m_BossOverlay2), but through a view at N64 offsets. On the host COverlay holds a pointer-wide
 * m_pScript, so the second overlay was constructed over m_pBossName/m_pBoss/m_BossPercent (m_pBoss
 * left non-NULL: an empty boss bar on screen from level 1 on, once DrawBossBars tested the right
 * member) and m_pBoss2 was never cleared (round 17 f4). The same calls by member. */
void func_0042EBBC(void *arg0) {
    COnScreen *os = (COnScreen *)arg0;

    os->m_pBoss = NULL;
    COverlay__Construct(&os->m_BossOverlay, 0, -40);
    COverlay__StartScript(&os->m_BossOverlay, D_800B2C30);
    os->m_pBoss2 = NULL;
    COverlay__Construct(&os->m_BossOverlay2, 0, -20);
    COverlay__StartScript(&os->m_BossOverlay2, D_800B2C40);
}
#else
void func_0042EBBC(Lists *lists) {
    lists->unk40 = 0;
    COverlay__Construct(lists->list0, 0, -40);
    COverlay__StartScript(lists->list0, D_800B2C30);
    lists->unk88 = 0;
    COverlay__Construct(lists->list1, 0, -20);
    COverlay__StartScript(lists->list1, D_800B2C40);
}
#endif
