#include "common.h"
#ifdef TARGET_PORT
#include "structs/world.h"
#endif

/* The counts this clears, scattered over a large block. */
#ifdef TARGET_PORT
typedef struct Arg0 {
/* the view at the host offsets of CScene (tools/port/hostview.py): each field in its own
 * member of one union, placed by __builtin_offsetof of the member it reads */
#define T2_HOFF_Arg0(m) __builtin_offsetof(CScene, m)
    union {
        struct { u8 _hp_unk8EC[T2_HOFF_Arg0(m_nDrawActors)]; s32 unk8EC; }; /* N64 0x8EC */
        struct { u8 _hp_unkAF0[T2_HOFF_Arg0(m_nMovers)]; s32 unkAF0; }; /* N64 0xAF0 */
        struct { u8 _hp_unkBF4[T2_HOFF_Arg0(m_nListed)]; s32 unkBF4; }; /* N64 0xBF4 */
        struct { u8 _hp_unkDF8[T2_HOFF_Arg0(m_nRegistered)]; s32 unkDF8; }; /* N64 0xDF8 */
        struct { u8 _hp_unkEFC[T2_HOFF_Arg0(m_nKind1)]; s32 unkEFC; }; /* N64 0xEFC */
        struct { u8 _hp_unkF84[T2_HOFF_Arg0(m_nFlagged)]; s32 unkF84; }; /* N64 0xF84 */
        struct { u8 _hp_unkFC8[T2_HOFF_Arg0(m_nKind5)]; s32 unkFC8; }; /* N64 0xFC8 */
        struct { u8 _hp_unk104C[T2_HOFF_Arg0(m_nList6)]; s32 unk104C; }; /* N64 0x104C */
        struct { u8 _hp_unk1060[T2_HOFF_Arg0(m_nList7)]; s32 unk1060; }; /* N64 0x1060 */
        struct { u8 _hp_unk1064[T2_HOFF_Arg0(m_nDeferred)]; s32 unk1064; }; /* N64 0x1064 */
        struct { u8 _hp_unk14A8[T2_HOFF_Arg0(m_nSectors)]; s32 unk14A8; }; /* N64 0x14A8 */
    };
} Arg0;
#else
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
#endif

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
