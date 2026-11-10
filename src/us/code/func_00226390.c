#include "common.h"

#ifdef TARGET_PORT
#include "structs/world.h"

/* Host body: the N64 body clears the counts through a view at N64 offsets; on the host those
 * offsets land inside the (8-byte) pointer arrays of CScene (round 7, v4). */
void CScene__ClearListCounts(CScene *arg0) {
    arg0->m_nDrawActors = 0;
    arg0->m_nMovers = 0;
    arg0->m_nListed = 0;
    arg0->m_nRegistered = 0;
    arg0->m_nKind1 = 0;
    arg0->m_nFlagged = 0;
    arg0->m_nKind5 = 0;
    arg0->m_nList6 = 0;
    arg0->m_nList7 = 0;
    arg0->m_nDeferred = 0;
    arg0->m_nSectors = 0;
}
#else

/* The counts this clears, scattered over a large block. */
typedef struct {
    /* 0x0000 */ u8 pad0[0x8EC];
    /* 0x08EC */ s32 unk8EC;
    /* 0x08F0 */ u8 pad8F0[0xAF0 - 0x8F0];
    /* 0x0AF0 */ s32 unkAF0;
    /* 0x0AF4 */ u8 padAF4[0xBF4 - 0xAF4];
    /* 0x0BF4 */ s32 unkBF4;
    /* 0x0BF8 */ u8 padBF8[0xDF8 - 0xBF8];
    /* 0x0DF8 */ s32 unkDF8;
    /* 0x0DFC */ u8 padDFC[0xEFC - 0xDFC];
    /* 0x0EFC */ s32 unkEFC;
    /* 0x0F00 */ u8 padF00[0xF84 - 0xF00];
    /* 0x0F84 */ s32 unkF84;
    /* 0x0F88 */ u8 padF88[0xFC8 - 0xF88];
    /* 0x0FC8 */ s32 unkFC8;
    /* 0x0FCC */ u8 padFCC[0x104C - 0xFCC];
    /* 0x104C */ s32 unk104C;
    /* 0x1050 */ u8 pad1050[0x1060 - 0x1050];
    /* 0x1060 */ s32 unk1060;
    /* 0x1064 */ s32 unk1064;
    /* 0x1068 */ u8 pad1068[0x14A8 - 0x1068];
    /* 0x14A8 */ s32 unk14A8;
} Arg0;

void CScene__ClearListCounts(Arg0 *arg0) {
    arg0->unk8EC = 0;
    arg0->unkAF0 = 0;
    arg0->unkBF4 = 0;
    arg0->unkDF8 = 0;
    arg0->unkEFC = 0;
    arg0->unkF84 = 0;
    arg0->unkFC8 = 0;
    arg0->unk104C = 0;
    arg0->unk1060 = 0;
    arg0->unk1064 = 0;
    arg0->unk14A8 = 0;
}
#endif
