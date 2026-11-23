#include "globals/render.h"

extern char sOptionsMenuText_ResolutionLoRez[];
extern char sOptionsMenuText_ResolutionHiRez[];
extern char sOptionsMenuText_ResolutionHiRezLb[];
extern char sOptionsMenuText_ResolutionLoRezLb[];

char *Video_GetResolutionText(void) {
    switch (gResolution) {
        case 0:
        default:
            return sOptionsMenuText_ResolutionLoRez;
        case 1:
            return sOptionsMenuText_ResolutionHiRez;
        case 2:
            return sOptionsMenuText_ResolutionHiRezLb;
        case 3:
            return sOptionsMenuText_ResolutionLoRezLb;
    }
}
