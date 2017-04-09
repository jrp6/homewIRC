#ifndef _DOJOIN_H
#define _DOJOIN_H

#include "connection.h"
#include "message.h"

void doJoin(const struct message msg, struct connection *conn);
void doPart(const struct message msg, struct connection *conn);

#endif
