#ifndef _DEFAULTS_H
#define _DEFAULTS_H

#define DEFAULT_PORT 6667

#ifdef CLIENT_DEBUG
#undef CLIENT_DEBUG
#endif

#ifndef SERVER_DEBUG
#define SERVER_DEBUG
#endif

#endif
