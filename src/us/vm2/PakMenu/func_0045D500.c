#include "globals/render.h"
#include "globals/menu_text.h"


char *Video_GetResolutionText_2(void) {
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
