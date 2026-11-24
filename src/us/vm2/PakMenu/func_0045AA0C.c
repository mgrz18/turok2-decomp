#include "globals/menu_desc.h"
typedef signed int s32;


char *PakMenu_GetStatsReturnMenu_2(s32 unused, s32 which) {
    switch (which) {
        case 0:
        default:
            return (char *)&gStatsAfterPakMenuDesc1_2;
        case 1:
            return (char *)&gStatsAfterPakMenuDesc2_2;
        case 2:
            return (char *)&gStatsAfterPakMenuDesc3_2;
        case 3:
            return (char *)&gStatsAfterPakMenuDesc4_2;
    }
}
