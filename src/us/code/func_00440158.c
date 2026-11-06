#include "common.h"

/* gLevelScript_script3_cursor: two words reset to -1; the word just before it (gLevelScript_script3_steps) holds a table pointer. */
typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
} State8012FA24;

extern s32 sPrimagenSoundScript2_2;
extern s32 sPrimagenSoundScript1_2;
extern s32 sPrimagenSoundScript3_2;
extern s32 sPrimagenSoundScript4_2;
extern State8012FA24 gLevelScript_script3_cursor;

void Primagen_SetSoundScript_2(s32 arg0) {
    gLevelScript_script3_cursor.unk0 = -1;
    gLevelScript_script3_cursor.unk4 = -1;
    if (arg0 == 0) {
        ((s32 **) &gLevelScript_script3_cursor)[-1] = NULL;
    }
    if (arg0 == 1) {
        ((s32 **) &gLevelScript_script3_cursor)[-1] = &sPrimagenSoundScript1_2;
    }
    if (arg0 == 4) {
        ((s32 **) &gLevelScript_script3_cursor)[-1] = &sPrimagenSoundScript4_2;
    }
    if (arg0 == 2) {
        ((s32 **) &gLevelScript_script3_cursor)[-1] = &sPrimagenSoundScript2_2;
    }
    if (arg0 == 3) {
        ((s32 **) &gLevelScript_script3_cursor)[-1] = &sPrimagenSoundScript3_2;
    }
}
