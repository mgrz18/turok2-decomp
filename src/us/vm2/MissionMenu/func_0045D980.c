/* Second-module copy of Mission_LevelObjectivesDone (first module). The flag test is an inline helper, so &gScene is re-addressed by symbol at each call. */
#include "common.h"
#include "globals/scene.h"
#include "protos/scene.h"



static __inline__ s32 IsSet(s32 flag) {
    return CScene__GetLinkGroupCount(&gScene, flag);
}

s32 Mission_LevelObjectivesDone_2(s32 level) {
    s32 open = 0;

    switch (level) {
        case 1:
            if (!IsSet(2) && !IsSet(3)) {
                open = 1;
            }
            break;
        case 2:
            if (!IsSet(4) && !IsSet(5)) {
                open = 1;
            }
            break;
        case 3:
            if (!IsSet(6) && !IsSet(7)) {
                open = 1;
            }
            break;
        case 4:
            if (!IsSet(8)) {
                open = 1;
            }
            break;
        case 5:
            if (!IsSet(9) && !IsSet(10)) {
                open = 1;
            }
            break;
        case 6:
            if (!IsSet(11) && !IsSet(12)) {
                open = 1;
            }
            break;
        default:
            open = 1;
            break;
    }
    return open;
}
