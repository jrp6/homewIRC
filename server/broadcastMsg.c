#include <event2/bufferevent.h>
#include <search.h>
#include <stdlib.h>
#include <string.h>

#include "connection.h"
#include "message.h"
#include "broadcastMsg.h"

// TODO: reduce redundancy between this function and transmitPrivmsg
void broadcastMsg(const struct message msg, const struct connection *conn)
{
  struct message transmitMsg = msg;

  if (!conn->hostmask) {
    return;
  }

  transmitMsg.source = conn->hostmask;

  char *transmitStr = stringifyMessage(transmitMsg);

  // Iterate through all connections and send transmitStr to them
  struct connection *conn_l;
  for (unsigned int i = 0; i < getNConns(); ++i) {
    if ((conn_l = getConnection(i))) {
      bufferevent_write(conn_l->bev, transmitStr, strlen(transmitStr));
    }
  }
  free(transmitStr);
}
