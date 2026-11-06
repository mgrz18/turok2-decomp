typedef signed int s32;

extern s32 gResolution;
extern char D_800B40E0[];
extern char D_800B40F8[];
extern char D_800B4110[];
extern char D_800B4140[];

char *Video_GetResolutionText(void) {
    switch (gResolution) {
        case 0:
        default:
            return D_800B40E0;
        case 1:
            return D_800B40F8;
        case 2:
            return D_800B4110;
        case 3:
            return D_800B4140;
    }
}
