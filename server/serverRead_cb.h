#ifndef _SERVERREAD_CB_H
#define _SERVERREAD_CB_H

#include <event2/bufferevent.h>

void serverRead_cb(struct bufferevent *bev, void *ctx);

#endif
