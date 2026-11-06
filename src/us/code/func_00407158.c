#include "common.h"

/* Two words at gLevelScript_script3_cursor; the pointer just before them (gLevelScript_script3_steps) picks
 * the data for the given mode. */
typedef struct {
    /* 0x0 */ s32 unk0;
    /* 0x4 */ s32 unk4;
} ModePair;

extern u8 sPrimagenSoundScript2[];
extern u8 sPrimagenSoundScript1[];
extern u8 sPrimagenSoundScript3[];
extern u8 sPrimagenSoundScript4[];
extern ModePair gLevelScript_script3_cursor;

void Primagen_SetSoundScript(s32 arg0) {
    gLevelScript_script3_cursor.unk0 = -1;
    gLevelScript_script3_cursor.unk4 = -1;
    if (arg0 == 0) {
        ((u8 **) &gLevelScript_script3_cursor)[-1] = NULL;
    }
    if (arg0 == 1) {
        ((u8 **) &gLevelScript_script3_cursor)[-1] = sPrimagenSoundScript1;
    }
    if (arg0 == 4) {
        ((u8 **) &gLevelScript_script3_cursor)[-1] = sPrimagenSoundScript4;
    }
    if (arg0 == 2) {
        ((u8 **) &gLevelScript_script3_cursor)[-1] = sPrimagenSoundScript2;
    }
    if (arg0 == 3) {
        ((u8 **) &gLevelScript_script3_cursor)[-1] = sPrimagenSoundScript3;
    }
}
