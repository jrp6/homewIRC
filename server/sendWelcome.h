#ifndef _SENDWELCOME_H
#define _SENDWELCOME_H

#include <event2/bufferevent.h>
#include "connection.h"

void sendWelcome(struct connection *conn, struct bufferevent *bev);

#endif
