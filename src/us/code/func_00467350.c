/* Second VM module copy of func_0042E430 (same source, linked 0x0042E350). */
#include "common.h"
#include "structs/player.h"

typedef struct PlayerDef {
    /* 0x00 */ u8 pad0[0xC];
    /* 0x0C */ s16 kind;
    /* 0x0E */ u8 padE[0x18 - 0xE];
    /* 0x18 */ s32 health;
    /* 0x1C */ u8 pad1C[0x38 - 0x1C];
    /* 0x38 */ f32 scale[3];
} PlayerDef;

extern u8 D_800F7078[];  /* the world */
extern u8 D_8011AAF1;    /* multiplayer */
extern s32 D_8011ACE4;
extern s32 D_800ADB5C;
extern s32 D_800ADB30[];

extern PlayerDef *func_00225EBC(void *world, s32 kind, s32 character);
extern s32 func_002113A4(void);
extern void func_00248A8C(CPlayer *p);
extern void func_002241D8(void *world, CPlayer *p, s32, s32);

static __inline__ PlayerDef *findDef(s32 kind, s32 character) {
    return func_00225EBC(D_800F7078, kind, character);
}

void func_00467350(CPlayer *p, s32 character) {
    PlayerDef *def;
    s32 sound;

    p->m_Data.m_nDeathmatchCharacter = character;
    if (p->m_pOptions->m_TagMode == 1) {
        character = 11;
    }
    def = findDef(11, character);
    if (def == NULL) {
        def = findDef(11, -1);
        if (def == NULL) {
            def = findDef(-1, -1);
        }
    }
    p->m_CharacterActor.header.m_pIntelligence = def;
    p->m_CharacterActor.m_vScale.x = def->scale[0];
    p->m_CharacterActor.m_vScale.y = def->scale[1];
    p->m_CharacterActor.m_vScale.z = def->scale[2];
    p->m_Data.m_Health = def->health << 8;
    p->m_CharacterActor.m_AI.m_Health = ((PlayerDef *)p->m_CharacterActor.header.m_pIntelligence)->health << 8;
    if (p->m_Data.m_nDeathmatchCharacter == 7) {
        p->m_CharacterActor.m_AI.m_nTexture = func_002113A4() % 18;
    } else {
        p->m_CharacterActor.m_AI.m_nTexture = -1;
    }
    func_00248A8C(p);
    sound = 0x66;
    if (D_8011AAF1) {
        if (D_8011ACE4 != 0 && p->m_pOptions->m_TagMode != 0) {
            sound = D_800ADB5C;
        } else {
            sound = D_800ADB30[((PlayerDef *)p->m_CharacterActor.header.m_pIntelligence)->kind];
        }
    }
    func_002241D8(D_800F7078, p, sound, p->m_CharAnim);
}
