#ifndef _ECHO_CB_H
#define _ECHO_CB_H

#include <event2/bufferevent.h>

// A read callback which prints everything it receives to stdout
void echo_cb(struct bufferevent *bev, void *ctx);

#endif
