/* port_shim.h: force-included first in every game file of the port build
 * (port/cmake/game.cmake). Host definitions for what the N64 build gets from
 * SN64 GCC itself. Never seen by the matching build. */
#ifndef PORT_SHIM_H
#define PORT_SHIM_H

#ifndef TARGET_PORT
#error "port_shim.h is for the port build only"
#endif

/* Asm labels (`extern void f(void) __asm__("Name")`) are taken verbatim by the
 * compiler, but Mach-O names C symbols with a leading underscore.
 * port/tools/hostify.py rewrites each label as T2_LABEL_PREFIX "Name". */
#define T2_STR_(x) #x
#define T2_STR(x) T2_STR_(x)
#define T2_LABEL_PREFIX T2_STR(__USER_LABEL_PREFIX__)

#endif
