#include "common.h"
#include "structs/game_object.h"

typedef struct {
    /* 0x00 */ s32 kind;
    /* 0x04 */ s32 visible;
    /* 0x08 */ u8 pad08[0x88 - 0x08];
} Targets;

/* Boss module state at 0x8012F608. */
typedef struct {
    /* 0x000 */ s32 stage;
    /* 0x004 */ u8 pad004[0x3BC];
    /* 0x3C0 */ s32 part;           /* 0x8012F9C8 */
    /* 0x3C4 */ u32 partsDone;      /* 0x8012F9CC */
    /* 0x3C8 */ u8 pad3C8;
    /* 0x3C9 */ u8 partActive;      /* 0x8012F9D1 */
    /* 0x3CA */ u8 pad3CA[0x1A];
    /* 0x3E4 */ s32 damage;         /* 0x8012F9EC */
    /* 0x3E8 */ f32 partTimer;      /* 0x8012F9F0 */
} BossModule;

extern void func_00246108(CGameObjectInstance *obj, CAI *ai, Targets *t, s32 f1, s32 f2);
extern void func_00246918(CGameObjectInstance *obj, CAI *ai);
extern void func_00243414(CGameObjectInstance *obj, CAI *ai, s32 mode);

extern s32 D_800F1F50[];
extern f32 D_800B6D28;          /* per-frame scale */
extern BossModule D_8012F608;
extern s32 D_8012F988[];
extern s32 D_8012F9A8[];

void func_00405958(CGameObjectInstance *obj, CAI *ai) {
    Targets t;
    s32 limit;
    u32 bit;

    D_800F1F50[0] |= 0x8000;
    func_00246108(obj, ai, &t, 1, 1);
    D_800F1F50[0] &= ~0x8000;

    limit = 0x4B00;
    if (D_8012F608.stage == 0) {
        limit = 0x2580;
    }
    if (D_8012F608.damage >= limit) {
        func_00246918(obj, ai);
        ai->m_VisBits &= ~(1 << D_8012F608.part);
        D_8012F608.partActive = 0;
        func_00243414(obj, ai, 7);
    } else if (D_8012F608.partTimer <= 0.0f) {
        func_00246918(obj, ai);
        D_8012F988[D_8012F608.part] = D_8012F9A8[D_8012F608.part];
        bit = 1 << D_8012F608.part;
        D_8012F608.partActive = 0;
        D_8012F608.partsDone |= bit;
        ai->m_VisBits |= bit;
        func_00243414(obj, ai, 7);
    } else {
        D_8012F608.partTimer -= D_800B6D28;
    }
}
