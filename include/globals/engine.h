#ifndef GLOBALS_ENGINE_H
#define GLOBALS_ENGINE_H

#include "common.h"

/* Crash screen: file of the failed assertion */
extern char *gAssertFile;

/* Engine mode screens: text line tables (NULL entries are blank lines) */
extern char *sPakCheckText[4];
extern char *sLegalText[22];
extern char *sDemoWarningText[22];
extern char *sExpansionPakText[4];

#endif
