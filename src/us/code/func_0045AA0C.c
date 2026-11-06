typedef signed int s32;

extern char gStatsAfterPakMenuDesc1_2[];
extern char gStatsAfterPakMenuDesc2_2[];
extern char gStatsAfterPakMenuDesc3_2[];
extern char gStatsAfterPakMenuDesc4_2[];

char *func_0045AA0C(void *arg0, s32 kind) {
    switch (kind) {
        case 0:
        default:
            return gStatsAfterPakMenuDesc1_2;
        case 1:
            return gStatsAfterPakMenuDesc2_2;
        case 2:
            return gStatsAfterPakMenuDesc3_2;
        case 3:
            return gStatsAfterPakMenuDesc4_2;
    }
}
