#ifndef DEBUG_H
#define DEBUG_H

// TODO Later make this definable at compile time
#define DEBUG_MSG

#if defined(DEBUG_MSG)
extern int (*d_puts)(const char *s);
extern int (*d_printf)(const char *format, ...);
#else
int d_puts();
int d_printf();
#endif // DEBUG_MSG

#endif // DEBUG_H
