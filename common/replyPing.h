#ifndef _REPLYPING_H
#define _REPLYPING_H

#include <event2/bufferevent.h>
#include "message.h"

void replyPing(const struct message *ping, struct bufferevent *bev);

#endif
