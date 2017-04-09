#ifndef _TRANSMITPRIVMSG_H
#define _TRANSMITPRIVMSG_H

#include "connection.h"
#include "message.h"

void transmitPrivmsg(const struct message msg, const struct connection *conn);

#endif
