#include "globals/menu_desc.h"
typedef signed int s32;


char *func_00421AEC(s32 unused, s32 which) {
    switch (which) {
        case 0:
        default:
            return (char *)&gStatsAfterPakMenuDesc1;
        case 1:
            return (char *)&gStatsAfterPakMenuDesc2;
        case 2:
            return (char *)&gStatsAfterPakMenuDesc3;
        case 3:
            return (char *)&gStatsAfterPakMenuDesc4;
    }
}
