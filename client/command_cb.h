#ifndef _COMMAND_CB_H
#define _COMMAND_CB_H

#include <event2/bufferevent.h>

void command_cb(struct bufferevent *bev, void *ctx);

#endif
