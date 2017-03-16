#ifndef _ACCEPT_CB_H
#define _ACCEPT_CB_H

#include <event2/listener.h>
#include <arpa/inet.h>

void accept_conn_cb(struct evconnlistener *listener, evutil_socket_t fd,
                           struct sockaddr *address, int socklen, void *ctx);
void accept_error_cb(struct evconnlistener *listener, void *ctx);

#endif
