#ifndef _NICK_ON_CONNECT_CB_H
#define _NICK_ON_CONNECT_CB_H

#include <event2/bufferevent.h>

void nick_on_connect_cb(struct bufferevent *bev, short events, void *ptr);

#endif
