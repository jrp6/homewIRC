#ifndef _CLOSE_ON_ERROR_EOF_REMOVE_CONN_H
#define _CLOSE_ON_ERROR_EOF_REMOVE_CONN_H

#include <event2/bufferevent.h>

void close_on_error_eof_remove_conn(struct bufferevent *bev, short events, void *ptr);

#endif
