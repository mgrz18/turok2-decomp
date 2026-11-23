#include "globals/menu_text.h"
typedef signed char s8;
typedef signed int s32;
typedef unsigned char u8;

typedef struct {
    /* 0x00 */ char pad0[0x2F];
    /* 0x2F */ s8 kind;
    /* 0x30 */ char pad30[0x3];
    /* 0x33 */ u8 name[8];
} NamedThing;


void PlayerOptions_SetDefaultName_2(NamedThing *t) {
    s32 i;

    switch (t->kind) {
        case 0:
        default:
            for (i = 0; i < 8; i++) {
                t->name[i] = sPlayerNameText_Player1[i];
            }
            break;
        case 1:
            for (i = 0; i < 8; i++) {
                t->name[i] = sPlayerNameText_Player2[i];
            }
            break;
        case 2:
            for (i = 0; i < 8; i++) {
                t->name[i] = sPlayerNameText_Player3[i];
            }
            break;
        case 3:
            for (i = 0; i < 8; i++) {
                t->name[i] = sPlayerNameText_Player4[i];
            }
            break;
    }
}
