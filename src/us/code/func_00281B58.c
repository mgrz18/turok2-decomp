#include "common.h"
#include "structs/camera.h"

/* Resets a camera text (see CCameraPool__AddTextEx.c): two words cleared. */
typedef struct {
    /* 0x00 */ u8 pad0[0x14];
    /* 0x14 */ s32 unk14;
    /* 0x18 */ u8 pad18[4];
    /* 0x1C */ s32 unk1C;
} TextReset;

#ifdef TARGET_PORT
/* The host body on the members (round 12 a5, tools/port/n64viewcheck.py): m_nId is pointer-wide
 * on the host (round 11 z3), so the view's 0x1C cleared the padding after m_nAlpha and left the
 * owner id of the previous text. */
void CCameraText__Reset(CCameraText *arg0) {
    arg0->m_nMode = 0;
    arg0->m_nId = 0;
}
#else
void CCameraText__Reset(TextReset *arg0) {
    arg0->unk14 = 0;
    arg0->unk1C = 0;
}
#endif
