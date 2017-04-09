#ifndef _BROADCASTMSG_H
#define _BROADCASTMSG_H

#include "connection.h"
#include "message.h"

void broadcastMsg(const struct message msg, const struct connection *conn);

#endif
