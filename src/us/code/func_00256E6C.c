typedef int s32;
typedef unsigned char u8;

typedef struct {
    char pad0[0x24];
    s32 unk_24;                 /* 0x24 */
    char pad28[0x34 - 0x28];
    s32 unk_34;                 /* 0x34 */
} SubSettings;

typedef struct {
    u8 enabled;                 /* 0x000 */
    char pad1[0xB - 0x1];
    u8 value_B;                 /* 0x00B */
    char padC[0x10 - 0xC];
    u8 value_10;                /* 0x010 */
    char pad11[0x15 - 0x11];
    u8 value_15;                /* 0x015 */
} Settings;

extern Settings gEngineApp_m_Settings_multiplayer; /* local type: globals/game.h declares it; this view (its type here) is what keeps the ROM's code */

u8 Hud_GetSpriteValue(s32 unused, s32 id) {
    Settings *settings;
    SubSettings *sub;

    if (id == 0x2DA) {
        settings = &gEngineApp_m_Settings_multiplayer;
        sub = (SubSettings *)((char *)settings + 0x1BF);
        if (settings->enabled != 0) {
            if (sub->unk_34 == 0) {
                if (sub->unk_24 != 0) {
                    return settings->value_15;
                }
                return settings->value_B;
            }
            return settings->value_10;
        }
    }
    return 1;
}
