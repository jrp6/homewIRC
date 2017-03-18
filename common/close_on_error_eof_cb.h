#ifndef _CLOSE_ON_ERROR_EOF_CB_H
#define _CLOSE_ON_ERROR_EOF_CB_H

#include <event2/bufferevent.h>

void close_on_error_eof_cb(struct bufferevent *bev, short events, void *ptr);

#endif
