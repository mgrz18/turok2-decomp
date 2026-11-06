#include "common.h"

void Demo_UpdateInput();                            /* extern */
void Controller_LatchAll();                            /* extern */

void CEngineApp__UpdateControllers(void) {
    Controller_LatchAll();
    Demo_UpdateInput();
}
