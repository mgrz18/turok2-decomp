#ifndef STRUCTS_PLAYER_OPTIONS_H
#define STRUCTS_PLAYER_OPTIONS_H

/* One player's options, apart from structs/player.h so that structs/game.h
 * (GameSettings holds five of them) does not need the whole player. It was
 * GameSlot in structs/game.h and CPlayerOptions in structs/player.h: the same
 * 0x40 bytes (D_8011AB18 = D_8011AAD8 + 0x40). */


/* A player's slot in the game options (GameSettings, D_8011AAD8): the
 * single player's at D_8011AB18 (+0x40, defaultSlot), the four multiplayer
 * ones from D_8011AB58 (+0x80, slots).
 * CPlayer::m_pOptions points at one. The counts are kept per opponent. */
typedef struct CPlayerOptions {
    /* 0x00 */ u16 m_Suicides;
    /* 0x02 */ u16 m_Deaths;
    /* 0x04 */ u16 m_Kills;
    /* 0x06 */ u16 m_Tags;
    /* 0x08 */ u16 m_TaggedBy[4];
    /* 0x10 */ u16 m_Tagged[4];
    /* 0x18 */ u16 m_KilledBy[4];
    /* 0x20 */ u16 m_Killed[4];
    /* 0x28 */ u8 m_bActive;               /* joined the game */
    /* 0x29 */ u8 m_SettingX;              /* 0x80-centred, stepped by 8 in the menu */
    /* 0x2A */ u8 m_SettingY;              /* 0x80-centred, stepped by 8 in the menu */
    /* 0x2B */ s8 unk_0x2B;
    /* 0x2C */ u8 unk_0x2C;
    /* 0x2D */ u8 unk_0x2D;
    /* 0x2E */ u8 unk_0x2E;
    /* 0x2F */ s8 m_nController;
    /* 0x30 */ s8 m_nCharacter;            /* also the team in team games */
    /* 0x31 */ u8 m_ControlStyle;
    /* 0x32 */ u8 m_Handicap;
    /* 0x33 */ u8 m_Name[8];
    /* 0x3B */ u8 _pad0;
    /* 0x3C */ u8 m_bDead;
    /* 0x3D */ u8 unk_0x3D;
    /* 0x3E */ u8 m_TagMode;
    /* 0x3F */ u8 m_LastTagMode;
} CPlayerOptions; /* size 0x40 */

#endif
