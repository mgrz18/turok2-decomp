#include "common.h"

void EngineMode13_Game_Draw(sptr);                       /* extern */

/* The app arrives in a0 and goes straight on (the host needs it passed). */
void EngineMode08_MainMenu_Draw(sptr arg0) {
    EngineMode13_Game_Draw(arg0);
}
