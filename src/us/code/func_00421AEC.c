typedef signed int s32;

extern char gStatsAfterPakMenuDesc1[];
extern char gStatsAfterPakMenuDesc2[];
extern char gStatsAfterPakMenuDesc3[];
extern char gStatsAfterPakMenuDesc4[];

char *func_00421AEC(s32 unused, s32 which) {
    switch (which) {
        case 0:
        default:
            return gStatsAfterPakMenuDesc1;
        case 1:
            return gStatsAfterPakMenuDesc2;
        case 2:
            return gStatsAfterPakMenuDesc3;
        case 3:
            return gStatsAfterPakMenuDesc4;
    }
}
