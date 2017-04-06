#ifndef _CLIENTREAD_CB_H
#define _CLIENTREAD_CB_H

#include <event2/bufferevent.h>

void clientRead_cb(struct bufferevent *bev, void *ctx);

#endif
